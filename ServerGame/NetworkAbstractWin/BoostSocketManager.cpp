//
// Created by Arnaud WURMEL on 18/01/2018.
//

#include <iostream>
#include "BoostSocketManager.hh"
#include "BoostSocket.h"
#include "BoostSocketUdp.hh"

boost::asio::io_service NetworkAbstract::BoostSocketManager::_ioService;

NetworkAbstract::BoostSocketManager::BoostSocketManager() {

}

std::shared_ptr<NetworkAbstract::ISocket>   NetworkAbstract::BoostSocketManager::getSocket(SocketType type, std::condition_variable& cv) {
    if (type == SocketType::TCP) {
        return std::shared_ptr<ISocket>(new BoostSocket(_ioService, cv));
    }
    else {
        return std::shared_ptr<ISocket>(new BoostUdpSocket(_ioService, cv));
    }
}

void    NetworkAbstract::BoostSocketManager::enableSocket() {
    boost::system::error_code   e;

    _ioService.run();
    std::cout << e.message() << std::endl;
}

void    NetworkAbstract::BoostSocketManager::disableSocket() {
    std::cout << "Disable socket" << std::endl;
    _ioService.stop();
    _ioService.reset();
}

bool    NetworkAbstract::BoostSocketManager::isEnabled() const {
    return !_ioService.stopped();
}

NetworkAbstract::BoostSocketManager::~BoostSocketManager() {}