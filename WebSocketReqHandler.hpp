#ifndef WEBSOCKREQHAND_H
#define WEBSOCKREQHAND_H

#include <iostream>
#include "PocoInclude.hpp"

#define BUFSIZE 65536
#define MAXFRAMES 160

class WebSocketRequestHandler: public HTTPRequestHandler
{
    public: 
        void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);
};

#endif
