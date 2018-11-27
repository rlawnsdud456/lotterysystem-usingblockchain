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
#include <unordered_map>
#include "genesisBlock.hpp"
#include "util.hpp"

using namespace std;

class Blockchain{
public:
    Blockchain(genesisBlock genesis);
    Blockchain();
    
    unordered_map<unsigned long, string> participants_hash;
    time_t getCreatedTime();
    vector<Block> _vChain;

    bool setGenesisBlock(genesisBlock genesis);
    void MineBlock(Block bNew,time_t finish);
    int getLength();
    bool addBlock(Block nBlock);
    bool checkParticipants(std::string it);
    bool checkValidBlock(Block nBlock, int nDifficulty);
    void copyChain(Blockchain newChain);
    string getWinnerCondition();
    uint32_t getDifficulty();
    Block getBlock(int i);
    Block getLastBlock() const;
    genesisBlock getGenesisBlock();
private:
    uint32_t _nDifficulty;
    string condition;
    
};
#endif /* Blockchain_hpp */
