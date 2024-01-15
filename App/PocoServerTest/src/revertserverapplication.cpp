#include "revertserverapplication.hpp"
#include <Poco/Net/TCPServer.h>

using namespace Poco::Net;

class RevertConnection : public TCPServerConnection {
public:
    RevertConnection(const StreamSocket &socket) : TCPServerConnection(socket) {

    }

    void run () {
        StreamSocket& ss = socket();
        ss.sendBytes(welcomeStr.c_str(), welcomeStr.length());

        char buffer[256];
        std::string str;
        int n = ss.receiveBytes(buffer, sizeof(buffer));

        while ( n > 0 ) {
            for ( int i = 0; i < n; ++i ) {
                if (buffer[i] == '\n') {
                    std::reverse(str.begin(), str.end());
                    str.push_back('\n');
                    ss.sendBytes(str.c_str(), str.length());
                    str.clear();
                } else {
                    str.push_back(buffer[i]);
                }
            }
            n = ss.receiveBytes(buffer, sizeof(buffer));
        }
    }

private:
        static const std::string welcomeStr;
};

const std::string RevertConnection::welcomeStr = "Welcome to POCO TCP server. Enter you string:\n";

RevertServerApplication::RevertServerApplication() : ServerApplication()
{
}

int RevertServerApplication::main(const std::vector<std::string> &args)
{
    (void)args;
    TCPServer server( new TCPServerConnectionFactoryImpl<RevertConnection>(), ServerSocket(PORT));
    server.start();

    waitForTerminationRequest();

    return Application::EXIT_OK;
}
