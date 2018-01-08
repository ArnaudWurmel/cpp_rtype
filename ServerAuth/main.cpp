#include <memory>
#include "ArgumentLoader/ArgumentLoader.hh"
#include "ArgumentLoader/Configure.hh"
#include "ServerRegister/ServerRegister.hh"
#include "RTypeManager/RTypeManager.hh"

static bool runServerRegister(rtp::ArgumentLoader const& argumentLoader, std::shared_ptr<rtp::ServerRegister>& serverRegiter) {
    unsigned short serverPort = 8080;

    if (argumentLoader.haveArg("--server-port")) {
        serverPort = std::stoi(argumentLoader.getArg("--port").value);
    }
    serverRegiter = std::make_shared<rtp::ServerRegister>(serverPort);
    return true;
}

int main(int ac, char **av) {
    rtp::ArgumentLoader argumentLoader;
    std::shared_ptr<rtp::ServerRegister>    serverRegister;

    if (!argumentLoader.loadArguments(ac, av)) {
        return 1;
    }
    rtp::Configure::configure(argumentLoader.get());
    try {
        if (!runServerRegister(argumentLoader, serverRegister)) {
            return 1;
        }
        rtp::RTypeManager   rTypeManager(serverRegister);

        if (rTypeManager.loop()) {
            std::cout << "Exited" << std::endl;
        }
        else {
            std::cout << "Error happened" << std::endl;
            return 1;
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception in runServer() " << e.what() << std::endl;
        return 1;
    }
    return 0;
}