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
    }
    else {
        throw ParsingNetworkException();
    }
    _lastMessage = std::chrono::system_clock::now();
}

bool    rtp::GameHandler::update(sf::RenderWindow& window) {
    handleTouchIsPressed();
    while (_gameSocket->haveAvailableData()) {
        NetworkAbstract::Message    message = _gameSocket->getAvailableMessage();

        _lastMessage = std::chrono::system_clock::now();
        if (_callbackList.find(NetworkAbstract::getTypeOf<Command>(message)) != _callbackList.end()) {
            if (!_callbackList[NetworkAbstract::getTypeOf<Command>(message)](std::string(message.getBody(), message.getBodySize()))) {
                return false;
            }
        }
        else {
            std::cerr << "Doesn't handle type : <" << message.getType() << ">" << std::endl;
        }
    }
    auto iterator = _playerList.begin();

    while (iterator != _playerList.end()) {
        (*iterator)->render();
        window.draw(*(*iterator).get());
        ++iterator;
    }
    return _gameSocket->isOpen() && _lastMessage.time_since_epoch().count() + 10000000 > std::chrono::system_clock::now().time_since_epoch().count();
}

bool    rtp::GameHandler::handlePlayerSpawn(std::string const& pInfo) {
    try {
        std::shared_ptr<Player> newPlayer = Player::instanciateFromInfo(pInfo);

        auto iterator = std::find_if(_playerList.begin(), _playerList.end(), [&](std::shared_ptr<Player> const& player) {
            return player->getId() == newPlayer->getId();
        });
        if (iterator != _playerList.end() || !newPlayer->init()) {
            return false;
        }
        _playerList.push_back(newPlayer);
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

    std::cout << "Update player" << std::endl;
    while (iterator != _playerList.end()) {
        if ((*iterator)->getId() == std::stoi(pInfosArr[0])) {
            (*iterator)->updateFrom(pInfosArr);
            return true;
        }
        ++iterator;
    }
    return true;
}

rtp::GameHandler::~GameHandler() {}