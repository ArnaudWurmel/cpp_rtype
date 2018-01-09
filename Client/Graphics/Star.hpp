#ifndef STAR_HPP
#define STAR_HPP

#include <SFML/Graphics.hpp>

class Star {
private:
  sf::Uint16 x;
  sf::Uint16 y;
  sf::Uint16 size;
  sf::Image StarImg;

public:
  Star(){};
  Star(sf::Uint16, sf::Uint16, sf::Uint16);
  ~Star(){};
  sf::Uint16 GetX();
  sf::Uint16 GetY();
  sf::Uint16 GetSize();
  sf::Image GetImg();
  void SetX(sf::Uint16);
  void SetY(sf::Uint16);
  void MoveY(sf::Uint16);
};

#endif
