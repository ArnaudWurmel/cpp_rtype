#ifndef MAINVIEW_HPP
#define MAINVIEW_HPP

#include "IView.hpp"
#include "StarField.hpp"

class MainView : public IView {
private:
  sf::Image _starsImage;
  sf::Texture _starsTexture;
  sf::Sprite _starsSprite;
  sf::Font _font;
  sf::Text _text;
  Starfield _stars;
  char _name[255];
  char _ipPort[255] = "0.0.0.0:0000";

public:
  MainView(sf::Vector2u &);
  ~MainView();
  Starfield getStarfield();
  bool Update(sf::RenderWindow &);
};

#endif
