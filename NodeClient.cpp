//
//  NodeClient.cpp
//  
//
//  Created by KJY on 18/11/2018.
//

#include "NodeClient.hpp"


#define INITIALIZE -3
#define INTERACTNODE -4
#define MAXLINE 1024 //buf 크기
#define TOTALFORK 5 //클라이언트 수

int NodeClient::connectToPort(int portNumber,int receiverPort, char *serverIp){
    struct sockaddr_in servaddr;
    int strlen = sizeof(servaddr);
    int sockfd, buf, cNum;//cNum 연결 번호

    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket fail");
    }
    
    memset(&servaddr, 0, strlen);
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, serverIp, &servaddr.sin_addr);
    servaddr.sin_port = htons(receiverPort);
    ownNode->checkDataUsing(true);
    int connecta = connect(sockfd, (struct sockaddr *)&servaddr, strlen);
    if(connecta<0){
        
        ownNode->lock = false;
        return -1;
    }
    //printf("I will connect to %d and connected %d\n", receiverPort,connecta);
    //printf("connected with:%d %ld %d\n", receiverPort,time(NULL),connecta);
    
    buf = INTERACTNODE;
    vector<int> tempQue;
    int queueSize = ownNode->portPoolQueue.size();
    for(int i = 0; i < queueSize;i++) {
        int temp = ownNode->portPoolQueue.front();
        tempQue.push_back(temp);
        ownNode->portPoolQueue.pop();
        ownNode->portPoolQueue.push(temp);
    }
    
    write(sockfd,&buf,4); // send INTERACTNODE
    
    read(sockfd,&buf,4);
    
    while(buf>0){
        if(!ownNode->portPoolQueueHas(ownNode->portPoolQueue,buf))ownNode->portPoolQueue.push(buf);
        for(int i = 0; i < tempQue.size(); i++){
            if(tempQue[i] == buf) {
                tempQue.erase(tempQue.begin()+i);
                i--;
            }
        }
        read(sockfd,&buf,4);
    }
    
    int sisi = tempQue.size();
    write(sockfd,&portNumber,4);
    for(int i = 0; i < sisi; i++){
        int tempNumber = tempQue[i];
        write(sockfd,&tempNumber,4);//send My PortNumber
    }
    sisi = -1;
    write(sockfd,&sisi,4);
    
    vector<string> tempPool;
    for(int i = 0; i< ownNode->participantsPool.size();i++){
        tempPool.push_back(ownNode->participantsPool[i]);
    }
    int poolsize;
    read(sockfd,&poolsize,4);
    
    int requestorPoolsize = tempPool.size();
    write(sockfd,&requestorPoolsize,4);

    for(int i = 0; i < poolsize; i++){
        char buff[65];
        string temp = "";
        temp.resize(65);
        read(sockfd,&buff[0],65);

        temp = buff;
        //printf("get string %s\n", temp.c_str());
        if(!ownNode->participantsPoolHas(temp)){
            ownNode->participantsPool.push_back(temp);
        }
        for(int i = 0; i < tempPool.size(); i++){
            if(tempPool[i] == temp) tempPool.erase(tempPool.begin()+i);
        }
    }
    
    requestorPoolsize = tempPool.size();
    write(sockfd,&requestorPoolsize,4);
    
    for(int i = 0 ; i < requestorPoolsize; i++){
        //char kkk = kk[i];
        string kkk = tempPool[i];
        write(sockfd,kkk.c_str(),65);
    }

    while(buf>0){
        if(!ownNode->portPoolQueueHas(ownNode->portPoolQueue,buf))ownNode->portPoolQueue.push(buf);
        read(sockfd,&buf,4);
    }

    int chainSize = ownNode->ownChain.getLength();
    printf("I have %d size of chain\n", chainSize);
    write(sockfd,&chainSize,4);
    read(sockfd,&buf,4);
    //printf("In client get %d\n", buf);
    if(buf==0){
        buf = ownNode->ownChain.participants_hash.size();
        write(sockfd,&buf,4);
        read(sockfd,&buf,4);
        if(buf == 2){
            string kkk = ownNode->ownChain.getLastBlock().CalculateHash();
            write(sockfd,kkk.c_str(),65);
            read(sockfd,&buf,4);
        }
    }
    
    if(buf==1){
        printf("start sending block information\n");
        
        Block tempBLock = ownNode->ownChain.getBlock(0);
        
        time_t timeBuf = tempBLock.getCreatedTime();
        write(sockfd,&timeBuf,sizeof(time_t));
        
        for(int i = 1; i < chainSize; i++){
            tempBLock = ownNode->ownChain._vChain[i];
            buf = tempBLock.getNonce();
            write(sockfd,&buf,4);
            
            timeBuf = tempBLock.getAddingTime();
            write(sockfd,&timeBuf,sizeof(time_t));
            
            timeBuf = tempBLock.getCreatedTime();
            write(sockfd,&timeBuf,sizeof(time_t));
            
            string strTemp = tempBLock.getHash();
            write(sockfd,strTemp.c_str(),65);
            
            int partisize = tempBLock.getParticiSize();
            write(sockfd,&partisize,4);
            
            for(int i = 0; i < partisize; i++ ){
                strTemp = tempBLock.participantsVector[i];
                write(sockfd,strTemp.c_str(),65);
            }
            read(sockfd,&buf,4);
            if(buf==-1)break;
        }
    }
    
    
    ownNode->checkDataUsing(false);

    // receive response
    ownNode->lock = false;
    printf("in client request end:%d\n",receiverPort);
    return sockfd;
}

