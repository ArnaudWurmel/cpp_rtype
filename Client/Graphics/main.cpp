
#include "MainView.hpp"
#include "StarField.hpp"
#include "imgui-SFML.h"
#include "imgui.h"
#include <SFML/Graphics.hpp>
#include <iostream>

void test(sf::RenderWindow &window, sf::Clock &deltaClock, char Name[]) {
  ImGui::SFML::Update(window, deltaClock.restart());
  ImGui::Begin("Menu");
  ImGui::InputText("Name", Name, 255);
  if (ImGui::Button("Enter your name")) {
  }
  if (ImGui::Button("Exit Game")) {
    window.close();
  }
  ImGui::End();
}

int main() {

  sf::Vector2u screenDimensions(1024, 720);
  sf::RenderWindow window(
      sf::VideoMode(screenDimensions.x, screenDimensions.y, 32), "R-TYPE");
  ImGui::SFML::Init(window);
  sf::Clock deltaClock;

  window.setSize(screenDimensions);
  window.setFramerateLimit(60);

  MainView main(screenDimensions);
  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);
      switch (event.type) {
      case sf::Event::Closed:
        window.close();
        break;

      case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::Escape) {
          window.close();
        }
        break;

      default:
        break;
      }
    }
    ImGui::SFML::Update(window, deltaClock.restart());
    main.Update(window);
    ImGui::SFML::Render(window);
    window.display();
  }
  ImGui::SFML::Shutdown();
  return 0;
}
