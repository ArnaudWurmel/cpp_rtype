//
// Created by Arnaud WURMEL on 12/01/2018.
//

#include "RoomListViewController.hh"

rtp::RoomListViewController::RoomListViewController(RootViewController& delegate) : _delegate(delegate) {}

bool    rtp::RoomListViewController::render() {
    return true;
}

rtp::RoomListViewController::~RoomListViewController() {}