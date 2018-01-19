#include "GameViewController.hh"

rtp::GameViewController::GameViewController(rtp::RootViewController &delegate, std::shared_ptr<NetworkAbstract::ISocket> socket, std::shared_ptr<rtp::Player> player) : _rootViewController(delegate), _gameHandler(socket, player) {
}

bool rtp::GameViewController::render() {
    return _gameHandler.update(_rootViewController.getWindow());
}

std::vector<rtp::DataGetter::Command>   rtp::GameViewController::getCommandObserver() const {
    return std::vector<rtp::DataGetter::Command>();
}

void    rtp::GameViewController::handleInput(NetworkAbstract::Message const &) {}

void    rtp::GameViewController::viewDidReappear() {}

bool    rtp::GameViewController::drawTitle() const {
    return false;
}

rtp::GameViewController::~GameViewController() = default;
