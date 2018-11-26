//
//  comPanyNode.hpp
//  
//
//  Created by Mac on 08/11/2018.
//

#ifndef comPanyNode_hpp
#define comPanyNode_hpp

#include <stdio.h>
#include <cstdint>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include "sha256.hpp"
#include "Block.hpp"
#include "Node.hpp"
#include "Blockchain.hpp"


class comPanyNode: public Node{
public:
    comPanyNode(time_t finish1,int portNumber,bool isCompany);
};

#endif /* comPanyNode_hpp */
