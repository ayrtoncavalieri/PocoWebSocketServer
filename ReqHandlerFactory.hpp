#ifndef REQHANDFACT_H
#define REQHANDFACT_H

#include <iostream>
#include "PocoInclude.hpp"
#include "PageReqHandler.hpp"
#include "WebSocketReqHandler.hpp"

class RequestHandlerFactory: public HTTPRequestHandlerFactory
{
    public:
        HTTPRequestHandler *createRequestHandler(const HTTPServerRequest &request);
};

#endif
