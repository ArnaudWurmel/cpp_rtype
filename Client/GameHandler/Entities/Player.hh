//
// Created by Arnaud WURMEL on 15/01/2018.
//

#ifndef CLIENTNETWORK_PLAYER_HH
#define CLIENTNETWORK_PLAYER_HH

# include "ADrawableEntity.hh"

namespace rtp {
    class Player : public ADrawableEntity {
    public:
        Player(std::string const& spriteName, int nbFrames, int currentFrame, int x, int y, int width, int height, std::string const& pseudo);
        ~Player() override;
    };
}

#endif //CLIENTNETWORK_PLAYER_HH
