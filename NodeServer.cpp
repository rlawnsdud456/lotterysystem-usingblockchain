//
//  NodeServer.cpp
//  
//
//  Created by KJY on 18/11/2018.
//

#include "NodeServer.hpp"
#define THREAD_NUM 5 //클라이언 동시 접속 수
#define LISTENQ 10 //Listen Q 설정
#define INITIALIZE -3
#define INTERACTNODE -4


void *companyShakeHandReceive(void *arg,int sock,int portNumber); //shakeHand with non-companyNode
void NodeShakeHandReceive(void *arg,int sock,int portNumber);
static void *listen(void *arg);
void *listenNode(void* arg);
int bindTo(int portNumber);

static int result = 0;
static int cntNum = 0; //client count
static struct sockaddr_in servaddr, cliaddr;
static int  accp_sock[THREAD_NUM];
static socklen_t addrlen = sizeof(servaddr);
static int i, status ;
static pthread_t tid[10];
static pid_t pid;
static int myPort,portBuf = -1;
static Node* noddd;
static bool lockCreate = false;
static bool listening1 = false;
static bool listening2 = false;

//mutex
static pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;

int bindTo(int portNumber){
    printf("created %d\n", portNumber);

    int listen_sock1;
    if((listen_sock1 = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket Fail");
    }
    
    memset(&servaddr, 0, sizeof(servaddr)); //0으로 초기화
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(portNumber);
    
    //bind 호출
    if(::bind(listen_sock1, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        printf("bindFail%d\n", portNumber);
        perror("bind Fail");
        exit(0);
    }
    lockCreate = false;
    return listen_sock1;
}

int NodeServer::CompanyWaitForListen(time_t finish){
    //while(cntNum<1){
    lockCreate = true;
    int tempcntNum = 0;
    int portNum = noddd->portQueue.front();
    noddd->portQueue.pop();
    if((status = pthread_create(&tid[0], NULL, &listen,&portNum))!=0){
        printf("%d thread create error: %s\n", cntNum, strerror(status));
    }
            
    while(time(NULL)+10 < noddd->getFinishTime() ){}
    pthread_exit(NULL);
    //printf("inserver1 %d: %d\n",tempcntNum);

    //}
    return portBuf;
}

int NodeServer::NodeWaitForListen(){
    //while(cntNum<2){
    lockCreate = true;
    int tempcntNum = 1;
    int portNum = noddd->portQueue.front();
    noddd->portQueue.pop();
    if((status = pthread_create(&tid[tempcntNum], NULL, &listenNode,&portNum))!=0){
        printf("%d thread create error: %s\n", cntNum, strerror(status));
        }
            
    while(time(NULL)+10 < noddd->getFinishTime() ){}
    pthread_exit(NULL);
    //}
    return portBuf;
}

static void* listen(void *arg){
    cntNum++;

    int portNumber = (int) *((int*) arg);
    int listen_sock = bindTo(portNumber);
    while(time(NULL)+10<= noddd->getFinishTime()){

        if(!listening1){
            listening1 = true;
            listen(listen_sock, LISTENQ);
            puts("Company is waiting....1");
            accp_sock[cntNum] = accept(listen_sock, (struct sockaddr *)&cliaddr, &addrlen);
            if(accp_sock[cntNum] < 0) {
                perror("accept fail");
            }
            companyShakeHandReceive((void *) &accp_sock[cntNum],listen_sock,portNumber);
            
            //printf("fifi %ld %ld \n", time(NULL),noddd->getFinishTime());
            listening1 = false;
        }
    }
    //printf("listen end\n");
}

void* listenNode(void *arg){
    int portNumber = (int) *((int*) arg);
    int listen_sock = bindTo(portNumber);
    cntNum++;

    while(time(NULL)+10<= noddd->getFinishTime()){

        if(!listening2){
            listening2 = true;
            puts("Node is waiting....1");
            listen(listen_sock, LISTENQ);
            accp_sock[cntNum] = accept(listen_sock, (struct sockaddr *)&cliaddr, &addrlen);
            if(accp_sock[cntNum] < 0) {
                perror("accept fail");
            }
            NodeShakeHandReceive((void *) &accp_sock[cntNum],listen_sock,portNumber);
            
            //printf("fifi %ld %ld \n", time(NULL),noddd->getFinishTime());
            listening2 = false;
        }
        
    }
}
void NodeShakeHandReceive(void *arg,int sock,int portNumber){
    int accp_sock = (int) *((int*) arg);
    int buf;
    noddd->checkDataUsing(true);

    read(accp_sock, &buf, 4); //INITIALIZE ?
    if(buf == INTERACTNODE){

        if(noddd->portPoolQueue.empty()) buf = -1;
        else{
            int size = noddd->portPoolQueue.size();
            for(int i=0;i<size;i++){
                buf = noddd->portPoolQueue.front();
                noddd->portPoolQueue.pop();
                noddd->portPoolQueue.push(buf);
                
                write(accp_sock,&buf,4);
            }
        }
        buf = -1;
        write(accp_sock,&buf,4);

        
        int forPortpool;
        read(accp_sock,&forPortpool,4); //client portNumber
        int toIden = forPortpool;
        read(accp_sock,&forPortpool,4); //client portNumber
        while(forPortpool>0){
            noddd->portPoolQueue.push(forPortpool);
            read(accp_sock,&forPortpool,4);
        }

        
        printf("Node get INITIALIZE signal with portNumber:%d\n",toIden);
        
        int poolsize = noddd->participantsPool.size();
        write(accp_sock,&poolsize,4);
        
        int requesterPoolsize;
        read(accp_sock,&requesterPoolsize,4);
        
        for(int i = 0 ; i < poolsize; i++){
            //char kkk = kk[i];
            string kkk = noddd->participantsPool[i];
            write(accp_sock,kkk.c_str(),65);
        }
        requesterPoolsize;
        read(accp_sock,&requesterPoolsize,4);
        for(int i = 0; i < requesterPoolsize; i++){
            char buff[65];
            string temp = "";
            temp.resize(65);
            read(accp_sock,&buff[0],65);
            temp = buff;
            //printf("get string %s\n", temp.c_str());
            if(!noddd->participantsPoolHas(temp))noddd->participantsPool.push_back(temp);
        }
        
        

        int chainSize ;
        read(accp_sock,&chainSize,4);
        //printf("chainSize:%d\n",chainSize);
        //printf("In server have:%d client have:%d so i send :\n",noddd->ownChain.getLength(), chainSize);
        if(noddd->ownChain.getLength() < chainSize) {
            buf = 1 ;
            write(accp_sock,&buf,4);
        }
        else if(noddd->ownChain.getLength() == chainSize){
            buf = 0;
            write(accp_sock,&buf,4);
            read(accp_sock,&buf,4);
            if(buf > noddd->ownChain.participants_hash.size()){
                buf = 1;
            }
            else if (buf == noddd->ownChain.participants_hash.size()){
                buf = 2;
                write(accp_sock,&buf,4);
                char buff[65];
                string temp = "";
                temp.resize(65);
                read(accp_sock,&buff[0],65);
                
                temp = buff;
                if(getValue(temp) > getValue(noddd->ownChain.getLastBlock().CalculateHash())){
                    buf = 1;
                }
                else {
                    buf = -1;
                    chainSize = -1;
                }
            }
            else {
                buf = -1;
                chainSize = -1;
            }
            write(accp_sock,&buf,4);
        }
        else {
            buf = -1;
            write(accp_sock,&buf,4);
            chainSize = -1;
        }
        //printf("Inserver after check chainsize send %d\n",buf);
        
        vector<string> temp;
        genesisBlock genesis(temp);
        if(chainSize!=-1){
            time_t timeBuf;
            read(accp_sock,&timeBuf,sizeof(time_t));
            genesis.setCreatedTime(timeBuf);
        }
        Blockchain tempChain = Blockchain(genesis);

        //printf("before expected seg1\n");

        for(int i = 1; i <chainSize; i ++){
            //printf("before expected seg11\n");

            int nonce;
            time_t addtime,createdtime;
            vector<string> tempPartiVector;
            //printf("before expected seg2\n");

            read(accp_sock,&nonce,4);//get Nonce
            read(accp_sock,&addtime,sizeof(time_t));
            read(accp_sock,&createdtime,sizeof(time_t));
            
            char buff[65];
            string blockHash = "";
            blockHash.resize(65);
            read(accp_sock,&buff[0],65);
            blockHash = buff;
            
            int partisize;
            read(accp_sock,&partisize,4);
            
            for(int i = 0; i < partisize; i++){
                string strTemp = "";
                strTemp.resize(65);
                read(accp_sock,&buff[0],65);
                strTemp = buff;
                tempPartiVector.push_back(strTemp);
            }
            //printf("before expected seg3\n");

            if(tempChain.addBlock(Block(i,nonce,addtime, createdtime,tempPartiVector,tempChain.getLastBlock().CalculateHash())))buf=1;
            else buf = -1;
            write(accp_sock,&buf,4);
            if(buf == -1 ) break;
            //printf("before expected seg4\n");

            
        }
        if(buf==1) {
            //printf("copy chain\n");

            noddd->ownChain.copyChain(tempChain);
            int checkPool = noddd->participantsPool.size();
            int it = 0;
            //printf("after expected seg\n");

            for(int i = 0; i < checkPool; i++,it++){
                if(noddd->ownChain.checkParticipants(noddd->participantsPool[it])) {
                    //printf("erase %s \n", noddd->participantsPool[it].c_str());
                    noddd->participantsPool.erase(noddd->participantsPool.begin()+it);
                    it--;
                }
            }
            //printf("after expected seg\n");
        }
    }
    noddd->checkDataUsing(false);

    close(accp_sock);
    noddd->portQueue.push(portNumber);
    printf("serverside end\n");
}

void *companyShakeHandReceive(void *arg,int sock,int portNumber){
    int accp_sock = (int) *((int*) arg);
    int buf;
    noddd->checkDataUsing(true);

    read(accp_sock, &buf, 4); //INITIALIZE ?
    if(buf == INITIALIZE){
        
        buf = portNumber+1;
        write(accp_sock,&buf,4); //send myPortNumber
        if(noddd->portPoolQueue.empty()) buf = -1;
        else{
            int size = noddd->portPoolQueue.size();
            for(int i=0;i<size;i++){
                buf = noddd->portPoolQueue.front();
                noddd->portPoolQueue.pop();
                noddd->portPoolQueue.push(buf);
                
                write(accp_sock,&buf,4);
            }
        }
        buf = -1;
        write(accp_sock,&buf,4);
        
        int forPortpool;
        read(accp_sock,&forPortpool,4); //client portNumber
        
        //printf("company get INITIALIZE signal with portNumber:%d\n",forPortpool);
        buf = myPort;
        time_t finish = noddd->getFinishTime();
        
        write(accp_sock,&finish,sizeof(time_t)); //send finishTime
        int poolsize = noddd->participantsPool.size() ;
        write(accp_sock,&poolsize,4);
        
        for(int i = 0 ; i < poolsize; i++){
            //char kkk = kk[i];
            string kkk = noddd->participantsPool[i];
            write(accp_sock,kkk.c_str(),65);
        }
        
        char buff[65];
        string tempString = "";
        tempString.resize(65);
        read(accp_sock,&buff[0],65);
        tempString = buff;
        noddd->participantsPool.push_back(tempString);
        
        
        //send block
        int chainSize = noddd->ownChain.getLength();
        //printf("I have %d size of chain\n", chainSize);
        write(accp_sock,&chainSize,4);
        read(accp_sock,&buf,4);
        
        if(buf==1){
            //printf("start sending block information\n");

            Block tempBLock = noddd->ownChain.getBlock(0);

            time_t timeBuf = tempBLock.getCreatedTime();
            write(accp_sock,&timeBuf,sizeof(time_t));
            
            for(int i = 1; i < chainSize; i++){
                tempBLock = noddd->ownChain._vChain[i];
                buf = tempBLock.getNonce();
                write(accp_sock,&buf,4);
                
                timeBuf = tempBLock.getAddingTime();
                write(accp_sock,&timeBuf,sizeof(time_t));
                timeBuf = tempBLock.getCreatedTime();
                write(accp_sock,&timeBuf,sizeof(time_t));
                
                string strTemp = tempBLock.getHash();
                write(accp_sock,strTemp.c_str(),65);
                
                int partisize = tempBLock.getParticiSize();
                write(accp_sock,&buf,4);
                
                for(int i = 0; i < partisize; i++ ){
                    strTemp = tempBLock.participantsVector[i];
                    write(accp_sock,strTemp.c_str(),65);
                }
            }
        }

        if(!noddd->portPoolQueueHas(noddd->portPoolQueue,forPortpool))noddd->portPoolQueue.push(forPortpool);
        noddd->checkDataUsing(false);
    }
    close(accp_sock);
    noddd->portQueue.push(portNumber);
}

NodeServer::NodeServer(int port,Node& node) {
    myPort = port;
    ownNode = &node;
    noddd = &node;
    ownNode->mode = -2;


}
