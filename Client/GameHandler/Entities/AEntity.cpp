//
// Created by Arnaud WURMEL on 20/01/2018.
//

#include <memory>
#include "AEntity.hh"
#include "../../Exceptions/Exception.hh"
#include "../../DataGetter/DataGetter.hh"

std::shared_ptr<rtp::AEntity>   rtp::AEntity::instanciateFromToken(std::vector<std::string> const& tokenList) {
    std::cout << tokenList.size() << std::endl;
    if (tokenList.size() == 7) {
        std::shared_ptr<rtp::AEntity> newEntity(new rtp::AEntity(std::stoi(tokenList[0]), std::stoi(tokenList[1]), std::stoi(tokenList[2]), std::stoi(tokenList[3]), std::stoi(tokenList[4]), tokenList[5]));

        newEntity->parseFrame(rtp::DataGetter::getTokenFrom(tokenList[6], ';'));
        return newEntity;
    }
    throw rtp::ParsingNetworkException();
}

rtp::AEntity::AEntity(int entityId, int rotation, int x, int y, int currentFrame, std::string const &spriteName) : ADrawableEntity(spriteName, currentFrame, x, y, "", rotation) {

}

rtp::AEntity::~AEntity() {}