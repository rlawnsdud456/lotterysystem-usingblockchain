//
//  block.hpp
//  
//
//  Created by JunYeong on 31/10/2018.
//

#ifndef Block_hpp
#define Block_hpp

#include <cstdint>
#include <iostream>
#include <string>
#include <time.h>

class Block{
public:
    std::string sPrevHash;
    
    Block(uint32_t nIndexIn, const std::string &sDataIn);
    
    std::string getHash();
    std::string getNumberValue();
    
    void MineBlock(uint32_t nDifficulty,time_t finish);
    std::string CalculateHash();
    
    int64_t getNonce();
    time_t getTime();

    
private:
    uint32_t _nIndex;
    int64_t _nNonce;
    std::string _sData;
    std::string _sHash;
    time_t _tTime;
    std::string numberValue;

};



#endif /* block_hpp */


