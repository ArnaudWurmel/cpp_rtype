//
// Created by Arnaud WURMEL on 12/01/2018.
//

#include "WaitingRoomViewController.hh"

rtp::WaitingRoomViewController::WaitingRoomViewController(RootViewController& delegate, int roomId, bool isOwner) : _delegate(delegate) {
    _roomId = roomId;
    _isOwner = isOwner;
}

bool    rtp::WaitingRoomViewController::render() {
    return true;
}

void    rtp::WaitingRoomViewController::viewDidReappear() {}

rtp::WaitingRoomViewController::~WaitingRoomViewController() = default;