#ifndef PAGEREQHANDLER_H
#define PAGEREQHANDLER_H

#include <iostream>
#include "PocoInclude.hpp"

class PageRequestHandler: public HTTPRequestHandler
{
    public:
        void handleRequest(HTTPServerRequest &request, HTTPServerResponse &response);
        
};

#endif
