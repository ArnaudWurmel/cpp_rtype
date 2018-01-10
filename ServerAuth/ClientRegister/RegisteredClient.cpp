//
// Created by Arnaud WURMEL on 10/01/2018.
//

#include "ClientRegister.hh"
#include "RegisteredClient.hh"

unsigned int rtp::RegisteredClient::_clientId = 0;

rtp::RegisteredClient::RegisteredClient(std::shared_ptr<NetworkAbstract::ISocket> socket, rtp::ClientRegister& clientRegister) : SocketReceiver(socket), _delegate(clientRegister) {
    _id = rtp::RegisteredClient::_clientId++;
}

bool    rtp::RegisteredClient::handleNewData() {
    return true;
}

unsigned int    rtp::RegisteredClient::getId() const {
    return _id;
}

rtp::RegisteredClient::~RegisteredClient() {}