//
// Created by wurmel_a on 1/8/18.
//

#include "ServerRegister.hh"

rtp::ServerRegister::ServerRegister(unsigned short port) {
    _threadRunning = true;
    _acceptor = std::unique_ptr<NetworkAbstract::IAcceptor>(new NetworkAbstract::BoostAcceptor(port, _clientNotifier));
    _thread = std::unique_ptr<std::thread>(new std::thread(&rtp::ServerRegister::serverLooping, this));
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

        _clientNotifier.wait_for(lck, std::chrono::milliseconds(200));
        while (_acceptor->haveAwaitingClient()) {
            std::shared_ptr<RegisteredServer>   server(new RegisteredServer(_acceptor->acceptClient()));

            _serverList.push_back(server);
        }

        auto iterator = _serverList.begin();

        while (iterator != _serverList.end()) {
            if (!(*iterator)->isOpen() || !(*iterator)->handleNewData()) {
                _serverList.erase(iterator);
            }
            else {
                ++iterator;
            }
        }
    }
}

bool    rtp::ServerRegister::isRunning() const {
    return _threadRunning;
}

void    rtp::ServerRegister::stop() {
    if (_threadRunning) {
        _threadRunning = false;
        _clientNotifier.notify_one();
        _thread->join();
        _thread.reset();
    }
}

std::vector<std::shared_ptr<rtp::RegisteredServer> > const& rtp::ServerRegister::getServer() const {
    return _serverList;
}

rtp::ServerRegister::~ServerRegister() {
    stop();
}