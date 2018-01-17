//
// Created by wurmel_a on 27/09/17.
//

#include <iostream>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "BoostSocket.h"

NetworkAbstract::BoostSocket::BoostSocket(boost::asio::io_service& io_service, std::condition_variable& cv) : ISocket(cv), _socket(io_service) {}

bool    NetworkAbstract::BoostSocket::isOpen() const {
    return _socket.is_open();
}

bool    NetworkAbstract::BoostSocket::connectSocket(std::string const& host, unsigned short port) {
    boost::asio::ip::tcp::endpoint  endpoint(boost::asio::ip::address::from_string(host), port);
    boost::system::error_code   code;

    _socket.connect(endpoint, code);
    startSession();
    return !code && _socket.is_open();
}

void    NetworkAbstract::BoostSocket::close() {
    _socket.cancel();
    if (_socket.is_open()) {
        _socket.close();
        _cv.notify_one();
    }
}

boost::asio::ip::tcp::socket&   NetworkAbstract::BoostSocket::getSocket() {
    return _socket;
}

void    NetworkAbstract::BoostSocket::startSession() {
    boost::asio::async_read(_socket,
                            boost::asio::buffer(_readM.data(), Message::headerSize),
                            boost::bind(&NetworkAbstract::BoostSocket::handleReadHeader, shared_from_this(), boost::asio::placeholders::error));
}

void    NetworkAbstract::BoostSocket::handleReadHeader(const boost::system::error_code &error) {
    if (!error) {
        if (_readM.decodeHeader()) {
            boost::asio::async_read(_socket,
                                    boost::asio::buffer((_readM.getBody()), _readM.getBodySize()),
                                    boost::bind(&NetworkAbstract::BoostSocket::handleReadBody, shared_from_this(), boost::asio::placeholders::error));
        }
        else {
            BoostSocket::close();
        }
    }
    else {
        BoostSocket::close();
    }
}

void    NetworkAbstract::BoostSocket::handleReadBody(const boost::system::error_code &error) {
    if (!error) {
        addMessage(_readM);
        startSession();
    }
    else {
        close();
    }
}

std::string NetworkAbstract::BoostSocket::getIpAddr() const {
    return _socket.remote_endpoint().address().to_string();
}

void    NetworkAbstract::BoostSocket::write(NetworkAbstract::Message message) {
    _mutex.lock();
    bool    onWriting = !_writeList.empty();

    _writeList.push(message);
    _writeList.back().encodeHeader();
    _writeList.back().encodeData();
    if (!onWriting) {
        boost::asio::async_write(_socket,
                                 boost::asio::buffer(_writeList.front().data(),
                                                     _writeList.front().totalSize()),
                                 boost::bind(&NetworkAbstract::BoostSocket::handleWrite, shared_from_this(),
                                             boost::asio::placeholders::error));
        _mutex.unlock();
        return ;
    }
    _mutex.unlock();
}

void    NetworkAbstract::BoostSocket::handleWrite(boost::system::error_code const& e) {
    _mutex.lock();
    _writeList.pop();
    if (!_writeList.empty()) {
            boost::asio::async_write(_socket,
                                     boost::asio::buffer(_writeList.front().data(),
                                                         _writeList.front().totalSize()),
                                     boost::bind(&NetworkAbstract::BoostSocket::handleWrite, shared_from_this(),
                                                 boost::asio::placeholders::error));
        _mutex.unlock();
        return ;
    }
    _mutex.unlock();
}

NetworkAbstract::BoostSocket::~BoostSocket() {
    std::cout << "Socket Deleted" << std::endl;
    if (isOpen()) {
        close();
    }
}