NodeClient::NodeClient(int portNumber,int receiverPort,Node& node,bool company){
    ownNode = &node;
    ownNode->mode = -2;
    struct sockaddr_in servaddr;
    int strlen = sizeof(servaddr);
    int buf, cNum;//cNum 연결 번호
    char* serverIp = "127.0.0.1";
    int sockfd;
    fd_set fdset;
    
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket fail");
    }
    //int offset = (time(NULL) % 3);
    //if(company) receiverPort += offset;
    memset(&servaddr, 0, strlen);
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, serverIp, &servaddr.sin_addr);
    servaddr.sin_port = htons(receiverPort);
    int temp = receiverPort;
    if(company){
        printf("start Initialize\n");
        int status = connect(sockfd, (struct sockaddr *)&servaddr, strlen);
        printf("client connect to companyServer get %d \n", status);
        if(status < 0 ) {
            printf("initialize fail\n");
            exit(0);
        }
        buf = INITIALIZE;
        //Initialize from company
        write(sockfd,&buf,4); // sendInitialize
        
        read(sockfd,&buf,4);  // get portNumber from company
        ownNode->portPoolQueue.push(buf);
        
        read(sockfd,&buf,4);
        ownNode->checkDataUsing(true);
        if(buf<0) ownNode->mode = -3;
        else {
            while(buf>0){
                if(!ownNode->portPoolQueueHas(ownNode->portPoolQueue,buf))ownNode->portPoolQueue.push(buf);
                read(sockfd,&buf,4);
            }
        }
        write(sockfd,&portNumber,4);//send My portNumber

        time_t finish;
        read(sockfd,&finish, sizeof(time_t));//read the finish time from server

        ownNode->setFinishTime(finish);
        
        int poolsize;
        read(sockfd,&poolsize,4);
        
        char buff[65];
        for(int i = 0; i < poolsize; i++){
            string temp = "";
            temp.resize(65);
            read(sockfd,&buff[0],65);
            temp = buff;
            if(!ownNode->participantsPoolHas(temp)){
                ownNode->participantsPool.push_back(temp);
            }
        }
        
        string kkk = ownNode->getOwnPhoneNumber();
        write(sockfd,kkk.c_str(),65);
        
        int chainSize ;
        read(sockfd,&chainSize,4);
        //printf("chainSize:%d\n",chainSize);
        buf =1;
        write(sockfd,&buf,4);
        time_t timeBuf;
        
        read(sockfd,&timeBuf,sizeof(time_t));
        
        vector<string> temp;
        genesisBlock genesis(temp);
        genesis.setCreatedTime(timeBuf);
        ownNode->ownChain = Blockchain(genesis);
        
        for(int i = 1; i <chainSize; i ++){
            int nonce;
            time_t addtime,createdtime;
            vector<string> tempPartiVector;
            read(sockfd,&nonce,4);//get Nonce
            read(sockfd,&addtime,sizeof(time_t));
            read(sockfd,&createdtime,sizeof(time_t));
            
            string blockHash = "";
            blockHash.resize(65);
            read(sockfd,&buff[0],65);
            blockHash = buff;
            
            int partisize;
            read(sockfd,&partisize,4);
            
            for(int i = 0; i < partisize; i++){
                string strTemp = "";
                strTemp.resize(65);
                read(sockfd,&buff[0],65);
                strTemp = buff;
                tempPartiVector.push_back(strTemp);
            }
            ownNode->ownChain.addBlock(Block(i,nonce,addtime, createdtime,tempPartiVector,ownNode->ownChain.getLastBlock().CalculateHash()));
        }
        
        ownNode->checkDataUsing(false);
        
        ownNode->lock = false;
        printf("end Initialize\n");

    }
}


