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


Node::Node(time_t finish, std::string phone):finishTime(finish), ownPhoneNumber(phone){
    createdNodeTime = time(NULL);
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

