//
// Created by Arnaud WURMEL on 15/01/2018.
//

#include <SFML/Graphics/Text.hpp>
#include "ADrawableEntity.hh"

rtp::ADrawableEntity::ADrawableEntity(std::string const &spritePath, int nbFrames, int currentFrame, int x, int y,
                                      int width, int height, const std::string &title) {
    _title = title;
    _spritePath = spritePath;
    _nbFrames = nbFrames;
    _currentFrame = currentFrame;
    _x = x;
    _y = y;
    _width = width;
    if (getTextFromTitle().getLocalBounds().width > _width) {
        _width = getTextFromTitle().getLocalBounds().width;
    }
    _height = height;
}

bool    rtp::ADrawableEntity::init() {
    for (int i = 0; i < _nbFrames; i++) {
        _frameList.push_back(sf::IntRect(_x * i, 0, _width, _height));
    }
    return _spriteImage.loadFromFile(_spritePath) && _renderTexture.create(_width, _height + 50) && _font.loadFromFile("ressources/zorque.ttf");
}

void    rtp::ADrawableEntity::render() {
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
    setTexture(_renderTexture.getTexture());
}

sf::Text    rtp::ADrawableEntity::getTextFromTitle() const {
    sf::Text    titleText;

    titleText.setCharacterSize(12);
    titleText.setString(_title);
    titleText.setColor(sf::Color::White);
    titleText.setPosition((_width - titleText.getLocalBounds().width) / 2, _renderTexture.getSize().y - titleText.getLocalBounds().height);
    return titleText;
}

rtp::ADrawableEntity::~ADrawableEntity() = default;