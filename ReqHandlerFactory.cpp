#include "ReqHandlerFactory.hpp"

HTTPRequestHandler* RequestHandlerFactory::createRequestHandler(const HTTPServerRequest &request)
{
    Application& app = Application::instance();
    app.logger().information("Request from "
        + request.clientAddress().toString()
        + ": "
        + request.getMethod()
        + " "
        + request.getURI()
        + " "
        + request.getVersion());

    for (HTTPServerRequest::ConstIterator it = request.begin(); it != request.end(); ++it)
    {
        app.logger().information(it->first + ": " + it->second);
    }

    if(request.find("Upgrade") != request.end() && Poco::icompare(request["Upgrade"], "websocket") == 0)
        return new WebSocketRequestHandler;
    else
        return new PageRequestHandler;
}
