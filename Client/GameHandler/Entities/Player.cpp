//
// Created by Arnaud WURMEL on 15/01/2018.
//

#include "Player.hh"

rtp::Player::Player(std::string const& spriteName, int nbFrames, int currentFrame, int x, int y, int width, int height, std::string const& pseudo) : ADrawableEntity(spriteName, nbFrames, currentFrame, x, y, width, height, pseudo) {

}

rtp::Player::~Player() {}