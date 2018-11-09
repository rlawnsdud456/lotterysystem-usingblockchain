//
//  NodeNetWork.hpp
//  
//
//  Created by Mac on 08/11/2018.
//

#ifndef NodeNetWork_hpp
#define NodeNetWork_hpp

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <time.h>
#include "Node.hpp"
class NodeNetWork{
public:
    NodeNetWork(int port,Node& node);
    bool shakeHandToNode(int receiverPort,int mode);
    int CompanyWaitForListen(time_t finish);
    void requestPortPool(int port, char *serverIP);
    Node* ownNode;
    bool initializeFromCompany(int receiverPort);
    bool nodeRequestNode(int receiverPort);
    void nodeWaitForListen();
};

#endif /* NodeNetWork_hpp */
