#include "Star.hpp"

Star::Star(sf::Uint16 x, sf::Uint16 y, sf::Uint16 size) {
  this->x = x;
  this->y = y;
  this->size = size;
  this->StarImg.create(this->size, this->size, sf::Color::White);
}

sf::Uint16 Star::GetX() { return this->x; }

sf::Uint16 Star::GetY() { return this->y; }

sf::Image Star::GetImg() { return this->StarImg; }

sf::Uint16 Star::GetSize() { return this->size; }

void Star::SetX(sf::Uint16 x) {
  this->x = x;
  return;
}

void Star::SetY(sf::Uint16 y) {
  this->y = y;
  return;
}

void Star::MoveY(sf::Uint16 add) {
  this->y += add;
  return;
}
