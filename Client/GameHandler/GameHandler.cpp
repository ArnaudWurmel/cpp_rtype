//
// Created by Arnaud WURMEL on 15/01/2018.
//

#include <iostream>
#include "GameHandler.hh"
#include "../Exceptions/Exception.hh"

rtp::GameHandler::GameHandler(std::shared_ptr<NetworkAbstract::ISocket> socket, std::shared_ptr<Player> player) : _gameSocket(socket) {
    _playerList.push_back(player);
    if (player->init()) {
        _callbackList.insert(std::make_pair(Command::SPAWN_PLAYER, std::bind(&rtp::GameHandler::handlePlayerSpawn, this, std::placeholders::_1)));
    }
    else {
        throw ParsingNetworkException();
    }
}

bool    rtp::GameHandler::update(sf::RenderWindow& window) {
    while (_gameSocket->haveAvailableData()) {
        NetworkAbstract::Message    message = _gameSocket->getAvailableMessage();

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
    return _gameSocket->isOpen();
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

rtp::GameHandler::~GameHandler() {}