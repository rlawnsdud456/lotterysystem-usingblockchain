//
//  block.cpp
//  
//
//  Created by JunYeong on 31/10/2018.
//

#include "Block.hpp"
#include <iostream>
#include <string>
#include <tuple>
#include "sha256.hpp"

using namespace std;

Block::Block(uint32_t nIndexIn,vector<string>& partiVector,string previous):_nIndex(nIndexIn), sPrevHash(previous){
    _nNonce = -1;
    createdTime = time(NULL);
    addingTime = 0;
    
    int size = partiVector.size();
    for(int i = 0; i < size; i++){
        participantsVector.push_back(partiVector[i]);
    }
}

Block::Block(uint32_t nIndexIn,int64_t nonce,time_t addTime,time_t createTime,vector<string>& partiVector,string previous):_nIndex(nIndexIn), sPrevHash(previous),_nNonce(nonce),addingTime(addTime),createdTime(createTime){
    int size = partiVector.size();
    for(int i = 0; i < size; i++){
        participantsVector.push_back(partiVector[i]);
    }
}

std::string Block::getHash(){
    return CalculateHash();
}

std::string Block::getNumberValue(){
    return numberValue;
}

int64_t Block::getNonce(){
    return _nNonce;
}

std::string Block::CalculateHash(){
    std::string ss;
    int size = participantsVector.size();
    for(int i = 0; i < size; i ++){
        ss += participantsVector[i];
    }
    ss += to_string(_nIndex) + to_string(addingTime)+ to_string(createdTime) + to_string(_nNonce) + sPrevHash;
    //ss << _nIndex << _tTime << _sData << _nNonce << sPrevHash;
    
    return sha256(ss);
}

time_t Block::getCreatedTime(){
    return createdTime;
}

void Block::setCreatedTime(time_t temp){
    createdTime = temp;
}

int Block::getParticiSize(){
    return participantsVector.size();
}

void Block::setAddingTime(time_t temp){
    addingTime = temp;
}

time_t Block::getAddingTime(){
    return addingTime;
}

void Block::MineBlock(uint32_t nDifficulty,time_t finish){
    char cstr[nDifficulty + 1];
    for(uint32_t i = 0; i< nDifficulty; ++i){
        cstr[i] = '0';
    }
    cstr[nDifficulty] = '\0';
    
    do{
        _nNonce++;
        addingTime = time(NULL);
        _sHash = CalculateHash();

    }while(_sHash.substr(0,nDifficulty) != cstr);
}

bool Block::proofTheBlock(uint32_t nDifficulty,time_t finish){
    char cstr[nDifficulty + 1];
    for(uint32_t i = 0; i< nDifficulty; ++i){
        cstr[i] = '0';
    }
    cstr[nDifficulty] = '\0';
    _sHash = CalculateHash();
    if(_sHash.substr(0,nDifficulty) == cstr) return true;
    
    do{
        _nNonce++;
        addingTime = time(NULL);
        _sHash = CalculateHash();
    }while(_sHash.substr(0,nDifficulty) != cstr);
    return true;
}





