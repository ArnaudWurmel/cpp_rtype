//
// Created by Arnaud WURMEL on 15/01/2018.
//

#include <SFML/Graphics/Text.hpp>
#include "ADrawableEntity.hh"
#include "../../DataGetter/DataGetter.hh"

rtp::ADrawableEntity::ADrawableEntity(std::string const &spritePath, int currentFrame, int x, int y, const std::string &title, int rotation) {
    _title = title;
    _spritePath = "ressources/" + spritePath;
    _currentFrame = currentFrame;
    _x = x;
    _y = y;
    _rotation = rotation;
    _isInit = false;
}

bool    rtp::ADrawableEntity::isInit() const {
    return _isInit;
}

bool    rtp::ADrawableEntity::init() {
    bool state = _spriteImage.loadFromFile(_spritePath) && _font.loadFromFile("ressources/zorque.ttf") && _spriteTexture.loadFromImage(_spriteImage);

    if (state) {
        int maxWidth = _frameList[0].width;
        auto iterator = _frameList.begin();

        while (iterator != _frameList.end()) {
            if ((*iterator).width > maxWidth) {
                maxWidth = (*iterator).width;
            }
            ++iterator;
        }
        if (!_renderTexture.create(maxWidth, _frameList[_currentFrame].height + 20)) {
            return false;
        }
        _tmpSprite.setTexture(_spriteTexture);
    }
    _isInit = state;
    return state;
}

void    rtp::ADrawableEntity::render() {
    _renderLock.lock();
    _renderTexture.clear(sf::Color::Transparent);

    if (_frameList.size()) {
        _tmpSprite.setTextureRect(_frameList[_currentFrame % _frameList.size()]);
    }
    sf::Transform transform;

    _tmpSprite.setPosition((_renderTexture.getSize().x - _tmpSprite.getLocalBounds().width) / 2, 0);
    transform.rotate(_rotation, { _tmpSprite.getLocalBounds().width / 2.f, _tmpSprite.getLocalBounds().height / 2.f });
    _renderTexture.draw(_tmpSprite, transform);
    _renderTexture.draw(getTextFromTitle());
    _renderTexture.display();
    setTexture(_renderTexture.getTexture());
    setPosition(_x, _y);
    _renderLock.unlock();
}

void    rtp::ADrawableEntity::parseFrame(std::vector<std::string> const& frameList) {
    auto iterator = frameList.begin();

    while (iterator != frameList.end()) {
        std::vector<std::string>    frame = DataGetter::getTokenFrom(*iterator, '.');

        if (frame.size() == 4) {
            _frameList.push_back(sf::IntRect(std::stoi(frame[0]), std::stoi(frame[1]), std::stoi(frame[2]), std::stoi(frame[3])));
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
    if (_frameList[_currentFrame].width > titleText.getLocalBounds().width) {
        titleText.setPosition((_frameList[_currentFrame].width - titleText.getLocalBounds().width) / 2, _renderTexture.getSize().y - 20);
    }
    else {
        titleText.setPosition(0, _renderTexture.getSize().y - 20);
    }
    return titleText;
}

bool rtp::ADrawableEntity::shouldDelete() const {
    return _deleted;
}

void    rtp::ADrawableEntity::deleteEntity() {
    _deleted = true;
}


rtp::ADrawableEntity::~ADrawableEntity() = default;