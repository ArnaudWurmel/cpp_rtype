
#include "StarField.hpp"
#include <SFML/Graphics.hpp>

int main() {

  sf::Vector2u screenDimensions(1920, 1080);
  sf::RenderWindow window(
      sf::VideoMode(screenDimensions.x, screenDimensions.y, 32),
      "Game main window");

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

  Starfield stars(screenDimensions.x, screenDimensions.y, 100);

  // Game loop
  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {

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

    starsTexture.loadFromImage(starsImage);
    stars.updateStar();
    stars.drawStar(starsTexture);
    window.clear(sf::Color(0, 0, 0));
    window.draw(starsSprite);
    window.display();
  }

  return 0;
}
