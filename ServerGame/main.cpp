#include <iostream>
#include <cstring>
#include "ArgumentLoader/Configure.hh"
#include "GameServer/GameServer.hh"

static bool runGameServer(rtp::ArgumentLoader const& argumentLoader, std::shared_ptr<rtp::GameServer>& gameServer) {
    unsigned short  serverPort = 8888;
    unsigned short  authPort = 8080;
    std::string authHost = "";

    if (!argumentLoader.haveArg("--auth-host")) {
        std::cerr << "Can't run server without auth host" << std::endl;
        return false;
    }
    authHost = argumentLoader.getArg("--auth-host").value;
    if (argumentLoader.haveArg("--auth-port")) {
        authPort = std::stoi(argumentLoader.getArg("--auth-port").value);
    }
    if (argumentLoader.haveArg("--server-port")) {
        serverPort = std::stoi(argumentLoader.getArg("--server-port").value);
    }
    gameServer = std::shared_ptr<rtp::GameServer>(new rtp::GameServer(serverPort));
    if (!gameServer->connectToAuthServer(authHost, authPort)) {
        std::cerr << "Can't connect to AuthServer" << std::endl;
        return false;
    }
    if (!gameServer->registerServer()) {
        std::cerr << "Can't register the server" << std::endl;
        return false;
    }
    return true;
}

int main(int ac, char **av) {
    rtp::ArgumentLoader argumentLoader;
    std::shared_ptr<rtp::GameServer>    gameServer;

    argumentLoader.enableArgument("--auth-port", true);
    argumentLoader.enableArgument("--auth-host", true);
    argumentLoader.enableArgument("--server-port", true);
    if (!argumentLoader.loadArguments(ac, av)) {
        return 1;
    }
    rtp::Configure::configure(argumentLoader.get());
    try {
        if (!runGameServer(argumentLoader, gameServer)) {
            return 1;
        }
        gameServer->serverLoop();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}