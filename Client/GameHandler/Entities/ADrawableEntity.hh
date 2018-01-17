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
        ADrawableEntity(std::string const& spritePath, int currentFrame, int x, int y, std::string const& title = "");
        virtual ~ADrawableEntity();

    public:
        bool    init();
        void    render();

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

    private:
        sf::Image   _spriteImage;
        sf::RenderTexture   _renderTexture;
        sf::Font    _font;
    };
}

#endif //CLIENTNETWORK_ADRAWABLEENTITY_HH
