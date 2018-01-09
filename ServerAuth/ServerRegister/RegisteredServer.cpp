//
// Created by Arnaud WURMEL on 08/01/2018.
//

#include <iostream>
#include "RegisteredServer.hh"

unsigned int    rtp::RegisteredServer::_serverId = 0;

rtp::RegisteredServer::RegisteredServer(std::shared_ptr<NetworkAbstract::ISocket> controlSocket) : SocketReceiver(controlSocket) {
    _id = rtp::RegisteredServer::_serverId++;
    _port = -1;
    _funcPtrList.insert(std::make_pair(Command::REGISTER, std::bind(&rtp::RegisteredServer::registerServer, this, std::placeholders::_1)));
}

bool    rtp::RegisteredServer::handleNewData() {
    while (_socket->haveAvailableData()) {
        NetworkAbstract::Message    message = _socket->getAvailableMessage();

        say(std::to_string(message.getType()));
    }
    return true;
}

unsigned int    rtp::RegisteredServer::getId() const {
    return _id;
}

bool    rtp::RegisteredServer::isRegistered() const {
    return _port != -1;
}

bool    rtp::RegisteredServer::registerServer(std::vector<std::string> const& tokenList) {
    return false;
}

rtp::RegisteredServer::~RegisteredServer() {}