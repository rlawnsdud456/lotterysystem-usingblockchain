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
#include "NodeNetWork.hpp"
#include "genesisBlock.hpp"
#define comPanyNodePort 5003
using namespace std;

time_t finishtime;
int portNumber;
unsigned long getValue(string s){
    unsigned long result = 0;
    int k = 16 * 16 * 16 * 16 * 16;
    for(int i = 0 ; i < 7; i++){
        char temp = s[i];
        
        switch (temp) {
            case 'a':
                result += 10 * k;
                break;
            case 'b':
                result +=11 * k;
                break;
            case 'c':
                result +=12 * k;
                break;
            case 'd':
                result +=13 * k;
                break;
            case 'e':
                result +=14 * k;
                break;
            case 'f':
                result +=15 * k;
                break;
            default:
                int tempi = (int)temp - 48;
                result += tempi * k;
                break;
        }
         k /= 16;
    }
    return result;
}
int main(int argc, char *argv[]){
    int temp;
    //cout << "for companyNode type 0 else anything" << endl;
    //cin >> temp;
    
    //NodeNetWork network = NodeNetWork(0,&Node(finishtime,"123"));
    finishtime = time(NULL) + 50;
    if(atoi(argv[1]) == 0){
        
        portNumber = comPanyNodePort;
        comPanyNode tempCompany = comPanyNode(finishtime);
        NodeNetWork network = NodeNetWork(portNumber,tempCompany);
        
        while(time(NULL) <= finishtime){
            network.CompanyWaitForListen(finishtime);
            for(int i = 0;i<tempCompany.portPool.size();i++){
                cout << tempCompany.portPool[i] << endl;
            }
            
        }
        //cout << tempCompany.getChain().getGenesisBlock().getCondition() << endl;
    }
    else{
        portNumber = atoi(argv[2]);
        Node node = Node(finishtime,to_string(portNumber+1));
        node.portPool.emplace_back(comPanyNodePort);
        //Node* nodepointer = &node;
        NodeNetWork network = NodeNetWork(portNumber,node);
        printf("Redo%d\n",node.participantsPool.size());

        network.initializeFromCompany(comPanyNodePort);
       // printf("Redo%s\n",node.participantsPool[0].c_str());
       
       // while(time(NULL)<= finishtime){
            printf("Redo%d\n",node.participantsPool.size());
            if(portNumber%2==0)network.nodeWaitForListen();
            else {
                network.nodeRequestNode(portNumber-1);
                cout << "Let's Look at initialization" << endl;
                for(int i = 0;i<node.portPool.size();i++){
                    cout << node.portPool[i] << endl;
                }
                for(int i = 0; i<node.participantsPool.size(); i++){
                    cout << node.participantsPool[i] << endl;
                }
                printf("end\n");
            }
            printf("##\n");

        //}
       
    }
    
    
}
/*int main(){
    Blockchain bChain = Blockchain();
    finishtime = bChain.getCreatedTime() + 6;
    int k =1;
    cout << time(NULL) << " " <<finishtime<<endl;
    while(time(NULL)<=finishtime){
        //cout << "Mining block " << k <<"..." <<endl;
        //bChain.AddBlock(Block(k, "Block"+to_string(k) + "Data"),finishtime);
        if(k==1){
            string phone = to_string(rand() % 32767 + k);
            nodePool.emplace_back(Node(finishtime,phone,5000+k));
            participantsPool.emplace_back(nodePool.back().getOwnPhoneNumber());
            k++;
        }
        else if(k==2){
            string phone = to_string(rand() % 32767 + k);
            nodePool.emplace_back(Node(finishtime,phone,5000+k));
            participantsPool.emplace_back(nodePool.back().getOwnPhoneNumber());
            k++;
            
        }
    }
    
    string ss;
    for(int i = 0; i< bChain.getLength(); i++){
        ss = ss + bChain.getBlock(i).getHash();
    }
    string criteria = sha256(ss);
    cout << criteria << " " << getValue(criteria) << endl;
    
    unsigned long l;
    int winner = -1;
    unsigned int winner_difference = 999999999;
    unsigned long criteriaValue = getValue(criteria);
    for(int i =0; i< bChain.getLength(); i++){
        l = getValue(bChain.getBlock(i).getNumberValue());
        cout <<bChain.getBlock(i).getNumberValue() << " " << l<<endl;
        long temp = l - criteriaValue;
        if(labs(temp) <= winner_difference){
            if(labs(temp) == winner_difference){
                cout << "same" << i  << endl;
            }
            else {
                winner = i;
                cout << winner << endl;
            }
            
            winner_difference = labs(temp);
        }
    }
    return 0;
}*/
