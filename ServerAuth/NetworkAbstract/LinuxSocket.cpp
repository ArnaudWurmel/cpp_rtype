//
// Created by Arnaud WURMEL on 09/01/2018.
//

#include <unistd.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include "LinuxSocket.hh"
#include "IAcceptor.hh"

NetworkAbstract::LinuxSocket::LinuxSocket(std::condition_variable& cv, struct sockaddr_in& in, SOCKET socket) : ISocket(cv), _readM(), _in(in) {
    _socket = socket;
    _readHeader = true;
    if (_socket != -1) {
        IAcceptor::newFd(_socket);
    }
}

NetworkAbstract::LinuxSocket::LinuxSocket(std::condition_variable &cv, SOCKET socket) : ISocket(cv), _socket(socket) {}

bool    NetworkAbstract::LinuxSocket::bind(unsigned short port) {
    struct protoent *pe;
    struct sockaddr_in  serv_addr;

    if (_socket != -1) {
        ::close(_socket);
    }
    pe = getprotobyname("TCP");
    if (!pe) {
        return false;
    }
    _socket = socket(AF_INET, SOCK_STREAM, pe->p_proto);
    IAcceptor::newFd(_socket);
    if (_socket == -1) {
        return false;
    }
    std::memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    if (::bind(_socket, reinterpret_cast<struct sockaddr *>(&serv_addr), sizeof(serv_addr)) == -1) {
        return false;
    }
    if (listen(_socket, 10) == -1) {
        return false;
    }
    return true;
}

void    NetworkAbstract::LinuxSocket::close() {
    if (_socket != - 1) {
        IAcceptor::deleteFd(_socket);
        ::close(_socket);
        _socket = -1;
    }
}

bool    NetworkAbstract::LinuxSocket::isOpen() const {
    return _socket != -1;
}

void    NetworkAbstract::LinuxSocket::read() {
    if (_readHeader) {
        ssize_t state = ::read(_socket, _readM.data(), NetworkAbstract::Message::headerSize);
        if (state > 0) {
            if (_readM.decodeHeader()) {
                _readHeader = false;
            }
        }
        else {
            if (errno != EAGAIN) {
                close();
            }
        }
    }
    else {
        ssize_t state = ::read(_socket, _readM.getBody(), _readM.getBodySize());
        if (state > 0) {
            addMessage(_readM);
            _readHeader = true;
        }
        else {
            if (errno != EAGAIN) {
                close();
            }
        }
    }
}

void    NetworkAbstract::LinuxSocket::flushWrite() {
    if (haveSomethingToWrite()) {
        _writingLocker.lock();
        Message message = _writingList.front();
        _writingList.pop();
        _writingLocker.unlock();
        if (message.encodeHeader()) {
            message.encodeData();
            ::write(_socket, message.data(), message.totalSize());
        }
    }
}

bool    NetworkAbstract::LinuxSocket::connectSocket(std::string const& host, unsigned short port) {
    if (_socket != -1) {
        close();
    }
    return true;
}

void    NetworkAbstract::LinuxSocket::startSession() {}

std::string NetworkAbstract::LinuxSocket::getIpAddr() const {
    const struct sockaddr_in* pV4Addr = &_in;
    struct in_addr ipAddr = pV4Addr->sin_addr;
    char str[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &ipAddr, str, INET_ADDRSTRLEN);
    return std::string(str);
}

SOCKET& NetworkAbstract::LinuxSocket::getSocket() {
    return _socket;
}

SOCKET const& NetworkAbstract::LinuxSocket::getSocket() const {
    return _socket;
}

NetworkAbstract::LinuxSocket::~LinuxSocket() {
    close();
}