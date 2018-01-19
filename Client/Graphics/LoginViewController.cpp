//
// Created by Arnaud WURMEL on 11/01/2018.
//

#include <cstring>
#include "LoginViewController.hh"
#include "imgui.h"
#include "imgui-SFML.h"
#include "RoomListViewController.hh"
#include "../DataGetter/DataGetter.hh"

rtp::LoginViewController::LoginViewController(rtp::RootViewController& delegate) : _rootViewController(delegate) {
    _continue = true;
    std::memset(_name, 0, sizeof(_name));
    std::memset(_host, 0, sizeof(_host));
}

bool    rtp::LoginViewController::render() {
    ImGui::Begin("LoginViewController");
    ImGui::InputText("Name", _name, 255);
    ImGui::InputText("Ip / port", _host, 255);
    if (ImGui::Button("Connect")) {
        try {
            if (_rootViewController.getDataGetter().connectToHost(_host) && _rootViewController.getDataGetter().setPseudo(_name)) {
                std::shared_ptr<AViewController>    viewController(new RoomListViewController(_rootViewController));
                
                _rootViewController.instanciate(viewController);
            }
            else {
                std::memset(_name, 0, 255);
                std::memset(_host, 0, 255);
            }
        }
        catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    if (ImGui::Button("Exit Game")) {
        _continue = false;
    }
    ImGui::End();
    return _continue;
}

void    rtp::LoginViewController::viewDidReappear() {
    _rootViewController.getDataGetter().reset();
}

std::vector<rtp::DataGetter::Command>   rtp::LoginViewController::getCommandObserver() const {
    return std::vector<rtp::DataGetter::Command>();
}

void    rtp::LoginViewController::handleInput(NetworkAbstract::Message const &) {}

rtp::LoginViewController::~LoginViewController() = default;
