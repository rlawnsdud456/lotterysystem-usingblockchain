//
//  Node.hpp
//  
//
//  Created by Mac on 07/11/2018.
//

#ifndef Node_hpp
#define Node_hpp

#include <stdio.h>
#include <cstdint>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <queue>
#include "sha256.hpp"
#include "Block.hpp"
#include "Blockchain.hpp"


class Node{
public:
    Node(time_t finish,std::string phone,int portNumber1,bool isCompany);
    time_t getFinishTime();
    void setFinishTime(time_t finish);
    time_t getCreatedNodeTime();
    std::string getOwnPhoneNumber();
    bool checkPhoneNumber(std::string phone);
    void checkDataUsing(bool toUse);
    bool mineTheBlock();
    
    
    std::vector<std::string> participantsPool;
    std::vector<int> portPool;
    Blockchain ownChain;
    std::queue<int>portQueue;
    std::queue<int>portPoolQueue;
    int portNumber;
    int mode = -1;
    bool lock = false;
    bool portPoolQueueHas(std::queue<int> temp,int it);
    bool participantsPoolHas(std::string it);
    void checkParticipants();
    bool dataUsing;
private:
    time_t finishTime;
    time_t createdNodeTime;
    std::string ownPhoneNumber;
};


#endif /* Node_hpp */
