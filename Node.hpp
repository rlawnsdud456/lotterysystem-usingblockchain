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
#include "sha256.hpp"
#include "Block.hpp"
#include "Blockchain.hpp"


class Node{
public:
    Node(time_t finish,std::string phone);
    time_t getFinishTime();
    void setFinishTime(time_t finish);
    time_t getCreatedNodeTime();
    std::string getOwnPhoneNumber();
    bool checkPhoneNumber(std::string phone);
    std::vector<std::string> participantsPool;
    std::vector<int> portPool;
    Blockchain ownChain;
private:
    time_t finishTime;
    time_t createdNodeTime;
    std::string ownPhoneNumber;
};


#endif /* Node_hpp */
