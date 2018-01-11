#include "StarField.hpp"
#include <iostream>

Starfield::Starfield(int xRes, int yRes, unsigned int nbStars) {
    _xRes = xRes;
    _yRes = yRes;
    _nbStars = nbStars;
	_rng.seed(std::time(0));
    _rsize = std::uniform_int_distribution<int>(1, 4);
    _rx = std::uniform_int_distribution<int>(0, xRes);
    _ry = std::uniform_int_distribution<int>(0, 0);
    while (_stars.size() <= _nbStars) {
        _stars.push_back(Star(_rx(_rng), _ry(_rng), _rsize(_rng)));
    }
}

void Starfield::updateStar() {
	std::vector<Star>::iterator it = _stars.begin();
	while (it != _stars.end()) {
		if (it->getY() >= _yRes)
			it = _stars.erase(it);
		else
			it++;
	}
	for (it = _stars.begin(); it != _stars.end(); it++) {
		it->moveY(it->getSize());
	}
	while (_stars.size() <= _nbStars) {
		_stars.push_back(Star(_rx(_rng), _ry(_rng), _rsize(_rng)));
	}
}

void Starfield::drawStar(sf::Texture &t) const {
    for (std::vector<Star>::const_iterator it = _stars.begin(); it != _stars.end(); it++) {
        if (it->getY() + it->getSize() < _yRes && it->getX() + it->getSize() < _xRes) {
			t.update(it->getImage(), it->getX(), it->getY());
        }
    }
}
