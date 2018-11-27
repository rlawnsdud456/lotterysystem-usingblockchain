//
//  genesisBlock.hpp
//  
//
//  Created by Mac on 08/11/2018.
//

#ifndef genesisBlock_hpp
#define genesisBlock_hpp

#define conditionOfWinning_const "The 1st gets $1000"

#include <stdio.h>
#include "Block.hpp"

class genesisBlock: public Block{
private:
    const std::string conditionOfWinning = "The 1st gets $1000";
    
public:
    genesisBlock(std::vector<std::string>& temp);
    std::string getCondition();
    void setCreatedTime(time_t temp){
        Block::setCreatedTime(temp);
    }
};

#endif /* genesisBlock_hpp */
