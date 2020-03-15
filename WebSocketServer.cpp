#include "WebSocketServer.hpp"

WebSocketServer::WebSocketServer() :_helpRequested(false)
{

}

WebSocketServer::~WebSocketServer()
{

}

void WebSocketServer::initialize(Application &self)
{
    loadConfiguration(); // load default configuration files, if present
    ServerApplication::initialize(self);
}

void WebSocketServer::uninitialize()
{
    ServerApplication::uninitialize();
}

void WebSocketServer::defineOptions(OptionSet &options)
{
    ServerApplication::defineOptions(options);

    options.addOption(
        Option("help", "h", "display help information on command line arguments")
            .required(false)
            .repeatable(false));
}

void WebSocketServer::handleOption(const std::string &name, const std::string &value)
{
    ServerApplication::handleOption(name, value);

    if(name == "help")
        _helpRequested = true;
}

void WebSocketServer::displayHelp()
{
    HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    helpFormatter.setHeader("A sample HTTP server supporting the WebSocket protocol.");
    helpFormatter.format(std::cout);
}

int WebSocketServer::main(const std::vector<std::string>& args)
{
    if (_helpRequested)
    {
        displayHelp();
    }
    else
    {
        //Uncomment the next block for secure socket
        /*Poco::Net::initializeSSL();
        Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> pCert =
            new Poco::Net::ConsoleCertificateHandler(false);
        Poco::Net::Context::Ptr pContext = new Poco::Net::Context(
                Poco::Net::Context::SERVER_USE,
                "insert_key_file_path",
                "insert_cert_file_path",
                "",
                Poco::Net::Context::VERIFY_RELAXED,
                9,
                false,
                "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
        Poco::Net::SSLManager::instance().initializeClient(0, pCert, pContext);*/

        // get parameters from configuration file
        unsigned short port = (unsigned short) config().getInt("WebSocketServer.port", 43880);
        
        ThreadPool pool(128, 256, 60, POCO_THREAD_STACK_SIZE);

        // set-up a server socket
        ServerSocket svs(port);
        //SecureServerSocket svs(port, 64, pContext); // Uncomment for secure socket.
        svs.supportsIPv6() == true ? printf("IPv6\n") : printf("IPv4\n");
        // set-up a HTTPServer instance
        HTTPServer srv(new RequestHandlerFactory, pool, svs, new HTTPServerParams);
        // start the HTTPServer
        srv.start();
        // wait for CTRL-C or kill
        waitForTerminationRequest();
        // Stop the HTTPServer
        srv.stop();
    }
    return Application::EXIT_OK;
}
