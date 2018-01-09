//
// Created by wurmel_a on 1/8/18.
//

#include <sys/select.h>
#include "ServerRegister.hh"
#include "../NetworkAbstract/IAcceptor.hh"
#include "../NetworkAbstract/LinuxAcceptor.hh"

rtp::ServerRegister::ServerRegister(unsigned short port) {
    _threadRunning = true;
    _acceptor = std::unique_ptr<NetworkAbstract::IAcceptor>(new NetworkAbstract::LinuxAcceptor(_clientNotifier, port));
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
        while (_acceptor->haveAwaitingClient()) {
            std::shared_ptr<RegisteredServer>   server(new RegisteredServer(_acceptor->acceptClient()));

            _serverList.push_back(server);
        }
        if (!handleServerIO()) {
            _threadRunning = false;
            return ;
        }
        auto iterator = _serverList.begin();

        while (iterator != _serverList.end()) {
            if (!(*iterator)->isOpen() || !(*iterator)->handleNewData()) {
                say("Loose connection with server{" + std::to_string((*iterator)->getId()) + "}");
                _serverList.erase(iterator);
            }
            else {
                ++iterator;
            }
        }
    }
}

bool    rtp::ServerRegister::handleServerIO() {
    fd_set  rdsk;
    fd_set  wrsk;
    timeval timeval;

    timeval.tv_usec = 200;
    timeval.tv_sec = 0;
    FD_ZERO(&rdsk);
    FD_ZERO(&wrsk);

    auto iterator = _serverList.begin();

    while (iterator != _serverList.end()) {
        FD_SET((*iterator)->getSocket()->getSocket(), &rdsk);
        if ((*iterator)->getSocket()->haveSomethingToWrite()) {
            FD_SET((*iterator)->getSocket()->getSocket(), &wrsk);
        }
        ++iterator;
    }
    int selectRet;

    selectRet = select(NetworkAbstract::IAcceptor::getMaxFd() + 1, &rdsk, &wrsk, NULL, &timeval);
    if (selectRet < 0) {
        std::cerr << "Error in select()" << std::endl;
        return false;
    }
    else if (selectRet) {
        iterator = _serverList.begin();

        while (iterator != _serverList.end()) {
            if (FD_ISSET((*iterator)->getSocket()->getSocket(), &rdsk)) {
                (*iterator)->getSocket()->read();
            }
            if (FD_ISSET((*iterator)->getSocket()->getSocket(), &wrsk)) {
                (*iterator)->getSocket()->flushWrite();
            }
            ++iterator;
        }
    }
    return true;
}

bool    rtp::ServerRegister::isRunning() const {
    return _threadRunning && _acceptor->isRunning();
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