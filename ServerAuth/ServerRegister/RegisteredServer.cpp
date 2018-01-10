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
    _serverState = ServerState::NotRegistered;
    _stateServerTranslate.insert(std::make_pair(ServerState::Available, "Available"));
    _stateServerTranslate.insert(std::make_pair(ServerState::Busy, "Busy"));
    _stateServerTranslate.insert(std::make_pair(ServerState::Unknown, "Unknown"));
    _stateServerTranslate.insert(std::make_pair(ServerState::NotRegistered, "Server not registered"));
    _funcPtrList.insert(std::make_pair(Command::REGISTER, std::bind(&rtp::RegisteredServer::registerServer, this, std::placeholders::_1)));
    _funcPtrList.insert(std::make_pair(Command::PING, std::bind(&rtp::RegisteredServer::pingResult, this, std::placeholders::_1)));
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
    if (isRegistered()) {
        NetworkAbstract::Message    pingMessage;

        pingMessage.setType(Command::PING);
        _socket->write(pingMessage);
    }
    return true;
}

unsigned int    rtp::RegisteredServer::getId() const {
    return _id;
}

bool    rtp::RegisteredServer::isRegistered() const {
    return _port != -1 && _registrationToken.length() > 0;
}

bool    rtp::RegisteredServer::pingResult(NetworkAbstract::Message const& message) {
    if (message.getBodySize() == 0) {
        return false;
    }
    std::string body(message.getBody(), message.getBodySize());

    _serverState = getStateFromString(body);
    return true;
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

rtp::RegisteredServer::ServerState rtp::RegisteredServer::getState() const {
    return _serverState;
}

std::string rtp::RegisteredServer::getStateTranslated() {
    if (_stateServerTranslate.find(_serverState) != _stateServerTranslate.end()) {
        return _stateServerTranslate[_serverState];
    }
    return "";
}

rtp::RegisteredServer::ServerState rtp::RegisteredServer::getStateFromString(std::string const& state) const {
    auto iterator = _stateServerTranslate.begin();

    while (iterator != _stateServerTranslate.end()) {
        if (!(*iterator).second.compare(state)) {
            return (*iterator).first;
        }
        ++iterator;
    }
    return ServerState::Unknown;
}

rtp::RegisteredServer::~RegisteredServer() {
    _socket->close();
}