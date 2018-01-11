#ifndef IVIEW_HPP
#define IVIEW_HPP

#include <SFML/Graphics.hpp>

class IView {
public:
  virtual ~IView() {}
  virtual bool Update(sf::RenderWindow &) = 0;
};

#endif
