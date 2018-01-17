//
// Created by Arnaud WURMEL on 09/01/2018.
//

#include "GameServer.hh"
#include "../NetworkAbstract/BoostSocket.h"
#include "../NetworkAbstract/BoostAcceptor.hh"
#include "../NetworkAbstract/BoostUdpServer.hpp"
#include "Entity/APlayer.hh"

rtp::GameServer::GameServer(unsigned short port) : _port(port) {
    _serverState = NotRegistered;
    _callbackPtrs.insert(std::make_pair(Command::REGISTER, &rtp::GameServer::handleRegistering));
    _callbackPtrs.insert(std::make_pair(Command::PING, &rtp::GameServer::handlePing));
    _callbackPtrs.insert(std::make_pair(Command::RESERVED, &rtp::GameServer::handleReserved));
    _stateTranslator.insert(std::make_pair(ServerState::Available, "Available"));
    _stateTranslator.insert(std::make_pair(ServerState::Busy, "Busy"));
    _stateTranslator.insert(std::make_pair(ServerState::NotRegistered, "Not registered"));
    _stateTranslator.insert(std::make_pair(ServerState::Unknown, "Unknown"));
    _gameRunning = false;
}

bool    rtp::GameServer::connectToAuthServer(std::string const& authHost, unsigned short authPort) {
    _gameServer = std::shared_ptr<NetworkAbstract::IServer<NetworkAbstract::BoostUdpClient<APlayer > > >(new NetworkAbstract::BoostUdpServer<NetworkAbstract::BoostUdpClient<APlayer> >(_port, _pLock));
    _controlSocket = _gameServer->getEmptyASocket(_inputAvailable);
    _gameServer->run();
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
        _pLock.lock();
        auto    playerListIt = _gameServer->getClient().begin();

        while (playerListIt != _gameServer->getClient().end()) {
            if ((*playerListIt)->haveTimedOut()) {
                _gameServer->getClient().erase(playerListIt);
                std::cout << "remove player" << std::endl;
            }
            else {
                ++playerListIt;
            }
        }
        _pLock.unlock();
        if (_serverState == ServerState::Busy && _lockedAt.time_since_epoch().count() + 50000000 < std::chrono::steady_clock::now().time_since_epoch().count()) {
            if (_gameServer->getClient().empty()) {
                if (!registerServer()) {
                    _controlSocket->close();
                }
                else {
                    _gameServer->stop();
                    _gameServer->init(_authToken);
                }
            }
            else {
                std::cout << "Doesn't accept anymore connexion" << std::endl;
                _gameServer->acceptIncommingConnexion(false);
            }
        }
        std::unique_lock<std::mutex>    lck(_inputLocker);

        _inputAvailable.wait_for(lck, std::chrono::milliseconds(200));

        while (_controlSocket->haveAvailableData()) {
            NetworkAbstract::Message    message = _controlSocket->getAvailableMessage();

            if (!handleMessage(message)) {
                say("Can't handle type : <" + std::to_string(message.getType()) + ">");
                return ;
            }
        }
    }
}

bool    rtp::GameServer::handleRegistering(NetworkAbstract::Message const& message) {
    if (message.getBodySize() != TOKEN_SIZE) {
        return false;
    }
    _serverState = Available;
    _authToken = std::string(message.getBody(), message.getBodySize());
    std::cout << "Token : " << _authToken << std::endl;
    _gameServer->init(_authToken);
    return true;
}

bool    rtp::GameServer::handleReserved(NetworkAbstract::Message const& message) {
    NetworkAbstract::Message    response;

    response.setType(RESERVED);
    if (_serverState == Available) {
        _serverState = Busy;
        _gameServer->acceptIncommingConnexion(true);
        _lockedAt = std::chrono::steady_clock::now();
        std::cout << "Reserved" << std::endl;
        runGame();
        response.setBody("success", 7);
    }
    else {
        response.setBody("failure", 7);
    }
    _controlSocket->write(message);
    return true;
}

bool    rtp::GameServer::handlePing(NetworkAbstract::Message const& message) {
    NetworkAbstract::Message    response;

    response.setType(Command::PING);
    response.setBody(_stateTranslator[_serverState].c_str(), _stateTranslator[_serverState].length());
    _controlSocket->write(response);
    return true;
}

void    rtp::GameServer::runGame() {
    if (_gameRunning && _gameRunner) {
        _gameRunning = false;
        if (_gameRunner->joinable()) {
            _gameRunner->join();
        }
    }
    float diff = 0.0;
    _gameRunning = true;
    _gameRunner = std::unique_ptr<std::thread>(new std::thread([&] {
        while (_gameRunning) {
            std::clock_t    clock_start = std::clock();
            _pLock.lock();
            auto    iterator = _gameServer->getClient().begin();

            while (iterator != _gameServer->getClient().end()) {
                (*iterator)->handleMoving(diff);
                if ((*iterator)->isUpdated()) {
                    NetworkAbstract::Message    message;
                    std::string updateBody = (*iterator)->getInfos();

                    message.setType(APlayer::Command::UPDATE_PLAYER);
                    message.setBody(updateBody.c_str(), updateBody.length());
                    _gameServer->broadcastToAllClient(message);
                    (*iterator)->setUpdated(false);
                }
                ++iterator;
            }
            _pLock.unlock();
            std::clock_t    clock_end = std::clock();
            diff = ((clock_end - clock_start) / CLOCKS_PER_SEC);
            if (16 - diff > 0) {
                //td::chrono::milliseconds(static_cast<int>(16 - (diff * 1000)));
            }
        }
    }));
}

rtp::GameServer::~GameServer() {
    if (_gameRunning && _gameRunner->joinable()) {
        _gameRunner->join();
    }
}