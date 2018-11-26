//
//  Node.cpp
//  
//
//  Created by Mac on 07/11/2018.
//

#include "Node.hpp"
#include <iostream>
#include <string>
#include <tuple>
#include <time.h>
#include "sha256.hpp"


Node::Node(time_t finish, std::string phone,int portNumber1,bool isCompany):finishTime(finish), ownPhoneNumber(phone),portNumber(portNumber1){
    createdNodeTime = time(NULL);
    if(!isCompany)participantsPool.push_back(getOwnPhoneNumber());
    for(int i = portNumber1; i < portNumber1 + 3; i++)
    {
        portQueue.push(i);
    }
}

time_t Node::getFinishTime(){
    return finishTime;
}

time_t Node::getCreatedNodeTime(){
    return createdNodeTime;
}

std::string Node::getOwnPhoneNumber(){
    return sha256(ownPhoneNumber);
}

bool Node::checkPhoneNumber(std::string phone){
    return phone == ownPhoneNumber;
}

void Node::setFinishTime(time_t finish){
    finishTime = finish;
}

bool Node::portPoolQueueHas(std::queue<int> temp,int it){
    
    int size = temp.size();
    if(it == portNumber) return true;
    for(int i = 0; i< size; i++){
        
        if(it == temp.front()) return true;
        else temp.pop();
    }
    return false;
}

bool Node::participantsPoolHas(std::string it){
    if(ownChain.checkParticipants(it)) return true;
    int size = participantsPool.size();
    for(int i = 0; i< size; i++){
        if(!participantsPool[i].compare(it)) return true;
    }
    return false;
}

void Node::checkDataUsing(bool toUse){
    if(toUse){
        while(dataUsing){}
        dataUsing = true;
        if(time(NULL)>finishTime) dataUsing = false;
    }
    else dataUsing = false;
}

bool Node::mineTheBlock(){
    Block newB = Block(ownChain.getLength(),participantsPool,ownChain.getLastBlock().CalculateHash());
    ownChain.MineBlock(newB,finishTime);
    int size = participantsPool.size();
    for(int i = 0; i < size; i++){
        participantsPool.erase(participantsPool.begin());
    }
}


