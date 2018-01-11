#include "MainView.hpp"
#include "imgui-SFML.h"
#include "imgui.h"

MainView::~MainView(){};

MainView::MainView(sf::Vector2u &Dimensions) {
  _starsImage.create(Dimensions.x, Dimensions.y, sf::Color::Black);

  _starsTexture.loadFromImage(_starsImage);
  _starsTexture.setSmooth(false);

  _starsSprite.setTexture(_starsTexture);
  _starsSprite.setPosition(0, 0);

  _font.loadFromFile("./Graphics/utils/zorque.ttf");

  _text.setFont(_font);
  _text.setString("R-TYPE");
  _text.setCharacterSize(84);
  _text.setColor(sf::Color::White);
  _text.setStyle(sf::Text::Bold);
  sf::FloatRect t = _text.getLocalBounds();
  _text.setOrigin(t.left + t.width / 2.0f, t.top + t.height / 2.0f);
  _text.setPosition(Dimensions.x / 2.0f, Dimensions.y / 2.0f - 200);
  Starfield s(Dimensions.x, Dimensions.y, 100);
  _stars = s;
}

bool MainView::Update(sf::RenderWindow &window) {

  ImGui::Begin("Menu");
  ImGui::InputText("Name", _name, 255);
  ImGui::InputText("Ip / port", _ipPort, 255);
  if (ImGui::Button("Connect")) {
  }
  if (ImGui::Button("Exit Game")) {
    window.close();
  }
  ImGui::End();

  _starsTexture.loadFromImage(_starsImage);
  _stars.updateStar();
  _stars.drawStar(_starsTexture);

  window.clear(sf::Color(0, 0, 0));

  window.draw(_starsSprite);
  window.draw(_text);
  ImGui::SFML::Render(window);
  window.display();
  return true;
}

Starfield MainView::getStarfield() { return _stars; }
