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
#include <fstream>
#include "genesisBlock.hpp"
#include "util.hpp"
#include "NodeClient.hpp"
#include "NodeServer.hpp"
#include <pthread.h>
#include <signal.h>
#include <unistd.h>



#define comPanyNodePort 11012
#define number_demo_node 10
#define demoMode 2
#define companyMode 0
#define normalNode 1
using namespace std;

time_t finishtime;
int portNumber;
bool created = false;
bool bindFailMain = false;
void* comPanyWait(void* arg){
    NodeServer server1 = (NodeServer) *((NodeServer*) arg);
    int status = server1.CompanyWaitForListen(finishtime);
    if(status <0) bindFailMain = true;
    printf("Company server close\n");
    return 0;
}

void* listenNodeMain(void* arg){
    NodeServer server1 = (NodeServer) *((NodeServer*) arg);
    int status = server1.NodeWaitForListen();
    if(status <0 ) bindFailMain = true;
    printf("Node server close\n");
    return 0;
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
    return 0;
}

int main(int argc, char *argv[]){
    if(argc < 2) {
        printf("You should input what kind of mode you want to run\n0(company) or 1(normalNode) or 2(DemoRun)\n");
        return 0;
    }
    
    finishtime = time(NULL) + 120;
    
    
    int mode = atoi(argv[1]);
    int forkk[number_demo_node] = {0,0,0,0,0,0,0,0,0,0};
    bool create = true;
    int myOrder;
    
    if(mode==demoMode){
        for(int i = 0; i<number_demo_node;i++){
            if(create){
                forkk[i] = fork();
                if(forkk[i]==0) {
                    myOrder = i;
                    create = false;
                    portNumber = 5000+i;
                    mode = normalNode;
                    sleep(3+i);
                }
            }
            else break;
        }
    }
    
    if(mode!=normalNode){
        if(argc==3) finishtime = time(NULL) + atoi(argv[2]);
        vector<string> temp;
        genesisBlock genesis = genesisBlock(temp);
        
        portNumber = comPanyNodePort;
        comPanyNode tempCompany = comPanyNode(finishtime,portNumber,true);
        tempCompany.ownChain.setGenesisBlock(genesis);
        printf("chain size: %lu\n", tempCompany.ownChain._vChain.size());
        
        NodeServer server = NodeServer(portNumber,tempCompany);
        pthread_t pid[3];
        cout << tempCompany.getFinishTime() << endl;
        int status = pthread_create(&pid[0], NULL, &comPanyWait,&server);
        sleep(1);
        int status2 = pthread_create(&pid[1], NULL, &listenNodeMain,&server);
        
        sleep(1);

        if(mode==demoMode){
            int status;
            for(int i = 0; i < number_demo_node;i++){
                wait(&status);
            }
            if(bindFailMain){
                pthread_kill(pid[0],0);
                pthread_kill(pid[1],0);
                exit(0);
            }
        }
        pthread_join(pid[0], NULL);
        pthread_join(pid[1], NULL);
        
        printf("Company Close\n");
    }
    
    if(mode==normalNode){
        printf("start Initialize in main\n");
        
        if(create && mode==normalNode){
            if(argc!=3){
                printf("You should input the portNumber as arguments\n");
                return 0;
            }
            sleep(6);
            portNumber = atoi(argv[2]);
            myOrder = portNumber;
        }
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
            if(!node.participantsPoolHas(node.getOwnPhoneNumber()))
                node.participantsPool.push_back(node.getOwnPhoneNumber());
            
            for(int i = 0; i<node.participantsPool.size();i++){
                cout << "I have parti: " << node.participantsPool[i] << endl;
            }
            if(time(NULL)>node.getFinishTime()-temprest && node.participantsPool.size()>=1){
                printf("Let's mine rest!!\n");
                node.checkDataUsing(true);
                bool yes = node.mineTheBlock();
                node.checkDataUsing(false);
                printf("Minded? %d\n", node.ownChain.getLength());
            }
            if(node.participantsPool.size() >=3){
                printf("Let's mine!!\n");
                node.checkDataUsing(true);
                bool yes = node.mineTheBlock();
                node.checkDataUsing(false);
                printf("Minded? %d\n", node.ownChain.getLength());
            }
            
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
        pthread_kill(pid[1], 0);
        string filename = "./log/log" + to_string(myOrder) + ".txt";
        ofstream file;
        file.open(filename);
        Blockchain tempchain = node.ownChain;
        Block tempBlock = tempchain.getBlock(0);
        int chainsize = tempchain.getLength();
        //printf("\n");
        //cout << "mynumber: " << node.getOwnPhoneNumber().c_str() << endl;
        file << "mynumber: " << node.getOwnPhoneNumber().c_str() << "\n";
        string ss;
        for(int h = 0 ; h < chainsize;h++){
            tempBlock = tempchain.getBlock(h);
            int size = tempBlock.getParticiSize();
            //cout << "Block hash is: " << tempBlock.CalculateHash() << endl;
            file << "Block hash is: " << tempBlock.CalculateHash() << "\n";
            ss += tempBlock.CalculateHash();
            for(int k = 0; k < size; k++){
                //cout << "Block "  << h << " has: " <<tempBlock.participantsVector[k] << endl;
                file << "Block "  << h << " has: " <<tempBlock.participantsVector[k] << "\n";
            }
            //printf("\n");
            file << "\n";
        }
        
        ss = sha256(ss);
        string winner;
        unsigned long criValue= getValue(ss);
        //cout << "criteria is: " << ss << "value: " << criValue << endl;
        file << "criteria is: " << ss << "\n";
        file << "value: " << criValue << "\n";
        
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
        //cout << "winner is " << winner << " Iam? " << iam << endl;
        file << "winner is " << winner << " Iam? " << iam << "\n";
        file << "winner has gap: " << minn << "\n";
        file << "my gap is: " << labs(criValue - getValue(node.getOwnPhoneNumber())) << "\n";
        if(iam){
            //cout << "The condition of winning is: " << tempchain.getWinnerCondition().c_str() << endl;
            file << "The condition of winning is: " << tempchain.getWinnerCondition().c_str() << "\n";
        }
        file.close();
        printf("Node close\n");
        return 0;
    }
    
}
