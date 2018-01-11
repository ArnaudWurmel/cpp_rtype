//
// Created by Arnaud WURMEL on 11/01/2018.
//

#include "LoginViewController.hh"
#include "imgui.h"
#include "imgui-SFML.h"

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
            std::cout << _rootViewController.getDataGetter().connectToHost(_host) << std::endl;
            std::cout << _rootViewController.getDataGetter().setPseudo(_name) << std::endl;
/*            if (_rootViewController.getDataGetter().connectToHost(_host) && _rootViewController.getDataGetter().setPseudo(_name)) {
                std::cout << "Connected" << std::endl;
            }*/
        }
        catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
        std::memset(_name, 0, 255);
        std::memset(_host, 0, 255);
    }
    if (ImGui::Button("Exit Game")) {
        _continue = false;
    }
    ImGui::End();
    return _continue;
}

rtp::LoginViewController::~LoginViewController() = default;