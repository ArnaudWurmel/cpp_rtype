#include <memory>
#include "ArgumentLoader/ArgumentLoader.hh"
#include "ArgumentLoader/Configure.hh"
#include "ServerRegister/ServerRegister.hh"
#include "RTypeManager/RTypeManager.hh"
#include "ClientRegister/ClientRegister.hh"

static bool runServerRegister(rtp::ArgumentLoader const& argumentLoader,
                              std::shared_ptr<rtp::ServerRegister>& serverRegister,
                              std::shared_ptr<rtp::ClientRegister>& clientRegister) {
    unsigned short serverPort = 8080;
    unsigned short clientPort = 8888;

    if (argumentLoader.haveArg("--server-port")) {
        serverPort = std::stoi(argumentLoader.getArg("--server-port").value);
    }
    if (argumentLoader.haveArg("--client-port")) {
        clientPort = std::stoi(argumentLoader.getArg("--client-port").value);
    }
    serverRegister = std::make_shared<rtp::ServerRegister>(serverPort);
    clientRegister = std::make_shared<rtp::ClientRegister>(clientPort, serverRegister);
    return true;
}

int main(int ac, char **av) {
    rtp::ArgumentLoader argumentLoader;
    std::shared_ptr<rtp::ServerRegister>    serverRegister;
    std::shared_ptr<rtp::ClientRegister>    clientRegister;

    std::srand(time(NULL));
    argumentLoader.enableArgument("-d", false);
    argumentLoader.enableArgument("--server-port", true);
    argumentLoader.enableArgument("--client-port", true);
    if (!argumentLoader.loadArguments(ac, av)) {
        return 1;
    }
    rtp::Configure::configure(argumentLoader.get());
    try {
        if (!runServerRegister(argumentLoader, serverRegister, clientRegister)) {
            return 1;
        }
        rtp::RTypeManager   rTypeManager(serverRegister, clientRegister);

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