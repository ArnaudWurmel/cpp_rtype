#include "GameViewController.hh"

rtp::GameViewController::GameViewController(rtp::RootViewController &delegate)
    : _rootViewController(delegate) {}

bool rtp::GameViewController::render() { return _continue; }

rtp::GameViewController::~GameViewController() = default;
