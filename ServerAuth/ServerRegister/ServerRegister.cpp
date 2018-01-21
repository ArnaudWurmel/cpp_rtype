//
// Created by wurmel_a on 1/8/18.
//

#include "ServerRegister.hh"
#include "../NetworkAbstract/IAcceptor.hh"
#include "../NetworkAbstract/BoostAcceptor.hh"

rtp::ServerRegister::ServerRegister(unsigned short port) : BaseServer(port) {
    run();
}

void    rtp::ServerRegister::serverLooping() {
    try {
        _acceptor->run();
    }
    catch (std::exception& e) {
        say(std::string("Exception thread running : ") + e.what());
        _threadRunning = false;
    }
    while (_threadRunning) {
        std::unique_lock<std::mutex>    lck(_mClient);
        while (_acceptor->haveAwaitingClient()) {
            std::shared_ptr<RegisteredServer>   server(new RegisteredServer(_acceptor->acceptClient()));

            lockData();
            _serverList.push_back(server);
            unlockData();
        }
        lockData();
        auto iterator = _serverList.begin();

        while (iterator != _serverList.end()) {
            if (!(*iterator)->isOpen() || !(*iterator)->handleNewData()) {
                say("Loose connection with server{" + std::to_string((*iterator)->getId()) + "}");
                iterator = _serverList.erase(iterator);
            }
            else {
                ++iterator;
            }
        }
        unlockData();
    }
}

void    rtp::ServerRegister::lockData() {
    _dataSafer.lock();
}

void    rtp::ServerRegister::unlockData() {
    _dataSafer.unlock();
}

std::vector<std::shared_ptr<rtp::RegisteredServer> > const& rtp::ServerRegister::getServer() const {
    return _serverList;
}

rtp::ServerRegister::~ServerRegister() = default;