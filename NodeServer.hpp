//
//  NodeServer.hpp
//  
//
//  Created by KJY on 18/11/2018.
//

#ifndef NodeServer_hpp
#define NodeServer_hpp

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
#include "util.hpp"
class NodeServer{
public:
    NodeServer(int port,Node& node);
    int CompanyWaitForListen(time_t finish);
    int NodeWaitForListen();
    Node* ownNode;
};
#endif /* NodeServer_hpp */
