//
// Created by Arnaud WURMEL on 15/01/2018.
//

#include <iostream>
#include "GameHandler.hh"
#include "../Exceptions/Exception.hh"
#include "../DataGetter/DataGetter.hh"

rtp::GameHandler::GameHandler(std::shared_ptr<NetworkAbstract::ISocket> socket, std::shared_ptr<Player> player) : _gameSocket(socket) {
    _playerList.push_back(player);
    if (player->init()) {
        _callbackList.insert(std::make_pair(Command::SPAWN_PLAYER, std::bind(&rtp::GameHandler::handlePlayerSpawn, this, std::placeholders::_1)));
        _callbackList.insert(std::make_pair(Command::UPDATE_PLAYER, std::bind(&rtp::GameHandler::handleUpdatePlayer, this, std::placeholders::_1)));
        _callbackList.insert(std::make_pair(Command::SPAWN_ENTITY, std::bind(&rtp::GameHandler::handleSpawnEntity, this, std::placeholders::_1)));
        _callbackList.insert(std::make_pair(Command::DELETE_ENTITY, std::bind(&rtp::GameHandler::handleDeleteEntity, this, std::placeholders::_1)));
        _callbackList.insert(std::make_pair(Command::UPDATE_ENTITY, std::bind(&rtp::GameHandler::handleUpdateEntity, this, std::placeholders::_1)));
    }
    else {
        throw ParsingNetworkException();
    }
    _lastMessage = std::chrono::system_clock::now();
    _threadRunning = true;
    _updaterThread = std::unique_ptr<std::thread>(new std::thread(&rtp::GameHandler::updateLoop, this));
}

bool    rtp::GameHandler::update(sf::RenderWindow& window) {
    handleTouchIsPressed();
    _entitySafer.lock();
    auto iterator = _playerList.begin();

    sf::Clock   clock;
    while (iterator != _playerList.end()) {
        if ((*iterator)->shouldDelete()) {
            _playerList.erase(iterator);
        }
        else {
            (*iterator)->render();
            window.draw(*(*iterator).get());
            ++iterator;
        }
    }
    auto    iteratorEntity  = _entitiesList.begin();

    while (iteratorEntity != _entitiesList.end()) {
        if ((*iteratorEntity)->shouldDelete()) {
            _entitiesList.erase(iteratorEntity);
        }
        else {
            (*iteratorEntity)->render();
            window.draw(*(*iteratorEntity).get());
            ++iteratorEntity;
        }
    }
    std::cout << _entitiesList.size() << std::endl;
    _entitySafer.unlock();
    return _threadRunning && _gameSocket->isOpen();
}

bool    rtp::GameHandler::handlePlayerSpawn(std::string const& pInfo) {
    try {
        std::shared_ptr<Player> newPlayer = Player::instanciateFromInfo(pInfo);

        _entitySafer.lock();
        auto iterator = std::find_if(_playerList.begin(), _playerList.end(), [&](std::shared_ptr<Player> const& player) {
            return player->getId() == newPlayer->getId();
        });
        if (iterator != _playerList.end() || !newPlayer->init()) {
            _entitySafer.unlock();
            return true;
        }
        _playerList.push_back(newPlayer);
        _entitySafer.unlock();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

void    rtp::GameHandler::handleTouchIsPressed() {
    forward();
    backward();
    left();
    right();
}

void    rtp::GameHandler::forward() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        NetworkAbstract::Message    message;

        message.setType(FORWARD);
        _gameSocket->write(message);
        std::cout << "Sended" << std::endl;
    }
}

void    rtp::GameHandler::backward() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        NetworkAbstract::Message    message;

        message.setType(BACKWARD);
        _gameSocket->write(message);
    }
}

void    rtp::GameHandler::left() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        NetworkAbstract::Message    message;

        message.setType(LEFT);
        _gameSocket->write(message);
    }
}

void    rtp::GameHandler::right() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        NetworkAbstract::Message    message;

        message.setType(RIGHT);
        _gameSocket->write(message);
    }
}

bool    rtp::GameHandler::handleUpdatePlayer(std::string const& pInfo) {
    std::vector<std::string>    pInfosArr = rtp::DataGetter::getTokenFrom(pInfo);

    auto iterator = _playerList.begin();

    while (iterator != _playerList.end()) {
        if ((*iterator)->getId() == std::stoi(pInfosArr[0])) {
            (*iterator)->updateFrom(pInfosArr);
            return true;
        }
        ++iterator;
    }
    return true;
}

bool    rtp::GameHandler::handleSpawnEntity(std::string const& spawnMessage) {
    try {
        auto newEntity = AEntity::instanciateFromToken(DataGetter::getTokenFrom(spawnMessage));

        if (newEntity->init()) {
            _entitySafer.lock();
            _entitiesList.push_back(newEntity);
            _entitySafer.unlock();
        }
    }
    catch (std::exception& e) {
        std::cout << "Error in entities creation : " << e.what() << std::endl;
    }
    return true;
}

bool    rtp::GameHandler::handleDeleteEntity(std::string const& deleteMessage) {
    if (deleteMessage.length() > 0) {
        try {
            int entityId = std::stoi(deleteMessage);

            std::cout << "Delete : " << entityId << std::endl;
            _entitySafer.lock();
            auto iterator = std::find_if(_entitiesList.begin(), _entitiesList.end(), [&](std::shared_ptr<AEntity> const& entity) {
                return entity->getId() == entityId;
            });
            if (iterator != _entitiesList.end()) {
                (*iterator)->deleteEntity();
            }
            _entitySafer.unlock();
        }
        catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
    return true;
}

bool    rtp::GameHandler::handleUpdateEntity(std::string const& updateMessage) {
    std::vector<std::string>    tokenList = DataGetter::getTokenFrom(updateMessage);

    _entitySafer.lock();
    auto iterator = std::find_if(_entitiesList.begin(), _entitiesList.end(), [&](std::shared_ptr<AEntity> const& entity) {
        return entity->getId() == std::stoi(tokenList[0]);
    });
    if (iterator != _entitiesList.end()) {
        (*iterator)->updateFrom(tokenList);
    }
    _entitySafer.unlock();
    return true;
}

void    rtp::GameHandler::updateLoop() {
    while (_threadRunning) {
        while (_threadRunning && _gameSocket->haveAvailableData()) {
            NetworkAbstract::Message    message = _gameSocket->getAvailableMessage();

            _lastMessage = std::chrono::system_clock::now();
            if (_callbackList.find(NetworkAbstract::getTypeOf<Command>(message)) != _callbackList.end()) {
                if (!_callbackList[NetworkAbstract::getTypeOf<Command>(message)](std::string(message.getBody(), message.getBodySize()))) {
                    _threadRunning = false;
                    return ;
                }
            }
            else {
                std::cerr << "Doesn't handle type : <" << message.getType() << ">" << std::endl;
            }
        }
    }
}

rtp::GameHandler::~GameHandler() {
    _threadRunning = false;
    _updaterThread->join();
}