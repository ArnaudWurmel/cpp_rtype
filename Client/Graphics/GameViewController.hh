#ifndef GAMEVIEWCONTROLLER_HPP
#define GAMEVIEWCONTROLLER_HPP

#include "AViewController.hh"
#include "RootViewController.hh"
#include "../GameHandler/GameHandler.hh"

namespace rtp {
    class GameViewController : public AViewController {
    public:
        explicit GameViewController(RootViewController &, std::shared_ptr<NetworkAbstract::ISocket>, std::shared_ptr<Player>);
        ~GameViewController() override;

    public:
        bool render() override;
        void viewDidReappear() override;
        std::vector<rtp::DataGetter::Command>   getCommandObserver() const override;
        void    handleInput(NetworkAbstract::Message const&) override;
        bool    drawTitle() const override;

    private:
        RootViewController &_rootViewController;
        bool _continue;

    private:
        GameHandler _gameHandler;
    };
}

#endif
