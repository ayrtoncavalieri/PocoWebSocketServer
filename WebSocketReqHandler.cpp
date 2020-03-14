#include "WebSocketReqHandler.hpp"

void WebSocketRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
Application& app = Application::instance();
    try
    {
        Timespan timeOut(0, 30000000);
        WebSocket ws(request, response);
        ws.setReceiveTimeout(timeOut);
        ws.setSendTimeout(timeOut);
        ws.getBlocking() == true ? app.logger().information("blocking") : app.logger().information("~blocking");
        app.logger().information("WebSocket connection established.");
        char buffer[BUFSIZE];
        int flags;
        int n;
        do
        {
            app.logger().information("Receiving");
            n = ws.receiveFrame(buffer, BUFSIZE, flags);
            app.logger().information(Poco::format("Frame received (length=%d, flags=0x%x).", n, unsigned(flags)));
            if((flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_PING){
                app.logger().information(Poco::format("flags & WebSocket::FRAME_OP_BITMASK (flags=0x%x).", unsigned(flags & WebSocket::FRAME_OP_PING)));
                app.logger().information("A PING!");
                flags = (WebSocket::FRAME_FLAG_FIN | WebSocket::FRAME_OP_PONG);
                app.logger().information(Poco::format("flags = 0x%x", unsigned(flags)));
                n = 1;
                ws.sendFrame(buffer, 0, flags);
            }else{
                if(buffer[0] == 0x04){
                    ws.shutdown();
                    app.logger().information("Connection shutdown.");
                }else if(flags != 0x88){
                    int sent;
                    sent = ws.sendFrame(buffer, n, flags);
                    app.logger().information(Poco::format("Bytes sent: %d", sent));
                }
            }
        }while (n > 0 && (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);
        app.logger().information(Poco::format("(flags & WebSocket::FRAME_OP_BITMASK) = flags=0x%x", unsigned(flags)));
        app.logger().information("WebSocket connection closed.");
        Poco::UTF16String a;
    }
    catch (WebSocketException& exc)
    {
        app.logger().log(exc);
        switch (exc.code())
        {
        case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
            response.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
            // fallthrough
        case WebSocket::WS_ERR_NO_HANDSHAKE:
        case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
        case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
            response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
            response.setContentLength(0);
            response.send();
            break;
        }
    }catch (TimeoutException& e){
        app.logger().log(e);
        app.logger().information("Timeout exception.");
    }catch(NetException &e){
        app.logger().log(e);
        app.logger().information("Network exception.");
    }
}
