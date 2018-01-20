//
// Created by Arnaud WURMEL on 20/01/2018.
//

#include "AllyBullet.hh"

rtp::AllyBullet::AllyBullet(Vector2<int> position, unsigned int damage) : ABullet(BulletType::Ally, position, rtp::Vector2<int>{0, -400}) {
    _damage = damage;
}

// Do nothing there is no TK.
void    rtp::AllyBullet::onCollide(APlayer &) {}

rtp::AllyBullet::~AllyBullet() = default;