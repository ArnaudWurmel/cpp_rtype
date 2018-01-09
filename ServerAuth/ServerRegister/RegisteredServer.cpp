//
// Created by Arnaud WURMEL on 08/01/2018.
//

#include <iostream>
#include "RegisteredServer.hh"

unsigned int    rtp::RegisteredServer::_serverId = 0;

rtp::RegisteredServer::RegisteredServer(std::shared_ptr<NetworkAbstract::ISocket> controlSocket) : SocketReceiver(controlSocket) {
    _id = rtp::RegisteredServer::_serverId++;
    _port = -1;
    _registrationToken = "";
    _funcPtrList.insert(std::make_pair(Command::REGISTER, std::bind(&rtp::RegisteredServer::registerServer, this, std::placeholders::_1)));
}

bool    rtp::RegisteredServer::handleNewData() {
    while (_socket->haveAvailableData()) {
        NetworkAbstract::Message    message = _socket->getAvailableMessage();

        if (_funcPtrList.find(NetworkAbstract::getTypeOf<Command>(message)) != _funcPtrList.end()) {
            if (!_funcPtrList[NetworkAbstract::getTypeOf<Command>(message)](message)) {
                return false;
            }
        }
    }
    return true;
}

unsigned int    rtp::RegisteredServer::getId() const {
    return _id;
}

bool    rtp::RegisteredServer::isRegistered() const {
    return _port != -1 && _registrationToken.length() > 0;
}

bool    rtp::RegisteredServer::registerServer(NetworkAbstract::Message const& message) {
    if (message.getBodySize() == 0) {
        return false;
    }

    std::string messageBody(message.getBody(), message.getBodySize());

    bool    validatePort = !messageBody.empty() && std::find_if(messageBody.begin(),
                                                      messageBody.end(), [](char c) { return !std::isdigit(c); }) == messageBody.end();
    if (!validatePort) {
        return false;
    }
    NetworkAbstract::Message    response;

    _port = std::stoi(messageBody);
    _registrationToken = generateRandomString(32);
    response.setType(Command::REGISTER);
    response.setBody(_registrationToken.c_str(), _registrationToken.length());
    _socket->write(response);
    return true;
}

std::string rtp::RegisteredServer::generateRandomString(ssize_t len) const {
    static const char alphanum[] =
            "0123456789"
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyz";
    std::string line;

    for (int i = 0; i < len; ++i) {
        line += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return line;
}


rtp::RegisteredServer::~RegisteredServer() {
    _socket->close();
}