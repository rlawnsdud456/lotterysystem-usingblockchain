//
//  NodeNetWork.cpp
//  
//
//  Created by Mac on 08/11/2018.
//

#include "NodeNetWork.hpp"


#define SHAKEHAND -1
#define REQUEST_PORTPOOL -2
#define INITIALIZE -3
#define END -4
#define MAXLINE 1024 //buf 크기
#define LISTENQ 10 //Listen Q 설정
#define THREAD_NUM 5 //클라이언 동시 접속 수
void *companyShakeHandReceive(void *arg); //shakeHand with non-companyNode
void *nodeShakeHandReceive(void *arg);
void *listen(void *arg);
void *Nodelisten(void *arg);
void *checkKill(void *arg);
void *checkKillNode(void *arg);
int connectToPort(int port, char* serverIp);

int result = 0;
int cntNum = 0; //client count
struct sockaddr_in servaddr, cliaddr;
int listen_sock, accp_sock[THREAD_NUM];
socklen_t addrlen = sizeof(servaddr);
int i, status ;
pthread_t tid[10];
pid_t pid;
int myPort,portBuf = -1;
Node* noddd;

//mutex
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
bool checkHasS(vector<std::string> vect, std::string it){
    for(int i = 0; i<vect.size(); i++){
        if(vect[i] == it){
            return true;
        }
    }
    return false;
}
bool checkHasI(vector<int>vect, int it){
    for(int i = 0; i<vect.size(); i++){
        if(vect[i] == it){
            return true;
        }
    }
    return false;
}

void NodeNetWork::nodeWaitForListen(){
    if((status = pthread_create(&tid[0],NULL,&Nodelisten,NULL))!=0){
        printf("%d thread create error: %s\n", cntNum, strerror(status));
        exit(0);
    }
    cntNum++;
    if((status = pthread_create(&tid[1],NULL,&checkKillNode,NULL))!=0){
        printf("%d thread create error: %s\n", cntNum, strerror(status));
        exit(0);
    }
    cntNum--;
    
    pthread_join(tid[1], NULL);
    printf("%dkilled\n",cntNum);
    
    pthread_kill(tid[0],0);
    
    pthread_join(tid[0], NULL);
    printf("000join\n");

    return;
}
void* checkKillNode(void *arg){
    printf("333");
    time_t finish = time(NULL)+5;
    //time_t finish = time(NULL) + 50;
    //time_t finish = (time_t) *((time_t*) arg);
    while(time(NULL)<=finish){
        sleep(1);
        printf("111\n");
    }
}
void* Nodelisten(void *arg){
        listen(listen_sock, LISTENQ);
        //if(time(NULL)>=finish) close(listen_sock);
        puts("Company is waiting....");
        accp_sock[cntNum] = accept(listen_sock, (struct sockaddr *)&cliaddr, &addrlen);
        if(accp_sock[cntNum] < 0) {
            perror("accept fail");
            exit(0);
        }
        nodeShakeHandReceive((void *) &accp_sock[cntNum]);
        //if((status = pthread_create(&tid[cntNum], NULL, &companyShakeHandReceive, (void *) &accp_sock[cntNum])) != 0) {
        //    printf("%d thread create error: %s\n", cntNum, strerror(status));
        //    exit(0);
        // }
}
void *nodeShakeHandReceive(void *arg){
    int accp_sock = (int) *((int*) arg);
    int buf;
    
    read(accp_sock, &buf, 4);
    if(buf == INITIALIZE){
        printf("receiver get INITIALIZE signal\n",buf);
        buf = myPort;
        write(accp_sock,&buf,4);
        read(accp_sock,&buf,4);
        int newport = buf;
        printf("receiver get portnumber %d\n",newport);
        
        char aa[65];
        read(accp_sock,&aa,65);
        std::string newParticipants = aa;
        printf("receiver get participants %s\n",newParticipants.c_str());
        
        for(int i = 0; i < noddd->portPool.size();i++){
            
            buf = noddd->portPool[i];
            printf("receiver node has %d\n", buf);
            write(accp_sock,&buf,4);
        }
        if(!checkHasI(noddd->portPool,newport))noddd->portPool.emplace_back(newport);
        buf = END;
        write(accp_sock,&buf,4);
        
        std::string tempstring;
        for(int i = 0; i <  noddd->participantsPool.size(); i++){
            tempstring =noddd->participantsPool[i].c_str();
            strncpy(aa, tempstring.c_str(), 65);
            aa[64] = '\0';
            write(accp_sock,&aa,65);
        }
        tempstring = "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
        strncpy(aa, tempstring.c_str(), 65);
        write(accp_sock,&aa,65);
        if(!checkHasS(noddd->participantsPool,newParticipants.c_str()))noddd->participantsPool.emplace_back(newParticipants.c_str());
        
    }
    close(accp_sock);
}
bool NodeNetWork:: nodeRequestNode(int receiverPort){
    pid_t pids[5];
    int runProcess = 0;
    bool toreturn = false;
    sleep(1);
    
    char * aaa = "127.0.0.1";
    int sockfd = connectToPort(receiverPort,aaa);
    int buf = INITIALIZE;
    write(sockfd,&buf,4); // sendInitialize
    read(sockfd,&buf,4);  // receive response
    buf = myPort;         // to send myPortNumber
    write(sockfd,&buf,4); // send myPortNumber
    std::string stringBuf =  noddd->getOwnPhoneNumber(); //to send my phone number
    printf("%s\n", stringBuf.c_str());
    
    char charbuf[65];
    strncpy(charbuf, stringBuf.c_str(), 64);
    charbuf[64] = '\0';
    write(sockfd,&charbuf,65); //send my phone number
    
    while(buf != END){   //get portPool
        read(sockfd,&buf,4);
        if(buf!=END&&checkHasI(noddd->portPool,buf)) noddd->portPool.emplace_back(buf);
        else break;
    }
    
    while(stringBuf[0]!='!'){
        read(sockfd,&charbuf,65);
        stringBuf = std::string(charbuf);
        if(stringBuf[0]!='!'&&checkHasS(noddd->participantsPool,std::string(charbuf))) noddd->participantsPool.emplace_back(std::string(charbuf));
        else break;
    }
    for(int i = 0; i<noddd->participantsPool.size();i++){
        printf("participants pool have %s\n",noddd->participantsPool[i].c_str());
    }
    close(sockfd);
    return toreturn;
}



