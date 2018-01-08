//
// Created by Arnaud WURMEL on 08/01/2018.
//

#include <string>
#include <iostream>
#include "RTypeManager.hh"

rtp::RTypeManager::RTypeManager(std::shared_ptr<ServerRegister>& serverRegister) {
    _serverRegister = serverRegister;
    _continue = true;
    _functionPtr.insert(std::make_pair("ls", std::bind(&rtp::RTypeManager::listClient, this)));
    _functionPtr.insert(std::make_pair("exit", std::bind(&rtp::RTypeManager::exitServer, this)));
}

bool rtp::RTypeManager::loop() {
    std::string line;

    std::cout << "> ";
    while (_continue && std::getline(std::cin, line)) {
        if (!_serverRegister->isRunning()) {
            return false;
        }
        if (_functionPtr.find(line) != _functionPtr.end()) {
            _functionPtr[line]();
        }
        if (_continue)
            std::cout << "> ";
    }
    _serverRegister->stop();
    return true;
}

void    rtp::RTypeManager::listClient() {

}

void    rtp::RTypeManager::exitServer() {
    _continue = false;
}

rtp::RTypeManager::~RTypeManager() {}