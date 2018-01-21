//
// Created by Arnaud WURMEL on 10/01/2018.
//

#include "BaseServer.hh"
#ifdef _WIN32
#include "../NetworkAbstractWin/BoostAcceptor.hh"
#else
#include "../NetworkAbstract/BoostAcceptor.hh"
#endif // _WIN32

rtp::BaseServer::BaseServer(unsigned short port) {
    _threadRunning = true;
    _acceptor = std::unique_ptr<NetworkAbstract::IAcceptor>(new NetworkAbstract::BoostAcceptor( port, _clientNotifier));
}

void    rtp::BaseServer::run() {
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

rtp::BaseServer::~BaseServer() {
    stop();
}