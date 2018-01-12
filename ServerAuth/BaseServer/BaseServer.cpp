//
// Created by Arnaud WURMEL on 10/01/2018.
//

#include "BaseServer.hh"
#include "../NetworkAbstract/BoostAcceptor.hh"

rtp::BaseServer::BaseServer(unsigned short port) {
    _threadRunning = true;
    _acceptor = std::unique_ptr<NetworkAbstract::IAcceptor>(new NetworkAbstract::BoostAcceptor( port, _clientNotifier));
    _thread = std::unique_ptr<std::thread>(new std::thread(&rtp::BaseServer::serverLooping, this));
}

bool    rtp::BaseServer::isRunning() const {
    return _threadRunning && _acceptor->isRunning();
}

void    rtp::BaseServer::stop() {
    if (_threadRunning) {
        _threadRunning = false;
        _clientNotifier.notify_one();
        _thread->join();
        _thread.reset();
    }
}

void    rtp::BaseServer::serverLooping() {
    while (_threadRunning) {
        std::cout << "here" << std::endl;
    }
}

rtp::BaseServer::~BaseServer() {
    stop();
}