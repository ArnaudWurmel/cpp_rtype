//
// Created by Arnaud WURMEL on 12/01/2018.
//

#include "imgui.h"
#include "imgui-SFML.h"
#include "WaitingRoomViewController.hh"
#include "../GameHandler/Entities/Player.hh"
#include "GameViewController.hh"

rtp::WaitingRoomViewController::WaitingRoomViewController(RootViewController& delegate, int roomId, bool isOwner) : _delegate(delegate) {
    _roomId = roomId;
    _isOwner = isOwner;
    _continue = true;
    _onMatchmaking = false;
    _functionPtrs.insert(std::make_pair(DataGetter::Command::StartMatchmaking, std::bind(&rtp::WaitingRoomViewController::handleStartMatchmaking, this, std::placeholders::_1)));
    _functionPtrs.insert(std::make_pair(DataGetter::Command::StopMatchmaking, std::bind(&rtp::WaitingRoomViewController::handleStopMatchmaking, this, std::placeholders::_1)));
    _functionPtrs.insert(std::make_pair(DataGetter::Command::ServerFound, std::bind(&rtp::WaitingRoomViewController::handleServerFound, this, std::placeholders::_1)));
    _functionPtrs.insert(std::make_pair(DataGetter::Command::NewOwner, std::bind(&rtp::WaitingRoomViewController::handleNewOwner, this, std::placeholders::_1)));
}

bool    rtp::WaitingRoomViewController::render() {
    if (!_delegate.getDataGetter().updateRoomList()) {
        return false;
    }
    ImGui::Begin("Waiting Room");
    createListPlayer();
    if (_isOwner) {
        if (!_onMatchmaking) {
            if (ImGui::Button("Start matchmaking")) {
                if (!_delegate.getDataGetter().startMatchmaking()) {
                    return false;
                }
                _onMatchmaking = true;
            }
        }
        else {
            if (ImGui::Button("Stop matchmaking")) {
                if (!_delegate.getDataGetter().stopMatchmaking()) {
                    return false;
                }
                _onMatchmaking = false;
            }
        }
    }
    else {
        if (!_onMatchmaking) {
            ImGui::Text("%s", "Waiting for players...");
        }
        else {
            ImGui::Text("%s", "Looking for a server...");
        }
    }
    if (ImGui::Button("Quit room")) {
        _delegate.getDataGetter().leaveRoom();
        ImGui::End();
        return false;
    }
    ImGui::End();
    return true;
}

void    rtp::WaitingRoomViewController::createListPlayer() {
    auto iterator = std::find_if(_delegate.getDataGetter().getRoomList().begin(), _delegate.getDataGetter().getRoomList().end(),
                                 [&] (std::unique_ptr<Room> const& room) {
                                     return room->getId() == _roomId;
                                 });
    if (iterator == _delegate.getDataGetter().getRoomList().end()) {
        return ;
    }
    if (!ImGui::ListBoxHeader("Player list", (*iterator)->getPlayerList().size(), 20)) {
        return ;
    }
    for (int i = 0; i < (*iterator)->getPlayerList().size(); i++)
    {
        std::string playerName = (*iterator)->getPlayerList()[i];
        ImGui::PushID(i);
        ImGui::Text("%s", playerName.c_str());
        ImGui::PopID();
    }
    ImGui::ListBoxFooter();
}

void    rtp::WaitingRoomViewController::viewDidReappear() {
    _continue = false;
}

std::vector<rtp::DataGetter::Command>   rtp::WaitingRoomViewController::getCommandObserver() const {
    std::vector<rtp::DataGetter::Command>   observer;

    observer.push_back(DataGetter::Command::NewOwner);
    observer.push_back(DataGetter::Command::StartMatchmaking);
    observer.push_back(DataGetter::Command::StopMatchmaking);
    observer.push_back(DataGetter::Command::ServerFound);
    return observer;
}

void    rtp::WaitingRoomViewController::handleInput(NetworkAbstract::Message const& message) {
    if (_functionPtrs.find(NetworkAbstract::getTypeOf<DataGetter::Command>(message)) != _functionPtrs.end()) {
        _functionPtrs[NetworkAbstract::getTypeOf<DataGetter::Command>(message)](message);
    }
}

void    rtp::WaitingRoomViewController::handleNewOwner(NetworkAbstract::Message const &) {
    _isOwner = true;
}

void    rtp::WaitingRoomViewController::handleStopMatchmaking(NetworkAbstract::Message const &) {
    _onMatchmaking = false;
}

void    rtp::WaitingRoomViewController::handleStartMatchmaking(NetworkAbstract::Message const &) {
    _onMatchmaking = true;
}

void    rtp::WaitingRoomViewController::handleServerFound(NetworkAbstract::Message const& serverMessage) {
    _onMatchmaking = false;
    std::vector<std::string>    authTokenList = rtp::DataGetter::getTokenFrom(std::string(serverMessage.getBody(), serverMessage.getBodySize()), ';');
    std::cout << authTokenList.size() << std::endl;
    if (authTokenList.size() == 2) {
        std::vector<std::string>    serverConnectInfos = rtp::DataGetter::getTokenFrom(authTokenList[0], ':');
        if (serverConnectInfos.size() == 2) {
            std::cout << "Server founded (" << std::string(serverMessage.getBody(), serverMessage.getBodySize()) << ")" << std::endl;
            std::shared_ptr<NetworkAbstract::ISocket>   udpSocket = _delegate.getDataGetter().getEmptyUdpSocket(_gameCv);

            if (udpSocket->connectSocket(serverConnectInfos[0], std::stoi(serverConnectInfos[1]))) {
                DataGetter::authorizeClient(udpSocket, std::bind(&rtp::WaitingRoomViewController::authorizedToPlay, this, std::placeholders::_1, std::placeholders::_2), _delegate.getDataGetter().getPseudo(), authTokenList[1]);
            }
        }
    }
}

void    rtp::WaitingRoomViewController::authorizedToPlay(std::shared_ptr<NetworkAbstract::ISocket> from, NetworkAbstract::Message const& response) {
    std::cout << "On authorized" << std::endl;
    std::cout << std::string(response.getBody(), response.getBodySize()) << std::endl;
    try {
        std::shared_ptr<Player> player = Player::instanciateFromInfo(std::string(response.getBody(), response.getBodySize()));
        std::shared_ptr<AViewController>    viewController(new GameViewController(_delegate, from, player));

        _delegate.instanciate(viewController);
    }
    catch (std::exception& e) {
        std::cerr << "Error : " << e.what() << std::endl;
    }
}

rtp::WaitingRoomViewController::~WaitingRoomViewController() {}