int NodeNetWork::CompanyWaitForListen(time_t finish){
    if((status = pthread_create(&tid[cntNum], NULL, &listen,NULL))!=0){
        printf("%d thread create error: %s\n", cntNum, strerror(status));
        exit(0);
    }
    cntNum++;
    if((status = pthread_create(&tid[cntNum], NULL, &checkKill,(void *) finish))!=0){
        printf("%d thread create error: %s\n", cntNum, strerror(status));
        exit(0);
    }
    printf("%daaa\n", cntNum);

    /*listen(listen_sock, LISTENQ);
    if(time(NULL)>=finish) close(listen_sock);
    puts("Company is waiting....");
    accp_sock[cntNum] = accept(listen_sock, (struct sockaddr *)&cliaddr, &addrlen);
    if(accp_sock[cntNum] < 0) {
        perror("accept fail");
        exit(0);
    }
    
    if((status = pthread_create(&tid[cntNum], NULL, &companyShakeHandReceive, (void *) &accp_sock[cntNum])) != 0) {
        printf("%d thread create error: %s\n", cntNum, strerror(status));
        exit(0);
    }*/
    
    pthread_join(tid[1], NULL);
    printf("111killed\n");

    pthread_kill(tid[0],0);
    printf("000killed\n");

    pthread_join(tid[0], NULL);
   // cntNum = 0;
    return portBuf;
}

