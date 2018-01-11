
#include "StarField.hpp"
#include "imgui-SFML.h"
#include "imgui.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {

  sf::Vector2u screenDimensions(1024, 720);
  sf::RenderWindow window(
      sf::VideoMode(screenDimensions.x, screenDimensions.y, 32), "R-TYPE");
  ImGui::SFML::Init(window);
  sf::Clock deltaClock;

  window.setSize(screenDimensions);
  window.setFramerateLimit(60);

  // create an empty black image onto which the starfield will be painted every
  // frame
  sf::Image starsImage;
  starsImage.create(screenDimensions.x, screenDimensions.y, sf::Color::Black);

  sf::Texture starsTexture;
  starsTexture.loadFromImage(starsImage);
  starsTexture.setSmooth(false);

  sf::Sprite starsSprite;
  starsSprite.setTexture(starsTexture);
  starsSprite.setPosition(0, 0);

  sf::Font font;
  font.loadFromFile("./Graphics/utils/zorque.ttf");
  sf::Text text;
  Starfield stars(screenDimensions.x, screenDimensions.y, 100);

  text.setFont(font);
  text.setString("R-TYPE");
  text.setCharacterSize(84);
  text.setColor(sf::Color::White);
  text.setStyle(sf::Text::Bold);

  sf::FloatRect t = text.getLocalBounds();
  text.setOrigin(t.left + t.width / 2.0f, t.top + t.height / 2.0f);

  text.setPosition(
      sf::Vector2f(screenDimensions.x / 2.0f, screenDimensions.y / 2.0f - 200));

  char Name[255] = "Player1";
  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);
      switch (event.type) {
      case sf::Event::Closed:
        window.close();
        break;

      // Keypress related events
      case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::Escape ||
            event.key.code == sf::Keyboard::Return) {
          window.close();
        }
        break;

      default:
        break;
      }
    }
    ImGui::SFML::Update(window, deltaClock.restart());
    ImGui::Begin("Menu"); // begin window
    ImGui::InputText("Name", Name, 255);
    if (ImGui::Button("Enter your name")) {
      // this code gets if user clicks on the button
      // yes, you could have written if(ImGui::InputText(...))
      // but I do this to show how buttons work :)
    }
    if (ImGui::Button("Exit Game")) {
      window.close();
    }
    // Window title text edit
    ImGui::End();
    starsTexture.loadFromImage(starsImage);
    stars.updateStar();
    stars.drawStar(starsTexture);
    window.clear(sf::Color(0, 0, 0));
    window.draw(starsSprite);
    window.draw(text);
    ImGui::SFML::Render(window);
    window.display();
  }
  ImGui::SFML::Shutdown();
  return 0;
}
