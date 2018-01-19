#ifndef STARFIELD_HPP
#define STARFIELD_HPP

#include "Star.hpp"
#include <SFML/Graphics.hpp>
#include <random>
#include <vector>

class Starfield {
public:
  Starfield(){};
  Starfield(int xRes, int yRes, unsigned int nbStars);
  ~Starfield(){};

public:
  void updateStar();
  void drawStar(sf::Texture &) const;

private:
    std::vector<Star> _stars;
    int _xRes;
    int _yRes;
    unsigned int _nbStars;
    std::mt19937 _rng;
    std::uniform_int_distribution<int> _rsize;
    std::uniform_int_distribution<int> _rx;
    std::uniform_int_distribution<int> _ry;
};

#endif
