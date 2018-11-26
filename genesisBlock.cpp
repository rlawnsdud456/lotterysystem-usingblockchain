//
//  genesisBlock.cpp
//  
//
//  Created by Mac on 08/11/2018.
//

#include "genesisBlock.hpp"


using namespace std;

genesisBlock::genesisBlock(vector<string>& temp):Block(0,temp,""){
}

std::string genesisBlock::getCondition(){
    return conditionOfWinning;
}

