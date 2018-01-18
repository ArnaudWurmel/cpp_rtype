//
// Created by Arnaud WURMEL on 15/01/2018.
//

#include "CollideRect.hh"

rtp::CollideRect::CollideRect(int x, int y, int width, int height) : _x(x), _y(y), _width(width), _height(height) {}

int rtp::CollideRect::getX() const {
    return _x;
}

int rtp::CollideRect::getY() const {
    return _y;
}

int rtp::CollideRect::getWidth() const {
    return _width;
}

int rtp::CollideRect::getHeight() const {
    return _height;
}

void    rtp::CollideRect::translate(rtp::Vector2<int> const&   vector) {
    _x = vector.x;
    _y = vector.y;
}

rtp::CollideRect::~CollideRect() {}