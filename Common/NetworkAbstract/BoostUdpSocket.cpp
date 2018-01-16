//
// Created by wurmel_a on 27/09/17.
//

#include <iostream>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "BoostUdpSocket.hh"

NetworkAbstract::BoostUdpSocket::BoostUdpSocket(boost::asio::io_service& io_service, std::condition_variable& cv) : ISocket(cv), _socket(io_service), _resolver(io_service) {
    _socket.open(boost::asio::ip::udp::v4());
}

bool    NetworkAbstract::BoostUdpSocket::isOpen() const {
    return _socket.is_open();
}

bool    NetworkAbstract::BoostUdpSocket::connectSocket(std::string const& host, unsigned short port) {
    boost::asio::ip::udp::resolver::query   query(boost::asio::ip::udp::v4(), host, std::to_string(port));
    _serverEndpoint = *_resolver.resolve(query);
    startSession();
    return true;
}

void    NetworkAbstract::BoostUdpSocket::close() {
    if (_socket.is_open()) {
        _socket.cancel();
        _socket.close();
        _cv.notify_one();
    }
}

void    NetworkAbstract::BoostUdpSocket::startSession() {
    _socket.async_receive_from(boost::asio::buffer(_readM.data(), Message::headerSize + Message::maxBodySize), _readedEndpoint,
                            boost::bind(&NetworkAbstract::BoostUdpSocket::handleReadHeader, shared_from_this(),
                                        boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void    NetworkAbstract::BoostUdpSocket::handleReadHeader(const boost::system::error_code &error, std::size_t bytes) {
    if (!error) {
        if (_readM.decodeHeader()) {
            _readM.decodeData();
            addMessage(_readM);
        }
        startSession();
    }
    else {
        close();
    }
}

std::string NetworkAbstract::BoostUdpSocket::getIpAddr() const {
    return _socket.remote_endpoint().address().to_string();
}

void    NetworkAbstract::BoostUdpSocket::write(NetworkAbstract::Message message) {
    _mutex.lock();
    bool    onWriting = !_writeList.empty();

    _writeList.push(message);
    _writeList.back().encodeHeader();
    _writeList.back().encodeData();
    if (!onWriting) {
        _socket.async_send_to(boost::asio::buffer(_writeList.front().data(), _writeList.front().totalSize()), _serverEndpoint,
                                   boost::bind(&NetworkAbstract::BoostUdpSocket::handleWrite, shared_from_this(),
                                               boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        _mutex.unlock();
        return ;
    }
    _mutex.unlock();
}

void    NetworkAbstract::BoostUdpSocket::handleWrite(boost::system::error_code const& e, std::size_t size) {
    std::cout << e.message() << " " << size << std::endl;
    _mutex.lock();
    _writeList.pop();
    if (!_writeList.empty()) {
        _socket.async_send_to(boost::asio::buffer(_writeList.front().data(), _writeList.front().totalSize()), _serverEndpoint,
                              boost::bind(&NetworkAbstract::BoostUdpSocket::handleWrite, shared_from_this(),
                                          boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        _mutex.unlock();
        return ;
    }
    _mutex.unlock();
}

NetworkAbstract::BoostUdpSocket::~BoostUdpSocket() {
    if (isOpen()) {
        close();
    }
}