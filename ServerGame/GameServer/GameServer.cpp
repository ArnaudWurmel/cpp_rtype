//
// Created by Arnaud WURMEL on 09/01/2018.
//

#include "GameServer.hh"
#include "../NetworkAbstract/BoostSocket.h"
#include "../NetworkAbstract/BoostAcceptor.hh"

rtp::GameServer::GameServer(unsigned short port) : _port(port) {
    _callbackPtrs.insert(std::make_pair(Command::REGISTER, &rtp::GameServer::handleRegistering));
    _callbackPtrs.insert(std::make_pair(Command::PING, &rtp::GameServer::handlePing));
}

bool    rtp::GameServer::connectToAuthServer(std::string const& authHost, unsigned short authPort) {
    _acceptor = std::unique_ptr<NetworkAbstract::IAcceptor>(new NetworkAbstract::BoostAcceptor(_port, _inputAvailable));
    _controlSocket = _acceptor->getEmptySocket(_inputAvailable);
    _acceptor->run();
    return _controlSocket->connectSocket(authHost, authPort);
}

bool    rtp::GameServer::registerServer() {
    NetworkAbstract::Message    registerMessage;

    registerMessage.setType(rtp::GameServer::Command::REGISTER);
    registerMessage.setBody(std::to_string(_port).c_str(), std::to_string(_port).length());
    _controlSocket->write(registerMessage);
    return waitCommand(&rtp::GameServer::handleRegistering, NetworkAbstract::getTypeOf<Command>(registerMessage));
}

bool    rtp::GameServer::waitCommand(Callback callback, rtp::GameServer::Command command) {
    bool found = false;
    int    maxWaiting = 40;

    while (!found && maxWaiting > 0 && _controlSocket->isOpen()) {
        std::unique_lock<std::mutex>    lck(_inputLocker);

        _inputAvailable.wait_for(lck, std::chrono::milliseconds(100));
        if (_controlSocket->haveAvailableData()) {
            NetworkAbstract::Message    message = _controlSocket->getAvailableMessage();

            if (message.getType() == command) {
                return (this->*callback)(message);
            }
            _controlSocket->addMessage(message);
        }
        --maxWaiting;
    }
    return false;
}

bool    rtp::GameServer::handleMessage(NetworkAbstract::Message const& message) {
    if (_callbackPtrs.find(NetworkAbstract::getTypeOf<Command>(message)) != _callbackPtrs.end()) {
        return (this->*(_callbackPtrs[NetworkAbstract::getTypeOf<Command>(message)]))(message);
    }
    return false;
}

void    rtp::GameServer::serverLoop() {
    while (_controlSocket->isOpen()) {
        std::unique_lock<std::mutex>    lck(_inputLocker);

        while (_controlSocket->haveAvailableData()) {
            NetworkAbstract::Message    message = _controlSocket->getAvailableMessage();

            if (!handleMessage(message)) {
                say("Can't handle type : <" + std::to_string(message.getType()) + ">");
                return ;
            }
        }
        _inputAvailable.wait_for(lck, std::chrono::milliseconds(200));
    }
}

bool    rtp::GameServer::handleRegistering(NetworkAbstract::Message const& message) {
    if (message.getBodySize() != TOKEN_SIZE) {
        return false;
    }
    _authToken = std::string(message.getBody(), message.getBodySize());
    return true;
}

bool    rtp::GameServer::handlePing(NetworkAbstract::Message const& message) {
    NetworkAbstract::Message    response;

    response.setType(Command::PING);
    response.setBody(std::string("Available").c_str(), std::string("Available").length());
    _controlSocket->write(response);
    return true;
}

rtp::GameServer::~GameServer() = default;