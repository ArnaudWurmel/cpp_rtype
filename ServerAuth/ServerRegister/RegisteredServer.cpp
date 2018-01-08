//
// Created by Arnaud WURMEL on 08/01/2018.
//

#include "RegisteredServer.hh"

rtp::RegisteredServer::RegisteredServer(std::shared_ptr<NetworkAbstract::ISocket> controlSocket) {
    _socketControl = controlSocket;
}

bool    rtp::RegisteredServer::handleNewData() {
    return true;
}

rtp::RegisteredServer::~RegisteredServer() {}