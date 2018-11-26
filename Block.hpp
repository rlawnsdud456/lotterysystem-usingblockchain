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
#include <vector>
class Block{
public:
    Block(uint32_t nIndexIn,std::vector<std::string>& partiVector,std::string previous);
    Block(uint32_t nIndexIn,int64_t nonce,time_t addTime,time_t createTime,std::vector<std::string>& partiVector,std::string previous);
    std::string sPrevHash;
    std::vector<std::string> participantsVector;
    
    std::string getHash();
    std::string getNumberValue();
    
    void MineBlock(uint32_t nDifficulty,time_t finish);
    bool proofTheBlock(uint32_t nDifficulty,time_t finish);
    std::string CalculateHash();
    
    int64_t getNonce();
    time_t getCreatedTime();
    void setCreatedTime(time_t temp);
    void setAddingTime(time_t temp);
    time_t getAddingTime();
    int getParticiSize();
    
private:
    uint32_t _nIndex;
    int64_t _nNonce;
    std::string _sHash;
    time_t addingTime;
    time_t createdTime;
    std::string numberValue;
};



#endif /* block_hpp */


