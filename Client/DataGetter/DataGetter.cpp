//
// Created by Arnaud WURMEL on 11/01/2018.
//

#include "DataGetter.hh"
#include "../NetworkAbstract/BoostAcceptor.hh"
#include "../Exceptions/Exception.hh"

rtp::DataGetter::DataGetter() {
    reset();
}

void    rtp::DataGetter::reset() {
    _acceptor = std::make_unique<NetworkAbstract::BoostAcceptor>(_awaker);
    _controlSocket = _acceptor->getEmptySocket(NetworkAbstract::ISocketManager::TCP, _awaker);
}

void    rtp::DataGetter::emptyMessage(Emptier emptier) {
    while (_controlSocket->haveAvailableData()) {
        NetworkAbstract::Message    message = _controlSocket->getAvailableMessage();

        emptier(message);
    }
}

bool    rtp::DataGetter::joinRoom(int roomId) {
    NetworkAbstract::Message    message;

    message.setType(Command::JoinRoom);
    message.setBody(std::to_string(roomId).c_str(), std::to_string(roomId).length());
    return executeCommand(message, &rtp::DataGetter::handleJoinRoom);
}

bool    rtp::DataGetter::executeCommand(NetworkAbstract::Message const& command, Callback callback) {
    int maxWaiting = 50;

    _controlSocket->write(command);
    while (maxWaiting > 0 && _controlSocket->isOpen()) {
        std::unique_lock<std::mutex>    lck(_awakerLock);

        _awaker.wait_for(lck, std::chrono::milliseconds(200));
        if (_controlSocket->haveAvailableData()) {
            NetworkAbstract::Message    message = _controlSocket->getAvailableMessage();

            if (message.getType() == command.getType()) {
                return (this->*callback)(message);
            }
            _controlSocket->addMessage(message);
        }
        --maxWaiting;
    }
    throw rtp::NetworkException();
}

bool    rtp::DataGetter::leaveRoom() {
    NetworkAbstract::Message    message;

    message.setType(Command::LeaveRoom);
    return executeCommand(message, &rtp::DataGetter::handleLeaveRoom);
}

bool    rtp::DataGetter::startMatchmaking() {
    NetworkAbstract::Message    message;

    message.setType(Command::StartMatchmaking);
    return executeCommand(message, &rtp::DataGetter::handleStartMatchmaking);
}

bool    rtp::DataGetter::stopMatchmaking() {
    NetworkAbstract::Message    message;

    message.setType(Command::StopMatchmaking);
    return executeCommand(message, &rtp::DataGetter::handleStopMatchmaking);
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
    }
    if (_controlSocket->connectSocket(ip, port)) {
        _acceptor->run();
        return true;
    }
    return false;
}

bool    rtp::DataGetter::updateRoomList() {
    NetworkAbstract::Message    message;

    message.setType(Command::RoomList);
    return executeCommand(message, &rtp::DataGetter::handleRoomList);
}

bool    rtp::DataGetter::handleStartMatchmaking(NetworkAbstract::Message const &) {
    return true;
}

bool    rtp::DataGetter::handleStopMatchmaking(NetworkAbstract::Message const &) {
    return true;
}

int    rtp::DataGetter::createRoom() {
    NetworkAbstract::Message    message;

    message.setType(Command::CreateRoom);
    if (executeCommand(message, &rtp::DataGetter::handleCreateRoom)) {
        return _roomId;
    }
    return -1;
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

bool    rtp::DataGetter::handleRoomList(NetworkAbstract::Message const& response) {
    _roomList.clear();
    if (response.getBodySize() == 0) {
        return true;
    }
    std::string content = std::string(response.getBody(), response.getBodySize());
    std::vector<std::string>    roomList = getTokenFrom(content, ';');
    auto    iterator = roomList.begin();

    while (iterator != roomList.end()) {
        _roomList.push_back(std::unique_ptr<Room>(new Room()));
        if (!_roomList.back()->init(*iterator)) {
            _roomList.pop_back();
        }
        ++iterator;
    }
    return true;
}

bool    rtp::DataGetter::handleLeaveRoom(NetworkAbstract::Message const &) {
    return true;
}

bool    rtp::DataGetter::handleCreateRoom(NetworkAbstract::Message const& response) {
    if (response.getBodySize() == 0) {
        return false;
    }
    _roomId = std::stoi(std::string(response.getBody(), response.getBodySize()));
    return _roomId != -1;
}

bool    rtp::DataGetter::handleJoinRoom(NetworkAbstract::Message const& response) {
    return std::string(response.getBody(), response.getBodySize()).compare("success") == 0;
}

std::string const&  rtp::DataGetter::getPseudo() const {
    return _pseudo;
}

std::vector<std::string>    rtp::DataGetter::getTokenFrom(std::string const& input, char sep) {
    std::vector<std::string>    tokenList;
    std::string::const_iterator   it;

    it = input.begin();
    while (it != input.end()) {
        std::string token;
        while (it != input.end() && *it != sep) {
            token += *it;
            ++it;
        }
        if (token.size())
            tokenList.push_back(token);
        while (it != input.end() && *it == sep)
            ++it;
    }
    return tokenList;
}

std::vector<std::unique_ptr<rtp::Room> > const&  rtp::DataGetter::getRoomList() const {
    return _roomList;
}

std::shared_ptr<NetworkAbstract::ISocket>   rtp::DataGetter::getUdpSocket(std::condition_variable& cv) {
    return _acceptor->getEmptySocket(NetworkAbstract::ISocketManager::UDP, cv);
}

bool    rtp::DataGetter::authorizeClient(std::shared_ptr<NetworkAbstract::ISocket> socket, EmptierFrom emptier, std::string const& pseudo, std::string const& authToken) {
    NetworkAbstract::Message    message;

    message.setType(GameHandler::Command::AUTHORIZE);
    std::string body = pseudo + " " + authToken;
    message.setBody(body.c_str(), body.length());
    try {
        return waitCommandExecution(socket, message, emptier);
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

rtp::DataGetter::~DataGetter() = default;