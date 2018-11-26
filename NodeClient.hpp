//
//  NodeClient.hpp
//  
//
//  Created by KJY on 18/11/2018.
//

#ifndef NodeClient_hpp
#define NodeClient_hpp

#include <stdio.h>
#include "Node.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <netdb.h>
#include "util.hpp"
class NodeClient{
public:
    NodeClient(int portNumber,int receiverPort,Node& node,bool company);
    Node* ownNode;
    int connectToPort(int portNumber,int receiverPort, char *serverIp);
    
};
#endif /* NodeClient_hpp */
