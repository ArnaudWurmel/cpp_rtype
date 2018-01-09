//
// Created by Arnaud WURMEL on 09/01/2018.
//

#include <iostream>
#include <sys/select.h>
#include "LinuxAcceptor.hh"

NetworkAbstract::LinuxAcceptor::LinuxAcceptor(std::condition_variable& cv, unsigned short port) : _haveNewClient(cv) {
    _port = port;
    _serverSocket = std::unique_ptr<LinuxSocket>(new LinuxSocket(_clientAwake));
}

bool    NetworkAbstract::LinuxAcceptor::startAccept() {
    return _serverSocket->bind(_port);
}

bool    NetworkAbstract::LinuxAcceptor::haveAwaitingClient() {
    _clientListLocker.lock();
    bool state = _clientList.size() > 0;
    _clientListLocker.unlock();
    return state;
}

std::shared_ptr<NetworkAbstract::ISocket>   NetworkAbstract::LinuxAcceptor::acceptClient() {
    _clientListLocker.lock();
    std::shared_ptr<NetworkAbstract::ISocket>   client = _clientList.front();
    _clientList.pop();
    _clientListLocker.unlock();
    return client;
}

void    NetworkAbstract::LinuxAcceptor::run() {
    _continueRunning = true;
    if (startAccept()) {
        _threadAccept = std::unique_ptr<std::thread>(new std::thread(&NetworkAbstract::LinuxAcceptor::acceptLoop, this));
    }
}

void    NetworkAbstract::LinuxAcceptor::acceptLoop() {
    fd_set  rdsk;
    timeval timeval;
    int selectRet;

    FD_ZERO(&rdsk);
    FD_SET(_serverSocket->getSocket(), &rdsk);
    while (_continueRunning) {
        timeval.tv_sec = 5;
        timeval.tv_usec = 0;
        selectRet = select(IAcceptor::getMaxFd() + 1, &rdsk, NULL, NULL, &timeval);
        if (!_continueRunning) {
            return ;
        }
        if (selectRet < 0) {
            std::cerr << "Problem in select()" << std::endl;
            _continueRunning = false;
            return ;
        }
        else if (selectRet) {
            int clientFd;
            struct sockaddr_in  in;
            socklen_t len = sizeof(in);

            clientFd = accept(_serverSocket->getSocket(), (struct sockaddr *)&in, &len);
            _clientListLocker.lock();
            _clientList.push(std::shared_ptr<LinuxSocket>(new LinuxSocket(_clientAwake, in, clientFd)));
            _clientAwake.notify_one();
            _clientListLocker.unlock();
        }
    }
}

void    NetworkAbstract::LinuxAcceptor::stop() {
    if (_continueRunning) {
        _continueRunning = false;
    }
    _continueRunning = false;
    if (_serverSocket->isOpen()) {
        _serverSocket->close();
    }
    if (_threadAccept->joinable())
        _threadAccept->join();
}

NetworkAbstract::LinuxAcceptor::~LinuxAcceptor() {
    stop();
}