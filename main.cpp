//
//  main.cpp
//  
//
//  Created by Mac on 03/11/2018.
//

#include <stdio.h>
#include <cstdint>
#include <time.h>
#include <string>
#include "Blockchain.hpp"
#include "sha256.hpp"
#include <stdlib.h>
#include <thread>
#include "Node.hpp"
#include "comPanyNode.hpp"
#include <iostream>
#include "genesisBlock.hpp"
#include "util.hpp"
#include "NodeClient.hpp"
#include "NodeServer.hpp"
#include <pthread.h>

#define comPanyNodePort 1012
using namespace std;

time_t finishtime;
int portNumber;
bool created = false;
void* comPanyWait(void* arg){
    NodeServer server1 = (NodeServer) *((NodeServer*) arg);
    server1.CompanyWaitForListen(finishtime);
    printf("Company server close\n");
}

void* listenNodeMain(void* arg){
    NodeServer server1 = (NodeServer) *((NodeServer*) arg);
    server1.NodeWaitForListen();
    printf("Node server close\n");
}

void* requestNode(void* arg){
    //printf("1111I will request connect to\n");

    NodeClient nodec = (NodeClient) *((NodeClient*) arg);
    char* serverIp = "127.0.0.1";
    
    nodec.ownNode->checkDataUsing(true);
    int temp = nodec.ownNode->portPoolQueue.front();
    nodec.ownNode->portPoolQueue.pop();
    if(temp == nodec.ownNode->portNumber) {
        temp = nodec.ownNode->portPoolQueue.front();
        nodec.ownNode->portPoolQueue.pop();
    }
    if(!nodec.ownNode->portPoolQueueHas(nodec.ownNode->portPoolQueue,temp))nodec.ownNode->portPoolQueue.push(temp);
    
    nodec.ownNode->checkDataUsing(false);
    
    printf("Request connect to %d\n", temp);
    nodec.connectToPort(nodec.ownNode->portNumber,temp,serverIp);
    created = false;
    nodec.ownNode->lock = false;
}

int main(int argc, char *argv[]){
    int temp;
    
    finishtime = time(NULL) + 180;
    if(atoi(argv[1]) == 0){
        if(argc==3) finishtime = time(NULL) + atoi(argv[2]);
        vector<string> temp;
        genesisBlock genesis = genesisBlock(temp);
        
        portNumber = comPanyNodePort;
        comPanyNode tempCompany = comPanyNode(finishtime,portNumber,true);
        tempCompany.ownChain.setGenesisBlock(genesis);
        printf("chain size: %d\n", tempCompany.ownChain._vChain.size());
        
        NodeServer server = NodeServer(portNumber,tempCompany);
        pthread_t pid[3];
        cout << tempCompany.getFinishTime() << endl;
        int status = pthread_create(&pid[0], NULL, &comPanyWait,&server);
        int status2 = pthread_create(&pid[1], NULL, &listenNodeMain,&server);

        pthread_join(pid[0], NULL);
        pthread_join(pid[1], NULL);

        printf("Company Close\n");
    }
    else{
        
        printf("start Initialize\n");
        sleep(6);
        
        portNumber = atoi(argv[2]);
        Node node = Node(finishtime, to_string(portNumber+1),portNumber,false);
        
        NodeServer server = NodeServer(portNumber,node);

        pthread_t pid[3];

        int status = pthread_create(&pid[0], NULL, &listenNodeMain,&server);

        node.lock = true;
        int poolsi = 0;

        NodeClient nodeclient = NodeClient(portNumber,comPanyNodePort,node,true);
        finishtime = node.getFinishTime();
        
        time_t temprest = ( finishtime - time(NULL) ) / 3;
        if(temprest < 30) temprest = 30;
        
        while(time(NULL)+16 < finishtime){
 
            printf("running\n");
            sleep(5);
            node.checkDataUsing(true);
            if(!node.participantsPoolHas(node.getOwnPhoneNumber()))
                node.participantsPool.push_back(node.getOwnPhoneNumber());

            for(int i = 0; i<node.participantsPool.size();i++){
                cout << "I have parti: " << node.participantsPool[i] << endl;
            }
            if(time(NULL)>node.getFinishTime()-temprest && node.participantsPool.size()>=1){
                printf("Let's mine rest!!\n");
                bool yes = node.mineTheBlock();
                printf("Minded? %d\n", node.ownChain.getLength());
            }
            if(node.participantsPool.size() >=3){
                printf("Let's mine!!\n");
                bool yes = node.mineTheBlock();
                printf("Minded? %d\n", node.ownChain.getLength());
            }
            node.checkDataUsing(false);

            if(node.lock == false){
                
                if(!node.portPoolQueue.empty()){
                    node.lock = true;
                    created = true;
                    pthread_create(&pid[1], NULL, &requestNode,&nodeclient);
                }
            }
            finishtime = node.getFinishTime();
        }
        pthread_join(pid[0],NULL);
        //while(time(NULL)+10 < finishtime){}
        node.checkDataUsing(false);
        pthread_kill(pid[1], NULL);
        Blockchain tempchain = node.ownChain;
        Block tempBlock = tempchain.getBlock(0);

        int chainsize = tempchain.getLength();
        printf("\n");
        cout << "mynumber: " << node.getOwnPhoneNumber().c_str() << endl;
        string ss;
        for(int h = 0 ; h < chainsize;h++){
            tempBlock = tempchain.getBlock(h);
            int size = tempBlock.getParticiSize();
            cout << "Block hash is: " << tempBlock.CalculateHash() << endl;
            ss += tempBlock.CalculateHash();
            for(int k = 0; k < size; k++){
                cout << "Block "  << h << " has: " <<tempBlock.participantsVector[k] << endl;
            }
            printf("\n");
        }
        ss = sha256(ss);
        string winner;
        unsigned long criValue= getValue(ss);
        cout << "criteria is: " << ss << "value: " << criValue << endl;
        
        unsigned long minn = 99999999;
        for(int h = 0; h < chainsize; h++){
            tempBlock = tempchain.getBlock(h);
            int size = tempBlock.getParticiSize();
            for(int k = 0; k < size; k++){
                if(minn> labs(criValue - getValue(tempBlock.participantsVector[k]))) {
                    winner = tempBlock.participantsVector[k];
                    minn = labs(criValue - getValue(tempBlock.participantsVector[k]));
                }
            }
        }
        bool iam = winner == node.getOwnPhoneNumber();
        cout << "winner is " << winner << " Iam? " << iam << endl;
        if(iam){
            cout << "The condition of winning is: " << tempchain.getWinnerCondition().c_str() << endl;
        }
    }
}

