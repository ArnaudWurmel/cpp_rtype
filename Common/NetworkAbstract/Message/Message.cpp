//
// Created by wurmel_a on 25/09/17.
//

#include <cstdio>
#include <cstring>
#include <iostream>
#include "Message.h"

NetworkAbstract::Message::Message() : _data(headerSize + maxBodySize + 1, 0) {
    std::memset(&_message, 0, sizeof(_message));
    _message.magicNumber = magic_number;
    _message.body = nullptr;
    _message.bodySize = 0;
    _message.type = -1;
}

NetworkAbstract::Message::Message(Message const&other) : _data(headerSize + maxBodySize + 1) {
    _message.magicNumber = other._message.magicNumber;
    _message.bodySize = other._message.bodySize;
    _message.type = other._message.type;
    _message.body = new char[_message.bodySize];
    std::memcpy(_message.body, other._message.body, _message.bodySize);
}

char const* NetworkAbstract::Message::getBody() const {
    return _message.body;
}

char    *NetworkAbstract::Message::getBody() {
    return _message.body;
}

void    NetworkAbstract::Message::setBody(const char *body, unsigned int bodySize) {
    if (_message.body) {
        delete[] _message.body;
    }
    _message.bodySize = bodySize;
    _message.body = new char[bodySize];
    std::memcpy(_message.body, body, bodySize);
}

void    *NetworkAbstract::Message::data() {
    return _data.data();
}

unsigned int    NetworkAbstract::Message::getBodySize() const {
    return _message.bodySize;
}

bool    NetworkAbstract::Message::decodeHeader() {
  std::memcpy(&_message, _data.data(), headerSize);
    if (_message.bodySize > maxBodySize) {
        return false;
    }
    if (_message.body) {
        delete[] _message.body;
    }
    _message.body = new char[_message.bodySize];
    std::memset(_message.body, 0, _message.bodySize);
    return true;
}

void    NetworkAbstract::Message::setType(int type) {
    _message.type = type;
}

void    NetworkAbstract::Message::encodeData() {
    if (_message.bodySize >= maxBodySize) {
        _message.bodySize = maxBodySize - 1;
    }
    std::memcpy(_data.data() + headerSize, _message.body, _message.bodySize);
}

bool    NetworkAbstract::Message::encodeHeader() {
    if (_message.bodySize < maxBodySize)
        std::memcpy(_data.data(), &_message, headerSize);
    return _message.bodySize < maxBodySize;
}

int NetworkAbstract::Message::getType() const {
    return _message.type;
}

unsigned int    NetworkAbstract::Message::totalSize() const {
    return getBodySize() + headerSize;
}

NetworkAbstract::Message::~Message() {
    if (_message.body) {
        delete[] _message.body;
    }
}
