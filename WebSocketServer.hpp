#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <vector>
#include "PocoInclude.hpp"
#include "ReqHandlerFactory.hpp"

class WebSocketServer: public Poco::Util::ServerApplication
{
    public:
        WebSocketServer();
        ~WebSocketServer();

    protected:
       void initialize(Application &self);
       void uninitialize();
       void defineOptions(OptionSet &options);
       void handleOption(const std::string &name, const std::string &value);
       void displayHelp();
       int main(const std::vector<std::string>& args);

    private:
        bool _helpRequested;
};

/*
class Server{
    public:
        Server(unsigned short port);
        virtual ~Server();

        bool supportIPv6();

        void startSrv();

    private:
        unsigned short _port;
        SecureServerSocket svs;
};
*/
#endif 
//SERVER_H
