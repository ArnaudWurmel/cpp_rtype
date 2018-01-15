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
        ADrawableEntity(std::string const& spritePath, int nbFrames, int currentFrame, int x, int y, int width, int height, std::string const& title = "");
        virtual ~ADrawableEntity();

    public:
        bool    init();
        void    render();

    private:
        sf::Text    getTextFromTitle() const;

    private:
        std::string _spritePath;
        int _nbFrames;
        int _currentFrame;
        int _x;
        int _y;
        int _width;
        int _height;
        std::string _title;

    private:
        std::vector<sf::IntRect>    _frameList;
        sf::Image   _spriteImage;
        sf::RenderTexture   _renderTexture;
        sf::Font    _font;
    };
}

#endif //CLIENTNETWORK_ADRAWABLEENTITY_HH
