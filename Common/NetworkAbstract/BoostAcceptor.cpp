//
// Created by wurmel_a on 1/8/18.
//

#include <boost/bind.hpp>
#include <thread>
#include "BoostAcceptor.hh"

NetworkAbstract::BoostAcceptor::BoostAcceptor(unsigned short port,
                                              std::condition_variable& notifier) : _endpoint(boost::asio::ip::tcp::v4(), port), _acceptor(_ioService), _notifier(notifier) {
    _acceptor.open(_endpoint.protocol());
    _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    _acceptor.bind(_endpoint);
    _acceptor.listen();
    startAccept();
}

NetworkAbstract::BoostAcceptor::BoostAcceptor(std::condition_variable &notifier) : _acceptor(_ioService), _notifier(notifier) {}

bool    NetworkAbstract::BoostAcceptor::startAccept() {
    std::shared_ptr<BoostSocket>    new_session(new BoostSocket(_ioService, _notifier));

    _acceptor.async_accept(new_session->getSocket(),
                           boost::bind(&NetworkAbstract::BoostAcceptor::handle_accept, this, new_session, boost::asio::placeholders::error));
    return true;
}

void    NetworkAbstract::BoostAcceptor::run() {
    _threadRunner = std::unique_ptr<std::thread>(new std::thread([&]() {
        this->_ioService.run();
    }));
}

bool    NetworkAbstract::BoostAcceptor::isRunning() const {
    return _acceptor.is_open();
}

bool    NetworkAbstract::BoostAcceptor::haveAwaitingClient() {
    _queueLocker.lock();
    bool state = _clientQueue.size() > 1 || (_clientQueue.size() > 0 && _clientQueue.front()->isOpen());
    _queueLocker.unlock();
    return state;
}

std::shared_ptr<NetworkAbstract::ISocket> NetworkAbstract::BoostAcceptor::acceptClient() {
    _queueLocker.lock();
    std::shared_ptr<NetworkAbstract::ISocket> newClient(_clientQueue.front());
    _clientQueue.pop();
    _queueLocker.unlock();
    return newClient;
}

std::shared_ptr<NetworkAbstract::ISocket>   NetworkAbstract::BoostAcceptor::getEmptySocket(std::condition_variable& awaker) {
    return std::shared_ptr<NetworkAbstract::ISocket>(new BoostSocket(_ioService, awaker));
}

void    NetworkAbstract::BoostAcceptor::stop() {
    _ioService.stop();
    if (_threadRunner)
        _threadRunner->join();
}

void    NetworkAbstract::BoostAcceptor::handle_accept(std::shared_ptr<BoostSocket> new_session, const boost::system::error_code &error) {
    if (!error) {
        new_session->startSession();
        _queueLocker.lock();
        _clientQueue.push(new_session);
        _queueLocker.unlock();
        _notifier.notify_one();
    }
    else {
    }
    startAccept();
}

NetworkAbstract::BoostAcceptor::~BoostAcceptor() {
    stop();
}