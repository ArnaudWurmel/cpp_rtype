//
// Created by Arnaud WURMEL on 08/01/2018.
//

#include "RegisteredServer.hh"

unsigned int    rtp::RegisteredServer::_serverId = 0;

rtp::RegisteredServer::RegisteredServer(std::shared_ptr<NetworkAbstract::ISocket> controlSocket) : SocketReceiver(controlSocket) {
    _id = rtp::RegisteredServer::_serverId++;
}

bool    rtp::RegisteredServer::handleNewData() {
    if (_socket->haveAvailableData()) {

    }
    return true;
}

unsigned int    rtp::RegisteredServer::getId() const {
    return _id;
}

rtp::RegisteredServer::~RegisteredServer() {}