void* checkKill(void *arg){
    printf("333");
    time_t finish = noddd->getFinishTime();
    //time_t finish = time(NULL) + 50;
    //time_t finish = (time_t) *((time_t*) arg);
    while(time(NULL)<=finish){
        sleep(5);
        printf("111\n");
    }
}
void* listen(void *arg){
    while(time(NULL)<= noddd->getFinishTime()){
        listen(listen_sock, LISTENQ);
        //if(time(NULL)>=finish) close(listen_sock);
        puts("Company is waiting....");
        accp_sock[cntNum] = accept(listen_sock, (struct sockaddr *)&cliaddr, &addrlen);
        if(accp_sock[cntNum] < 0) {
            perror("accept fail");
            exit(0);
        }
        companyShakeHandReceive((void *) &accp_sock[cntNum]);
        //if((status = pthread_create(&tid[cntNum], NULL, &companyShakeHandReceive, (void *) &accp_sock[cntNum])) != 0) {
        //    printf("%d thread create error: %s\n", cntNum, strerror(status));
        //    exit(0);
       // }
    }
}
void *companyShakeHandReceive(void *arg){
    int accp_sock = (int) *((int*) arg);
    int buf;
    
    read(accp_sock, &buf, 4);
    if(buf == INITIALIZE){
        printf("company get INITIALIZE signal\n",buf);
        buf = myPort;
        write(accp_sock,&buf,4);
        read(accp_sock,&buf,4);
        int newport = buf;
        printf("company get portnumber %d\n",newport);

        char aa[65];
        read(accp_sock,&aa,65);
        std::string newParticipants = aa;
        printf("company get participants %s\n",newParticipants.c_str());
        
        for(int i = 0; i < noddd->portPool.size();i++){
            
            buf = noddd->portPool[i];
            printf("company node has %d\n", buf);
            write(accp_sock,&buf,4);
        }
        noddd->portPool.emplace_back(newport);
        buf = END;
        write(accp_sock,&buf,4);
        
        std::string tempstring;
        for(int i = 0; i <  noddd->participantsPool.size(); i++){
            tempstring =noddd->participantsPool[i].c_str();
            strncpy(aa, tempstring.c_str(), 65);
            aa[64] = '\0';
            write(accp_sock,&aa,65);
        }
        tempstring = "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
        strncpy(aa, tempstring.c_str(), 65);
        write(accp_sock,&aa,65);
        noddd->participantsPool.emplace_back(newParticipants.c_str());

    }
    close(accp_sock);
}



bool NodeNetWork:: shakeHandToNode(int receiverPort, int mode){
    pid_t pids[5];
    int runProcess = 0;
    bool toreturn = false;
    sleep(1);
    pids[runProcess] = fork();
    
    if(pids[runProcess] < 0) {
        exit(0);
    }
    
    if(pids[runProcess] == 0) {
        char * aaa = "127.0.0.1";
        int sockfd = connectToPort(receiverPort,aaa);
        int buf = -33333;
        write(sockfd,&buf,4);
        exit(0);
    } else { //부모 프로세스
        return toreturn;
    }
}
bool NodeNetWork:: initializeFromCompany(int receiverPort){
    pid_t pids[5];
    int runProcess = 0;
    bool toreturn = false;
    sleep(1);

    char * aaa = "127.0.0.1";
    int sockfd = connectToPort(receiverPort,aaa);
    int buf = INITIALIZE;
    write(sockfd,&buf,4); // sendInitialize
    read(sockfd,&buf,4);  // receive response
    buf = myPort;         // to send myPortNumber
    write(sockfd,&buf,4); // send myPortNumber
    std::string stringBuf =  noddd->getOwnPhoneNumber(); //to send my phone number
    printf("%s\n", stringBuf.c_str());

    char charbuf[65];
    strncpy(charbuf, stringBuf.c_str(), 64);
    charbuf[64] = '\0';
    write(sockfd,&charbuf,65); //send my phone number
    
    while(buf != END){   //get portPool
        read(sockfd,&buf,4);
        if(buf!=END) noddd->portPool.emplace_back(buf);
        else break;
    }

    while(stringBuf[0]!='!'){
        read(sockfd,&charbuf,65);
        stringBuf = std::string(charbuf);
        if(stringBuf[0]!='!') noddd->participantsPool.emplace_back(std::string(charbuf));
        else break;
    }
    for(int i = 0; i<noddd->participantsPool.size();i++){
        printf("participants pool have %s\n",noddd->participantsPool[i].c_str());
    }
    close(sockfd);
    return toreturn;
}

int connectToPort(int port, char *serverIp){
    struct sockaddr_in servaddr;
    int strlen = sizeof(servaddr);
    int sockfd, buf, cNum;//cNum 연결 번호
    
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket fail");
        exit(0);
    }
    
    memset(&servaddr, 0, strlen);
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, serverIp, &servaddr.sin_addr);
    servaddr.sin_port = htons(port);
    
    if(connect(sockfd, (struct sockaddr *)&servaddr, strlen) < 0) {
        perror("connect fail");
    }
    return sockfd;
}



NodeNetWork::NodeNetWork(int port,Node& node) {
    myPort = port;
    ownNode = &node;
    noddd = &node;
    if((listen_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket Fail");
        exit(0);
    }
    
    memset(&servaddr, 0, sizeof(servaddr)); //0으로 초기화
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    
    //bind 호출
    if(::bind(listen_sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind Fail");
        exit(0);
    }
   
}




