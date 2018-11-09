//
//  Blockchain.hpp
//  
//
//  Created by Mac on 03/11/2018.
//

#ifndef Blockchain_hpp
#define Blockchain_hpp

#include <stdio.h>
#include <cstdint>
#include <vector>
#include <time.h>
#include "Block.hpp"
#include "genesisBlock.hpp"

using namespace std;

class Blockchain{
public:
    Blockchain();
    
    void AddBlock(Block bNew,time_t finish);
    time_t getCreatedTime();
    int getLength();
    Block getBlock(int i);
    Block getLastBlock() const;
    genesisBlock getGenesisBlock();

private:
    uint32_t _nDifficulty;
    vector<Block> _vChain;
    time_t createdTime;
    genesisBlock genesis;
};
#endif /* Blockchain_hpp */
