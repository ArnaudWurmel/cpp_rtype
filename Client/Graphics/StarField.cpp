#include "StarField.hpp"
#include <iostream>

Starfield::Starfield(int xRes, int yRes, int nbStars) {
  this->xRes = xRes;
  this->yRes = yRes;
  this->nbStars = nbStars;
  this->rng.seed(std::time(0));
  this->rsize = std::uniform_int_distribution<int>(1, 4);
  this->rx = std::uniform_int_distribution<int>(0, this->xRes);
  this->ry = std::uniform_int_distribution<int>(0, 50);
  while (this->Stars.size() <= this->nbStars) {
    this->Stars.push_back(
        Star(this->rx(this->rng), this->ry(this->rng), this->rsize(rng)));
  }
}

void Starfield::UpdateStar() {
  for (std::vector<Star>::iterator it = this->Stars.begin();
       it != this->Stars.end(); it++) {
    if (it->GetY() >= this->yRes)
      this->Stars.erase(it);
  }

  for (std::vector<Star>::iterator it = this->Stars.begin();
       it != this->Stars.end(); it++) {
    it->MoveY(it->GetSize());
  }

  while (this->Stars.size() <= this->nbStars)
    this->Stars.push_back(
        Star(this->rx(this->rng), this->ry(this->rng), this->rsize(this->rng)));
  return;
}

void Starfield::DrawStar(sf::Texture &t) {
  for (std::vector<Star>::iterator it = this->Stars.begin();
       it != this->Stars.end(); it++) {
    std::cout << it->GetX() << " " << it->GetY() << " " << it->GetSize()
              << '\n';
    if (it->GetY() + it->GetSize() < this->yRes &&
        it->GetX() + it->GetSize() < this->xRes)
      t.update(it->GetImg(), it->GetX(), it->GetY());
  }
  return;
}
