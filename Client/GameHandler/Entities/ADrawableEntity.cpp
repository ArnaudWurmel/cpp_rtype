//
// Created by Arnaud WURMEL on 15/01/2018.
//

#include <SFML/Graphics/Text.hpp>
#include "ADrawableEntity.hh"
#include "../../DataGetter/DataGetter.hh"

rtp::ADrawableEntity::ADrawableEntity(std::string const &spritePath, int currentFrame, int x, int y, const std::string &title) {
    _title = title;
    _spritePath = "ressources/" + spritePath;
    _currentFrame = currentFrame;
    _x = x;
    _y = y;
}

bool    rtp::ADrawableEntity::init() {
    return _spriteImage.loadFromFile(_spritePath) && _font.loadFromFile("ressources/zorque.ttf");
}

void    rtp::ADrawableEntity::render() {
    int width = _frameList[_currentFrame].width;
    if (_frameList[_currentFrame].width < getTextFromTitle().getLocalBounds().width) {
        width = getTextFromTitle().getLocalBounds().width;
    }
    if (!_renderTexture.create(width, _frameList[_currentFrame].height + 20)) {
        return;
    }
    _renderTexture.clear(sf::Color::Transparent);
    sf::Sprite  sprite;
    sf::Texture texture;

    if (!texture.loadFromImage(_spriteImage)) {
        return ;
    }
    sprite.setTexture(texture);
    if (_frameList.size()) {
        sprite.setTextureRect(_frameList[_currentFrame % _frameList.size()]);
    }
    _renderTexture.draw(sprite);
    _renderTexture.draw(getTextFromTitle());
    _renderTexture.display();
    setTexture(_renderTexture.getTexture());
    setPosition(_x, _y);
}

void    rtp::ADrawableEntity::parseFrame(std::vector<std::string> const& frameList) {
    auto iterator = frameList.begin();

    while (iterator != frameList.end()) {
        std::vector<std::string>    frame = DataGetter::getTokenFrom(*iterator, '.');

        if (frame.size() == 4) {
            _frameList.push_back(sf::IntRect(std::stoi(frame[0]), std::stoi(frame[1]), std::stoi(frame[2]), std::stoi(frame[3])));
            std::cout << *iterator << std::endl;
        }
        ++iterator;
    }
}

sf::Text    rtp::ADrawableEntity::getTextFromTitle() const {
    sf::Text    titleText;

    titleText.setCharacterSize(12);
    titleText.setString(_title);
    titleText.setColor(sf::Color::White);
    titleText.setFont(_font);
    std::cout << titleText.getGlobalBounds().width << " " << titleText.getLocalBounds().height << " " << _title << std::endl;
    titleText.setPosition((_frameList[_currentFrame].width - titleText.getLocalBounds().width) / 2, _renderTexture.getSize().y - 20);
    return titleText;
}

rtp::ADrawableEntity::~ADrawableEntity() = default;