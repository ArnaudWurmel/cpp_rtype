//
// Created by Arnaud WURMEL on 10/01/2018.
//

#include "ClientRegister.hh"

rtp::ClientRegister::ClientRegister(unsigned short port, std::shared_ptr<IServerRegister> iServerRegister) : BaseServer(port) {
    _iServerRegister = iServerRegister;
}

void    rtp::ClientRegister::serverLooping() {
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
            std::shared_ptr<RegisteredClient>   client(new RegisteredClient(_acceptor->acceptClient(), *this));

            _clientList.push_back(client);
        }
        auto iterator = _clientList.begin();

        while (iterator != _clientList.end()) {
            if (!(*iterator)->isOpen() || !(*iterator)->handleNewData()) {
                say("Loose connection with client{" + std::to_string((*iterator)->getId()) + "}");
                _clientList.erase(iterator);
            }
            else {
                ++iterator;
            }
        }
    }
}

rtp::ClientRegister::~ClientRegister() = default;