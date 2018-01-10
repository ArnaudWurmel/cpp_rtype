//
// Created by Arnaud WURMEL on 10/01/2018.
//

#include "ClientRegister.hh"
#include "RegisteredClient.hh"

unsigned int rtp::RegisteredClient::_clientId = 0;

rtp::RegisteredClient::RegisteredClient(std::shared_ptr<NetworkAbstract::ISocket> socket, rtp::ClientRegister& clientRegister) : SocketReceiver(socket), _delegate(clientRegister) {
    _id = rtp::RegisteredClient::_clientId++;
    _commandCallback.insert(std::make_pair(Command::CreateRoom, std::bind(&rtp::RegisteredClient::createRoom, this, std::placeholders::_1)));
    _commandCallback.insert(std::make_pair(Command::LeaveRoom, std::bind(&rtp::RegisteredClient::leaveRoom, this, std::placeholders::_1)));
    _commandCallback.insert(std::make_pair(Command::RoomList, std::bind(&rtp::RegisteredClient::roomList, this, std::placeholders::_1)));
    _commandCallback.insert(std::make_pair(Command::JoinRoom, std::bind(&rtp::RegisteredClient::joinRoom, this, std::placeholders::_1)));
}

bool    rtp::RegisteredClient::handleNewData() {
    while (_socket->haveAvailableData()) {
        NetworkAbstract::Message    message = _socket->getAvailableMessage();

        if (_commandCallback.find(NetworkAbstract::getTypeOf<Command>(message)) != _commandCallback.end()) {
            if (!_commandCallback[NetworkAbstract::getTypeOf<Command>(message)](message)) {
                return false;
            }
        }
    }
    return true;
}

unsigned int    rtp::RegisteredClient::getId() const {
    return _id;
}

bool    rtp::RegisteredClient::createRoom(NetworkAbstract::Message const&) {
    NetworkAbstract::Message    message;

    message.setType(Command::CreateRoom);
    bool state = _delegate.playerCreateRoom(shared_from_this());
    if (state) {
        message.setBody(std::to_string(_delegate.getPlayerRoomId(shared_from_this())).c_str(), std::to_string(_delegate.getPlayerRoomId(shared_from_this())).length());
    }
    _socket->write(message);
    return state;
}

bool    rtp::RegisteredClient::leaveRoom(NetworkAbstract::Message const&) {
    NetworkAbstract::Message    message;

    message.setType(Command::LeaveRoom);
    _delegate.playerLeaveRoom(shared_from_this());
    _socket->write(message);
    return true;
}

bool    rtp::RegisteredClient::roomList(NetworkAbstract::Message const&) {
    NetworkAbstract::Message    message;
    std::string content;

    message.setType(Command::RoomList);
    auto iterator = _delegate.playerAskRoomList().begin();

    while (iterator != _delegate.playerAskRoomList().end()) {
        content = content + std::to_string((*iterator)->getId()) + " " + std::to_string((*iterator)->nbPlayerIn()) + ";";
        ++iterator;
    }
    message.setBody(content.c_str(), content.length());
    _socket->write(message);
    return true;
}

bool    rtp::RegisteredClient::joinRoom(NetworkAbstract::Message const& message) {
    if (message.getBodySize() == 0) {
        return false;
    }
    int roomId = std::stoi(std::string(message.getBody(), message.getBodySize()));
    bool success = _delegate.playerJoinRoom(shared_from_this(), roomId);
    NetworkAbstract::Message    response;

    response.setType(Command::JoinRoom);
    if (success) {
        response.setBody("success", 7);
    }
    else {
        response.setBody("failure", 7);
    }
    _socket->write(response);
    return true;
}

rtp::RegisteredClient::~RegisteredClient() {

}