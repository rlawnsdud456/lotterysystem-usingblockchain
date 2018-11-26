//
//  Blockchain.cpp
//  
//
//  Created by Mac on 03/11/2018.
//

#include "Blockchain.hpp"

Blockchain:: Blockchain(genesisBlock genesis){
    _vChain.emplace_back(genesis);
    _nDifficulty = 3;
}

Blockchain:: Blockchain(){
    _nDifficulty = 3;
}

bool Blockchain::setGenesisBlock(genesisBlock genesis){
    if(_vChain.size() == 0 ){
        _vChain.emplace_back(genesis);
        return true;
    }
    return false;
}


void Blockchain::MineBlock(Block bNew,time_t finish){
    if(time(NULL) <= finish){
        bool proofed = bNew.proofTheBlock(_nDifficulty,finish);
        if(time(NULL) <= finish && proofed){
            _vChain.push_back(bNew);
            cout << "Block mined: " << getLastBlock().CalculateHash() <<  " " << getLastBlock().getAddingTime() << " " << time(NULL) << " " << getLastBlock().getNumberValue() <<  " " << getLastBlock().getNonce() << endl;
        }
        for(int i = 0; i < bNew.participantsVector.size();i++){
            participants_hash.emplace(getValue(bNew.participantsVector[i]),bNew.participantsVector[i]);
        }
    }
}

Block Blockchain::getLastBlock() const{
    return _vChain.back();
}
Block Blockchain::getBlock(int i){
    return _vChain[i];
}
uint32_t Blockchain::getDifficulty(){
    return _nDifficulty;
}


int Blockchain::getLength(){
    return _vChain.size();
}

bool Blockchain::addBlock(Block nBlock){
    if(nBlock.getNonce()<0) return false;
    if(nBlock.getAddingTime() < 0 && nBlock.getAddingTime() < getBlock(0).getAddingTime()) return false;
    if(nBlock.getCreatedTime() < 0 && nBlock.getCreatedTime() < getBlock(0).getCreatedTime()) return false;
    
    int size = nBlock.participantsVector.size();
    for(int i = 0 ; i < size; i++){
        if(checkParticipants(nBlock.participantsVector[i])) return false;
    }
    
    if(!checkValidBlock(nBlock,_nDifficulty)) return false;
    
    for(int i = 0; i < nBlock.participantsVector.size();i++){
            participants_hash.emplace(getValue(nBlock.participantsVector[i]),nBlock.participantsVector[i]);
    }
    _vChain.push_back(nBlock);
    return true;
}

bool Blockchain::checkValidBlock(Block nBlock, int nDifficulty){
    char cstr[nDifficulty + 1];
    for(uint32_t i = 0; i< nDifficulty; ++i){
        cstr[i] = '0';
    }
    cstr[nDifficulty] = '\0';
    string _sHash = nBlock.CalculateHash();
    if(_sHash.substr(0,nDifficulty) == cstr) return true;
    else return false;
}

bool Blockchain::checkParticipants(std::string it){
    std::unordered_map<unsigned long, std::string>::const_iterator got = participants_hash.find(getValue(it));
    if(got != participants_hash.end()) {
        printf("I found %ld : %s\n",got->first,got->second.c_str());
        printf("I found %ld : %s\n",getValue(it),it.c_str());
        return true;
    }
    else return false;
}

void Blockchain::copyChain(Blockchain newChain){
    participants_hash.erase(participants_hash.begin(),participants_hash.end());
    int size = _vChain.size()-1;
    for(int i = 0; i<size;i++){
        _vChain.erase(_vChain.begin()+1);
    }
    size = newChain.getLength();
    for(int i = 1; i < size; i++){
        Block tempBlock = newChain.getBlock(i);
        Block nBlock = Block(i,tempBlock.getNonce(),tempBlock.getAddingTime(),tempBlock.getCreatedTime(),tempBlock.participantsVector,tempBlock.sPrevHash);
        _vChain.push_back(nBlock);
    }
    
    participants_hash = newChain.participants_hash;
}

