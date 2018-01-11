//
// Created by Arnaud WURMEL on 11/01/2018.
//

#include "DataGetter.hh"
#include "../NetworkAbstract/BoostAcceptor.hh"
#include "../Exceptions/Exception.hh"

rtp::DataGetter::DataGetter() {
    _acceptor = std::make_unique<NetworkAbstract::BoostAcceptor>(_awaker);
    _controlSocket = _acceptor->getEmptySocket(_awaker);
}

bool    rtp::DataGetter::executeCommand(NetworkAbstract::Message const& command, Callback callback) {
    int maxWaiting = 50;

    _controlSocket->write(command);
    while (maxWaiting > 0 && _controlSocket->isOpen()) {
        std::unique_lock<std::mutex>    lck(_awakerLock);

        _awaker.wait_for(lck, std::chrono::milliseconds(200));
        if (_controlSocket->haveAvailableData()) {
            while (_controlSocket->isOpen() && _controlSocket->haveAvailableData()) {
                NetworkAbstract::Message    message = _controlSocket->getAvailableMessage();

                if (message.getType() == command.getType()) {
                    return (this->*callback)(message);
                }
                _controlSocket->addMessage(message);
            }
        }
        else {
            --maxWaiting;
        }
    }
    throw rtp::NetworkException();
}

bool    rtp::DataGetter::setPseudo(std::string const& pseudo) {
    NetworkAbstract::Message    message;

    message.setType(Command::SetPseudo);
    message.setBody(pseudo.c_str(), pseudo.length());
    return executeCommand(message, &rtp::DataGetter::handlePseudoSet);
}

bool    rtp::DataGetter::connectToHost(std::string const &host) {
    unsigned short port = 8888;
    std::string ip = host;

    if (ip.find(":") != std::string::npos) {
        std::string portStr = ip.substr(ip.find(":") + 1, ip.length());
        if (portStr.length() == 0) {
            say("Can't parse host : `" + host + "`");
            return false;
        }
        port = std::stoi(portStr);
        ip = ip.substr(0, ip.find(":"));
        std::cout << ip << ":" << port << std::endl;
    }
    if (_controlSocket->connectSocket(ip, port)) {
        _acceptor->run();
        return true;
    }
    return false;
}

bool    rtp::DataGetter::isRunning() const {
    return _controlSocket->isOpen();
}

bool    rtp::DataGetter::handlePseudoSet(NetworkAbstract::Message const &response) {
    if (response.getBodySize() > 0) {
        _pseudo = std::string(response.getBody(), response.getBodySize());
    }
    return response.getBodySize() > 0;
}

std::string const&  rtp::DataGetter::getPseudo() const {
    return _pseudo;
}

rtp::DataGetter::~DataGetter() = default;