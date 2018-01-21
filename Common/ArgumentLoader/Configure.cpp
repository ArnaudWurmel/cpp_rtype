//
// Created by wurmel_a on 1/8/18.
//

#include <iostream>
#include "Configure.hh"
#ifdef _WIN32
#include "../LoggerWin/LoggerHelper.hh"
#else
#include "../Logger/LoggerHelper.hh"
#endif // _WIN32

void    rtp::Configure::configure(std::vector<ArgumentLoader::Argument> const& argumentList) {
    Configure   configure;

    configure.makeConfiguration(argumentList);
}

rtp::Configure::Configure() {
    _functionPtr.insert(std::make_pair("-d", std::bind(&rtp::Configure::configureDebug, this, std::placeholders::_1)));
}

void    rtp::Configure::makeConfiguration(std::vector<ArgumentLoader::Argument> const& argumentList) {
    auto    iterator = argumentList.begin();

    while (iterator != argumentList.end()) {
        if (_functionPtr.find((*iterator).argument) != _functionPtr.end()) {
            _functionPtr[(*iterator).argument]((*iterator).argument);
        }
        ++iterator;
    }
}

void    rtp::Configure::configureDebug(std::string const &) {
    LoggerHelper::setDebug(true);
}

rtp::Configure::~Configure() = default;