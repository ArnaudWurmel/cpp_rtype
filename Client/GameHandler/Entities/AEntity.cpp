//
// Created by Arnaud WURMEL on 20/01/2018.
//

#include <memory>
#include "AEntity.hh"
#include "../../Exceptions/Exception.hh"
#include "../../DataGetter/DataGetter.hh"

std::shared_ptr<rtp::AEntity>   rtp::AEntity::instanciateFromToken(std::vector<std::string> const& tokenList) {
    if (tokenList.size() == 7) {
        std::shared_ptr<rtp::AEntity> newEntity(new rtp::AEntity(std::stoi(tokenList[0]), std::stoi(tokenList[1]), std::stoi(tokenList[2]), std::stoi(tokenList[3]), std::stoi(tokenList[4]), tokenList[5]));

        newEntity->parseFrame(rtp::DataGetter::getTokenFrom(tokenList[6], ';'));
        return newEntity;
    }
    throw rtp::ParsingNetworkException();
}

rtp::AEntity::AEntity(int entityId, int rotation, int x, int y, int currentFrame, std::string const &spriteName) : ADrawableEntity(spriteName, currentFrame, x, y, "", rotation) {
    _entityId = entityId;
}

int rtp::AEntity::getId() const {
    return _entityId;
}

void    rtp::AEntity::updateFrom(std::vector<std::string> const& tokenList) {
    if (tokenList.size() == 5) {
        _renderLock.lock();
        _rotation = std::stoi(tokenList[1]);
        _x = std::stoi(tokenList[2]);
        _y = std::stoi(tokenList[3]);
        _currentFrame = std::stoi(tokenList[4]);
        _renderLock.unlock();
    }
}

rtp::AEntity::~AEntity() {
}