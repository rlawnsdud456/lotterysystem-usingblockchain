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

Block::Block(uint32_t nIndexIn, const string &sDataIn):_nIndex(nIndexIn), _sData(sDataIn){
    _nNonce = -1;
    numberValue = sha256(sDataIn);
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
    ss = to_string(_nIndex) + to_string(_tTime) + _sData + to_string(_nNonce) + sPrevHash;
    //ss << _nIndex << _tTime << _sData << _nNonce << sPrevHash;
    
    return sha256(ss);
}

time_t Block::getTime(){
    return _tTime;
}
void Block::MineBlock(uint32_t nDifficulty,time_t finish){
    char cstr[nDifficulty + 1];
    for(uint32_t i = 0; i< nDifficulty; ++i){
        cstr[i] = '0';
    }
    cstr[nDifficulty] = '\0';
    
    do{
        _nNonce++;
        _tTime = time(NULL);
        _sHash = CalculateHash();

    }while(_sHash.substr(0,nDifficulty) != cstr);
}




