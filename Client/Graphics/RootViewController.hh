//
// Created by Arnaud WURMEL on 11/01/2018.
//

#ifndef CLIENTNETWORK_ROOTVIEWCONTROLLER_HH
#define CLIENTNETWORK_ROOTVIEWCONTROLLER_HH

# include <memory>
# include <stack>
# include <SFML/Graphics.hpp>
# include "../DataGetter/DataGetter.hh"
# include "../ArgumentLoader/ArgumentLoader.hh"
# include "AViewController.hh"
# include "StarField.hpp"

namespace rtp {
    class RootViewController {
    public:
        explicit RootViewController();
        ~RootViewController();

    public:
        void    loop();
        rtp::DataGetter&    getDataGetter();
        void    instanciate(std::shared_ptr<AViewController>&);

    private:
        rtp::DataGetter _dataGetter;
        sf::RenderWindow    _window;
        std::stack<std::shared_ptr<rtp::AViewController> >    _stackView;

    private:
        sf::Image _starsImage;
        sf::Texture _starsTexture;
        sf::Sprite _starsSprite;
        sf::Font _font;
        sf::Text _text;
        Starfield _stars;
    };
}

#endif //CLIENTNETWORK_ROOTVIEWCONTROLLER_HH
