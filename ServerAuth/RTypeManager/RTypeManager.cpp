//
// Created by Arnaud WURMEL on 08/01/2018.
//

#include <string>
#include <iostream>
#include "RTypeManager.hh"

rtp::RTypeManager::RTypeManager(std::shared_ptr<ServerRegister>& serverRegister) {
    _serverRegister = serverRegister;
    _continue = true;
    _functionPtr.insert(std::make_pair("ls", std::bind(&rtp::RTypeManager::listClient, this, std::placeholders::_1)));
    _functionPtr.insert(std::make_pair("exit", std::bind(&rtp::RTypeManager::exitServer, this, std::placeholders::_1)));
    _functionPtr.insert(std::make_pair("help", std::bind(&rtp::RTypeManager::help, this, std::placeholders::_1)));
}

bool rtp::RTypeManager::loop() {
    std::string line;

    std::cout << "> ";
    while (_continue && std::getline(std::cin, line)) {
        if (!_serverRegister->isRunning()) {
            return false;
        }
        std::vector<std::string>    tokenList = getTokenFrom(line);
        if (tokenList.size() > 0 && _functionPtr.find(tokenList[0]) != _functionPtr.end()) {
            _functionPtr[tokenList[0]](tokenList);
        }
        else if (tokenList.size() > 0) {
            say("Unknown command : `" + tokenList[0] + "`");
        }
        if (_continue)
            std::cout << "> ";
    }
    _serverRegister->stop();
    return true;
}

void    rtp::RTypeManager::listClient(std::vector<std::string> const&) {
    auto iterator = _serverRegister->getServer().begin();
    int maxId = 4;
    int maxIp = 4;
    std::string tabLine;

    while (iterator != _serverRegister->getServer().end()) {
        if (std::to_string((*iterator)->getId()).length() + std::to_string((*iterator)->getId()).length() % 2 > maxId) {
            maxId = std::to_string((*iterator)->getId()).length() + std::to_string((*iterator)->getId()).length() % 2;
        }
        if ((*iterator)->getIpAddr().length() + (*iterator)->getIpAddr().length() % 2 > maxIp) {
            maxIp = (*iterator)->getIpAddr().length() + (*iterator)->getIpAddr().length() % 2;
        }
        ++iterator;
    }
    for (int i = 0; i < maxId + maxIp + 3; i++) {
        tabLine += "#";
    }
    say(tabLine, false);
    tabLine = "# ";
    for (int i = 0; i < (maxId - 4) / 2; i++) {
        tabLine += " ";
    }
    tabLine += "ID";
    for (int i = 0; i < (maxId - 4) / 2; i++) {
        tabLine += " ";
    }
    tabLine += " # ";
    for (int i = 0; i < (maxIp - 4) / 2; i++) {
        tabLine += " ";
    }
    tabLine += "IP";
    for (int i = 0; i < (maxIp - 4) / 2; i++) {
        tabLine += " ";
    }
    say(tabLine + " #", false);
    tabLine = "";
    for (int i = 0; i < maxId + maxIp + 3; i++) {
        tabLine += "#";
    }
    say(tabLine, false);
    iterator = _serverRegister->getServer().begin();
    while (iterator != _serverRegister->getServer().end()) {
        tabLine = "# " + std::to_string((*iterator)->getId()) + " # " + (*iterator)->getIpAddr() + " #";
        say(tabLine, false);
        tabLine = "";
        for (int i = 0; i < maxId + maxIp + 3; i++) {
            tabLine += "#";
        }
        say(tabLine, false);
        ++iterator;
    }
}

void    rtp::RTypeManager::exitServer(std::vector<std::string> const&) {
    _continue = false;
}

void    rtp::RTypeManager::help(std::vector<std::string> const&) {
    auto iterator = _functionPtr.begin();
    std::string printLine;

    while (iterator != _functionPtr.end()) {
        printLine += (*iterator).first;
        ++iterator;
        if (iterator != _functionPtr.end()) {
            printLine += ", ";
        }
    }
    say(printLine, false);
}

std::vector<std::string>    rtp::RTypeManager::getTokenFrom(std::string const& input) {
    std::vector<std::string>    tokenList;
    std::string::const_iterator   it;

    it = input.begin();
    while (it != input.end()) {
        std::string token;
        while (it != input.end() && *it != ' ') {
            token += *it;
            ++it;
        }
        if (token.size())
            tokenList.push_back(token);
        while (it != input.end() && *it == ' ')
            ++it;
    }
    return tokenList;
}

rtp::RTypeManager::~RTypeManager() {}