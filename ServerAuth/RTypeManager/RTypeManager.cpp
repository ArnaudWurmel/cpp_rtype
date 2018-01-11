//
// Created by Arnaud WURMEL on 08/01/2018.
//

#include <string>
#include <iostream>
#include "RTypeManager.hh"

rtp::RTypeManager::RTypeManager(std::shared_ptr<ServerRegister>& serverRegister, std::shared_ptr<ClientRegister>& clientRegister) {
    _serverRegister = serverRegister;
    _clientRegister = clientRegister;
    _continue = true;
    _functionPtr.insert(std::make_pair("ls", std::bind(&rtp::RTypeManager::listClient, this, std::placeholders::_1)));
    _functionPtr.insert(std::make_pair("exit", std::bind(&rtp::RTypeManager::exitServer, this, std::placeholders::_1)));
    _functionPtr.insert(std::make_pair("help", std::bind(&rtp::RTypeManager::help, this, std::placeholders::_1)));
    _functionPtr.insert(std::make_pair("debug", std::bind(&rtp::RTypeManager::debug, this, std::placeholders::_1)));
    _functionPtr.insert(std::make_pair("disconnectServer", std::bind(&rtp::RTypeManager::disconnectServer, this, std::placeholders::_1)));
}

bool rtp::RTypeManager::loop() {
    std::string line;

    std::cout << "> ";
    while (_continue && std::getline(std::cin, line)) {
        if (!_serverRegister->isRunning() || !_clientRegister->isRunning()) {
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
    _clientRegister->stop();
    return true;
}

void    rtp::RTypeManager::listClient(std::vector<std::string> const&) {
    auto iterator = _serverRegister->getServer().begin();
    int maxId = 2;
    int maxIp = 2;
    int maxState = 5;
    std::string tabLine;

    while (iterator != _serverRegister->getServer().end()) {
        if (std::to_string((*iterator)->getId()).length() > maxId) {
            maxId = std::to_string((*iterator)->getId()).length();
        }
        if ((*iterator)->getIpAddr().length() > maxIp) {
            maxIp = (*iterator)->getIpAddr().length();
        }
        if ((*iterator)->getStateTranslated().length() > maxState) {
            maxState = (*iterator)->getStateTranslated().length();
        }
        ++iterator;
    }
    tabLine = std::string(10 + maxId + maxIp + maxState, '#');
    say(tabLine, false);
    tabLine = "# ";
    tabLine += std::string((maxId - 2) / 2, ' ');
    tabLine += "ID";
    tabLine += std::string((maxId - 2) - ((maxId - 2) / 2), ' ');
    tabLine += " # ";
    tabLine += std::string((maxIp - 2) / 2, ' ');
    tabLine += "IP";
    tabLine += std::string((maxIp - 2) - ((maxIp - 2) / 2), ' ');
    tabLine += " # ";
    tabLine += std::string((maxState - 5) / 2, ' ');
    tabLine += "State";
    tabLine += std::string((maxState - 5) - ((maxState - 5) / 2), ' ');
    tabLine += " #";
    say(tabLine, false);
    tabLine = std::string(10 + maxId + maxIp + maxState, '#');
    say(tabLine, false);
    iterator = _serverRegister->getServer().begin();
    while (iterator != _serverRegister->getServer().end()) {
        tabLine = "# ";
        tabLine += std::string((maxId - std::to_string((*iterator)->getId()).length()) / 2, ' ');
        tabLine += std::to_string((*iterator)->getId());
        tabLine += std::string((maxId - std::to_string((*iterator)->getId()).length()) - ((maxId - std::to_string((*iterator)->getId()).length()) / 2), ' ');
        tabLine += " # ";
        tabLine += std::string((maxIp - (*iterator)->getIpAddr().length()) / 2, ' ');
        tabLine += (*iterator)->getIpAddr();
        tabLine += std::string((maxIp - (*iterator)->getIpAddr().length()) - ((maxIp - (*iterator)->getIpAddr().length()) / 2), ' ');
        tabLine += " # ";
        tabLine += std::string((maxState - (*iterator)->getStateTranslated().length()) / 2, ' ');
        tabLine += (*iterator)->getStateTranslated();
        tabLine += std::string((maxState - (*iterator)->getStateTranslated().length()) - ((maxState - (*iterator)->getStateTranslated().length()) / 2), ' ');
        tabLine += " #";
        say(tabLine, false);
        say(std::string(10 + maxId + maxIp + maxState, '#'), false);
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

void    rtp::RTypeManager::debug(std::vector<std::string> const&) {
    rtp::LoggerHelper::setDebug(!rtp::LoggerHelper::isDebugEnabled());
    if (rtp::LoggerHelper::isDebugEnabled()) {
        say("Debug now enabled", false);
    }
    else {
        say("Debug now disabled", false);
    }
}

void    rtp::RTypeManager::disconnectServer(std::vector<std::string> const& args) {
    if (args.size() < 2) {
        say("Missing serverId argument", false);
        return ;
    }
    auto iterator   = args.begin() + 1;

    while (iterator != args.end()) {
        int serverId = std::stoi(*iterator);
        auto    iteratorServer = _serverRegister->getServer().begin();

        while (iteratorServer != _serverRegister->getServer().end()) {
            if ((*iteratorServer)->getId() == serverId) {
                (*iteratorServer)->close();
            }
            else {
                ++iteratorServer;
            }
        }
        ++iterator;
    }
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