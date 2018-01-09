//
// Created by wurmel_a on 27/09/17.
//

#include <exception>
#include <iostream>
#include "ISocket.h"

NetworkAbstract::ISocket::ISocket(std::condition_variable& cv) : _cv(cv) {}

bool NetworkAbstract::ISocket::haveAvailableData() {
    _queueLocker.lock();
    bool state = _messageList.size() > 0;
    _queueLocker.unlock();
    return state;
}

void    NetworkAbstract::ISocket::addMessage(NetworkAbstract::Message message) {
    _queueLocker.lock();
    _messageList.push(message);
    _queueLocker.unlock();
    _cv.notify_one();
}

NetworkAbstract::Message  NetworkAbstract::ISocket::getAvailableMessage() {
    _queueLocker.lock();
    if (_messageList.size() > 0) {
        NetworkAbstract::Message message = _messageList.front();
        _messageList.pop();
        _queueLocker.unlock();
        return message;
    }
    throw std::exception();
}

bool    NetworkAbstract::ISocket::haveSomethingToWrite() {
    _writingLocker.lock();
    bool state = _writingList.size() > 0;
    _writingLocker.unlock();
    return state;
}

void    NetworkAbstract::ISocket::write(NetworkAbstract::Message message) {
    _writingLocker.lock();
    _writingList.push(message);
    _writingLocker.unlock();
}

NetworkAbstract::ISocket::~ISocket() {}