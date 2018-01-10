#include "Star.hpp"

Star::Star() {}

Star::Star(sf::Uint16 const& x, sf::Uint16 const& y, sf::Uint16 const& size) : _x(x), _y(y), _size(size) {
  _starImg.create(_size, _size, sf::Color::White);
}

sf::Uint16 const& Star::getX() const {
  return _x;
}

sf::Uint16 const& Star::getY() const {
  return _y;
}

sf::Image const& Star::getImage() const {
  return _starImg;
}

sf::Image&  Star::getImage() {
  return _starImg;
}

sf::Uint16 const& Star::getSize() const {
  return _size;
}

void Star::setX(sf::Uint16 const& x) {
  _x = x;
}

void Star::setY(sf::Uint16 const& y) {
  _y = y;
}

void Star::moveY(sf::Uint16 const& add) {
  _y += add;
}

Star::~Star() {}