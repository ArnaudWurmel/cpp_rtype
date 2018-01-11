//
// Created by Arnaud WURMEL on 11/01/2018.
//

#include "LoginViewController.hh"
#include "imgui.h"
#include "imgui-SFML.h"

rtp::LoginViewController::LoginViewController(rtp::RootViewController& delegate) : _rootViewController(delegate) {
    _continue = true;
}

bool    rtp::LoginViewController::render() {
    ImGui::Begin("LoginViewController");
    ImGui::InputText("Name", _name, 255);
    ImGui::InputText("Ip / port", _host, 255);
    if (ImGui::Button("Connect")) {
    }
    if (ImGui::Button("Exit Game")) {
        _continue = false;
    }
    ImGui::End();
    return _continue;
}

rtp::LoginViewController::~LoginViewController() = default;