//
//  Blockchain.cpp
//  
//
//  Created by Mac on 03/11/2018.
//

#include "Blockchain.hpp"

Blockchain:: Blockchain(){
    _vChain.emplace_back(genesis);
    _nDifficulty = 3;
    createdTime = time(NULL);
}

genesisBlock Blockchain::getGenesisBlock(){
    return genesis;
}

void Blockchain::AddBlock(Block bNew,time_t finish){
    if(time(NULL)<=finish){
        bNew.sPrevHash = getLastBlock().getHash();
        bNew.MineBlock(_nDifficulty,finish);
        if(time(NULL)<=finish){
            _vChain.push_back(bNew);
            cout << "Block mined: " << getLastBlock().CalculateHash() <<  " " << getLastBlock().getTime() << " " << getLastBlock().getNumberValue() <<  " " << getLastBlock().getNonce() << endl;
        }
    }
}

Block Blockchain::getLastBlock() const{
    return _vChain.back();
}
Block Blockchain::getBlock(int i){
    return _vChain[i];
}

time_t Blockchain::getCreatedTime(){
    return createdTime;
}

int Blockchain::getLength(){
    return _vChain.size();
}
