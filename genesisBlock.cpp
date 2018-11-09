//
//  genesisBlock.cpp
//  
//
//  Created by Mac on 08/11/2018.
//

#include "genesisBlock.hpp"


using namespace std;

genesisBlock::genesisBlock():Block(0,""){
}

std::string genesisBlock::getCondition(){
    return conditionOfWinning;
}
