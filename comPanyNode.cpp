//
//  comPanyNode.cpp
//  
//
//  Created by Mac on 08/11/2018.
//

#include "comPanyNode.hpp"

#include <iostream>
#include <string>
#include <tuple>
#include <time.h>
#include "sha256.hpp"

using namespace std;

comPanyNode::comPanyNode(time_t finish1):Node(finish1," "){
}

Blockchain comPanyNode::getChain(){
    return thisChain;
}
