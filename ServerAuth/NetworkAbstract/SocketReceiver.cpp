//
// Created by Arnaud WURMEL on 08/01/2018.
//

#include <string>
#include "SocketReceiver.hh"

NetworkAbstract::SocketReceiver::SocketReceiver(std::shared_ptr<ISocket> socket) {
    _socket = socket;
}

void    NetworkAbstract::SocketReceiver::write(NetworkAbstract::Message message) {
    _socket->write(message);
}

void    NetworkAbstract::SocketReceiver::close() {
    _socket->close();
}

bool    NetworkAbstract::SocketReceiver::isOpen() const {
    return _socket->isOpen();
}

std::string    NetworkAbstract::SocketReceiver::getIpAddr() const {
    return _socket->getIpAddr();
}

NetworkAbstract::SocketReceiver::~SocketReceiver() = default;