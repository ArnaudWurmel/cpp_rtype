//
// Created by Arnaud WURMEL on 12/01/2018.
//

#include "imgui.h"
#include "imgui-SFML.h"
#include "RoomListViewController.hh"
#include "WaitingRoomViewController.hh"

rtp::RoomListViewController::RoomListViewController(RootViewController& delegate) : _delegate(delegate) {}

bool    rtp::RoomListViewController::render() {

    if (!_delegate.getDataGetter().updateRoomList()) {
        return false;
    }
    ImGui::Begin("Home");
    createRoomList();
    if (ImGui::Button("Create Room")) {
        int roomId = _delegate.getDataGetter().createRoom();

        if (roomId == -1) {
            ImGui::End();
            return false;
        }
        std::shared_ptr<AViewController>    waitingRoomViewController(new WaitingRoomViewController(_delegate, roomId, true));
        _delegate.instanciate(waitingRoomViewController);
    }
    if (ImGui::Button("Disconnect")) {
        ImGui::End();
        return false;
    }
    ImGui::End();
    return true;
}

void    rtp::RoomListViewController::createRoomList() {
    if (!ImGui::ListBoxHeader("RoomList", _delegate.getDataGetter().getRoomList().size(), 20)) {
        return ;
    }
    for (int i = 0; i < _delegate.getDataGetter().getRoomList().size(); i++)
    {
        std::string roomName = "Room #" + std::to_string(_delegate.getDataGetter().getRoomList()[i]->getId()) + " " + std::to_string(_delegate.getDataGetter().getRoomList()[i]->getPlayerList().size()) + "/4";
        ImGui::PushID(i);
        if (ImGui::Selectable(roomName.c_str(), false)) {
            std::cout << "Selected " << i << std::endl;
        }
        ImGui::PopID();
    }
    ImGui::ListBoxFooter();
}

void    rtp::RoomListViewController::viewDidReappear() {}

std::vector<rtp::DataGetter::Command>    rtp::RoomListViewController::getCommandObserver() const {
    return std::vector<rtp::DataGetter::Command>();
}

void    rtp::RoomListViewController::handleInput(NetworkAbstract::Message const &) {}

rtp::RoomListViewController::~RoomListViewController() {}