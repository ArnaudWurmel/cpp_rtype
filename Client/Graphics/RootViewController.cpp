//
// Created by Arnaud WURMEL on 11/01/2018.
//

#include "RootViewController.hh"
#include "imgui-SFML.h"
#include "LoginViewController.hh"

rtp::RootViewController::RootViewController() : _window(sf::VideoMode(1920, 1080, 32), "R-TYPE") {
    _window.setFramerateLimit(60);
    ImGui::SFML::Init(_window);
    _stackView.push(std::shared_ptr<AViewController>(new LoginViewController(*this)));
    _starsImage.create(_window.getSize().x, _window.getSize().y, sf::Color::Black);
    _starsTexture.loadFromImage(_starsImage);
    _starsTexture.setSmooth(false);
    _starsSprite.setTexture(_starsTexture);
    _starsSprite.setPosition(0, 0);
    _font.loadFromFile("./ressources/zorque.ttf");
    _text.setFont(_font);
    _text.setString("R-TYPE");
    _text.setCharacterSize(84);
    _text.setColor(sf::Color::White);
    _text.setStyle(sf::Text::Bold);
    sf::FloatRect t = _text.getLocalBounds();
    _text.setOrigin(t.left + t.width / 2.0f, t.top + t.height / 2.0f);
    _text.setPosition(_window.getSize().x / 2.0f, _window.getSize().y / 2.0f - 200);
    Starfield s(_window.getSize().x, _window.getSize().y, 100);
    _stars = s;
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
        _starsTexture.loadFromImage(_starsImage);
        _stars.updateStar();
        _stars.drawStar(_starsTexture);
        _window.clear(sf::Color::Black);
        _window.draw(_starsSprite);
        _window.draw(_text);
        ImGui::SFML::Render(_window);
        _window.display();
    }
}

rtp::DataGetter&    rtp::RootViewController::getDataGetter() {
    return _dataGetter;
}

void    rtp::RootViewController::instanciate(std::shared_ptr<AViewController>& viewController) {
    _stackView.push(viewController);
}

rtp::RootViewController::~RootViewController() {
    ImGui::SFML::Shutdown();
}