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

namespace rtp {
    class RootViewController {
    public:
        explicit RootViewController();
        ~RootViewController();

    public:
        void    loop();
        rtp::DataGetter&    getDataGetter();

    private:
        rtp::DataGetter _dataGetter;
        sf::RenderWindow    _window;
        std::stack<std::unique_ptr<rtp::AViewController> >    _stackView;
    };
}

#endif //CLIENTNETWORK_ROOTVIEWCONTROLLER_HH
