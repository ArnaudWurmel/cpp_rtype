//
// Created by Arnaud WURMEL on 09/01/2018.
//

#include "GameServer.hh"
#include "../NetworkAbstract/BoostSocket.h"
#include "../NetworkAbstract/BoostSocketManager.hh"
#include "../NetworkAbstract/BoostUdpInputManager.hpp"

rtp::GameServer::GameServer(unsigned short port) : _port(port) {
    _serverState = NotRegistered;
    _callbackPtrs.insert(std::make_pair(Command::REGISTER, &rtp::GameServer::handleRegistering));
    _callbackPtrs.insert(std::make_pair(Command::PING, &rtp::GameServer::handlePing));
    _callbackPtrs.insert(std::make_pair(Command::RESERVED, &rtp::GameServer::handleReserved));

    _stateTranslator.insert(std::make_pair(ServerState::Available, "Available"));
    _stateTranslator.insert(std::make_pair(ServerState::Busy, "Busy"));
    _stateTranslator.insert(std::make_pair(ServerState::NotRegistered, "Not registered"));
    _stateTranslator.insert(std::make_pair(ServerState::Unknown, "Unknown"));
}

bool    rtp::GameServer::connectToAuthServer(std::string const& authHost, unsigned short authPort) {
    _socketManager = std::unique_ptr<NetworkAbstract::ISocketManager>(new NetworkAbstract::BoostSocketManager());
    _controlSocket = _socketManager->getSocket(NetworkAbstract::ISocketManager::SocketType::TCP, _inputAvailable);
    _inputManager = std::unique_ptr<NetworkAbstract::IUdpInputManager>(new NetworkAbstract::BoostUdpInputManager<APlayer>(_port));
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
    bool    threadContinue = true;
    std::unique_ptr<std::thread> networkManager(new std::thread([&] {
        while (threadContinue && !_socketManager->isEnabled()) {}
        if (!registerServer()) {
            _socketManager->disableSocket();
            return ;
        }
        while (threadContinue && _controlSocket->isOpen()) {
            if (_serverState == ServerState::Busy && _lockedAt.time_since_epoch().count() + 5000000 < std::chrono::system_clock::now().time_since_epoch().count() && !_inputManager->haveAcceptedClient()) {
                threadContinue = false;
            }
            std::unique_lock<std::mutex> lck(_inputLocker);
            _inputAvailable.wait_for(lck, std::chrono::milliseconds(200));

            while (_controlSocket->isOpen() && _controlSocket->haveAvailableData()) {
                NetworkAbstract::Message message = _controlSocket->getAvailableMessage();

                if (!handleMessage(message)) {
                    say("Can't handle type : <" + std::to_string(message.getType()) + ">");
                }
            }
        }
        _gameRunning = false;
        if (_gameManager) {
            _gameManager->join();
        }
        _gameManager.reset();
        _controlSocket->close();
        _inputManager->stop();
        _inputManager.reset();
        _controlSocket.reset();
        _socketManager->disableSocket();
    }));
    try {
        _socketManager->enableSocket();
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << "End" << std::endl;
    threadContinue = false;
    networkManager->join();
}

void    rtp::GameServer::handleGame() {
    std::vector<std::shared_ptr<AEnemy> >   enemyList;
    double   diff = 0.0;
    unsigned int    frame = 0;

    while (_gameRunning) {
        std::chrono::milliseconds startPoint = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        _inputManager->updateAllPlayer(diff);
        _inputManager->sendUpdate();
        if (frame % POP_RATE == 0 && _inputManager->haveAcceptedClient()) {
            auto tmpEnemyList = _monsterInstanciater.instanciateWave(MonsterInstanciater::BASIC);
            auto enemyListIt = tmpEnemyList.begin();

            while (enemyListIt != tmpEnemyList.end()) {
                enemyList.push_back(*enemyListIt);
                ++enemyListIt;
            }
            frame = 0;
        }
        auto iterator = enemyList.begin();

        while (iterator != enemyList.end()) {
            (*iterator)->update(diff);
            ++iterator;
        }
        ++frame;
        _inputManager->sendUpdateForEnemies(enemyList);
        _inputManager->handleCollision(enemyList);
        std::chrono::milliseconds endPoint = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        diff = (20 - (endPoint.count() - startPoint.count())) / 1000.0;
        std::this_thread::sleep_for(std::chrono::milliseconds(20) - (endPoint - startPoint));
    }
    std::cout << "EXITING!!!" << std::endl;
}

bool    rtp::GameServer::handleRegistering(NetworkAbstract::Message const& message) {
    if (message.getBodySize() != TOKEN_SIZE) {
        return false;
    }
    _serverState = Available;
    _authToken = std::string(message.getBody(), message.getBodySize());
    std::cout << "Token : " << _authToken << std::endl;
    _inputManager->initSocket(_authToken);
    _inputManager->run();
    return true;
}

bool    rtp::GameServer::handleReserved(NetworkAbstract::Message const& message) {
    NetworkAbstract::Message    response;

    response.setType(RESERVED);
    if (_serverState == Available) {
        _lockedAt = std::chrono::system_clock::now();
        _inputManager->startAcceptingClient();
        _gameRunning = true;
        _gameManager = std::unique_ptr<std::thread>(new std::thread(&rtp::GameServer::handleGame, this));
        _inputManager->run();
        _serverState = Busy;
        std::cout << "Reserved" << std::endl;
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
    std::cout << _stateTranslator[_serverState] << std::endl;
    return true;
}

std::vector<std::string>    rtp::GameServer::getTokenFrom(std::string const& input, char sep) {
    std::vector<std::string>    tokenList;
    std::string::const_iterator   it;

    it = input.begin();
    while (it != input.end()) {
        std::string token;
        while (it != input.end() && *it != sep) {
            token += *it;
            ++it;
        }
        if (token.size())
            tokenList.push_back(token);
        while (it != input.end() && *it == sep)
            ++it;
    }
    return tokenList;
}

rtp::GameServer::~GameServer() {}