#ifndef STARFIELD_HPP
#define STARFIELD_HPP

#include "Star.hpp"
#include <SFML/Graphics.hpp>
#include <random>
#include <vector>

class Starfield {
public:
  Starfield(int xRes, int yRes, int nbStars);
  ~Starfield(){};
  void UpdateStar();
  void DrawStar(sf::Texture &);

private:
  std::vector<Star> Stars;
  int xRes;
  int yRes;
  unsigned int nbStars;
  std::mt19937 rng;
  std::uniform_int_distribution<int> rsize;
  std::uniform_int_distribution<int> rx;
  std::uniform_int_distribution<int> ry;
};

#endif
