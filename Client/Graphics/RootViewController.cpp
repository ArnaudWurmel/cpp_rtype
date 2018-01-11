//
// Created by Arnaud WURMEL on 11/01/2018.
//

#include "RootViewController.hh"
#include "imgui-SFML.h"
#include "LoginViewController.hh"

rtp::RootViewController::RootViewController() : _window(sf::VideoMode(1920, 1080, 32), "R-TYPE") {
    _window.setFramerateLimit(60);
    ImGui::SFML::Init(_window);
    _stackView.push(std::unique_ptr<AViewController>(new LoginViewController(*this)));
}


void    rtp::RootViewController::loop() {
    sf::Clock   deltaClock;

    while (!_stackView.empty() && _window.isOpen()) {
        sf::Event   event;

        while (_window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed ||
                    (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                _window.close();
            }
        }
        ImGui::SFML::Update(_window, deltaClock.restart());
        if (!_stackView.top()->render()) {
            _stackView.pop();
        }
        _window.clear(sf::Color::Black);
        ImGui::SFML::Render(_window);
        _window.display();
    }
}

rtp::DataGetter&    rtp::RootViewController::getDataGetter() {
    return _dataGetter;
}

rtp::RootViewController::~RootViewController() {
    ImGui::SFML::Shutdown();
}