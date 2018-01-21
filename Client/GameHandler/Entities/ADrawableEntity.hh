//
// Created by Arnaud WURMEL on 15/01/2018.
//

#ifndef CLIENTNETWORK_ADRAWABLEENTITY_HH
#define CLIENTNETWORK_ADRAWABLEENTITY_HH

#include <SFML/Graphics/Sprite.hpp>
#include <string>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics.hpp>

namespace rtp {
    class   ADrawableEntity : public sf::Sprite {
    public:
        ADrawableEntity(std::string const& spritePath, int currentFrame, int x, int y, std::string const& title = "", int rotation = 0);
        virtual ~ADrawableEntity();

    public:
        bool    init();
        void    render();
        bool    shouldDelete() const;
        void    deleteEntity();
        bool    isInit() const;

    protected:
        void    parseFrame(std::vector<std::string> const&);

    private:
        sf::Text    getTextFromTitle() const;

    private:
        std::string _spritePath;
        std::string _title;

    protected:
        std::vector<sf::IntRect>    _frameList;

    protected:
        int _x;
        int _y;
        int _currentFrame;
        int _rotation;
        std::mutex  _renderLock;
        bool    _deleted;
        bool    _isInit;

    private:
        sf::Image   _spriteImage;
        sf::RenderTexture   _renderTexture;
        sf::Font    _font;
        sf::Texture _spriteTexture;
        sf::Sprite  _tmpSprite;
    };
}

#endif //CLIENTNETWORK_ADRAWABLEENTITY_HH
