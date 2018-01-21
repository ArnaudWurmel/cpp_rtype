//
// Created by Arnaud WURMEL on 18/01/2018.
//

#ifndef SERVERGAME_BOOSTUDPINPUTMANAGER_HH
#define SERVERGAME_BOOSTUDPINPUTMANAGER_HH

# include <boost/asio.hpp>
#include <boost/bind.hpp>
# include "IUdpInputManager.hh"
# include "BoostSocketManager.hh"
# include "BoostUdpClient.hpp"

namespace NetworkAbstract {
    template<typename ClientCallback>
    class BoostUdpInputManager : public IUdpInputManager, public std::enable_shared_from_this<BoostUdpInputManager<ClientCallback> > {
    public:
        explicit BoostUdpInputManager(unsigned short port) : _port(port), _socket(BoostSocketManager::_ioService), _listeningEndpoint(boost::asio::ip::udp::v4(), _port) {
            _acceptClient = false;
        }

        void    initSocket(std::string const& authToken) override {
            _authToken = authToken;
            if (_socket.is_open()) {
                _socket.close();
            }
            _socket.open(boost::asio::ip::udp::v4());
            _socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
            _socket.bind(_listeningEndpoint);
        }

        void    run() override {
            startSession();
        }

        void    stop() override {
            _socket.cancel();
            _acceptedClient.clear();
        }

        void    startAcceptingClient() override {
            _acceptClient = true;
        }

        void    stopAcceptingClient() override {
            _acceptClient = false;
        }

    private:
        void    handleNewData(NetworkAbstract::Message const& message, boost::asio::ip::udp::endpoint const& from) {
            auto    iterator = std::find_if(_acceptedClient.begin(), _acceptedClient.end(), [&](std::shared_ptr<BoostUdpClient<ClientCallback> > const& client) {
                return client->getEndpoint() == from;
            });
            if (iterator == _acceptedClient.end()) {
                if (_acceptClient) {
                    _clientLocker.lock();
                    _acceptedClient.push_back(std::shared_ptr<BoostUdpClient<ClientCallback> >(new BoostUdpClient<ClientCallback>(std::bind(&NetworkAbstract::BoostUdpInputManager<ClientCallback>::authorizeWithToken, this, std::placeholders::_1, std::placeholders::_2), from)));
                    _clientLocker.unlock();
                    iterator = _acceptedClient.end() - 1;
                }
                else {
                    return ;
                }
            }
            (*iterator)->injectInput(message);
            iterator = _acceptedClient.begin();
            while (iterator != _acceptedClient.end()) {
                if ((*iterator)->hasTimedOut()) {
                    _clientLocker.lock();
                    _acceptedClient.erase(iterator);
                    _clientLocker.unlock();
                }
                else {
                    ++iterator;
                }
            }
            _clientLocker.unlock();
        }

        void    startSession() {
            _socket.async_receive_from(boost::asio::buffer(_readM.data(), Message::maxBodySize + Message::headerSize), _clientEndpoint,
                                        boost::bind(&NetworkAbstract::BoostUdpInputManager<ClientCallback>::handleReceiveData, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        }

        void    handleReceiveData(const boost::system::error_code& error, std::size_t bytes_transferred) {
            if (!error) {
                if (_readM.decodeHeader()) {
                    _readM.decodeData();
                    handleNewData(_readM, _clientEndpoint);
                    startSession();
                }
            }
            else {
                std::cout << error.message() << std::endl;
            }
        }

        void    writeToClient(boost::asio::ip::udp::endpoint const& endpoint, NetworkAbstract::Message const& message) {
            _writingLocker.lock();
            bool    onWriting = !_writingList.empty();
            _writingList.push(std::make_pair(endpoint, message));
            _writingList.back().second.encodeHeader();
            _writingList.back().second.encodeData();
            if (!onWriting) {
                _socket.async_send_to(boost::asio::buffer(_writingList.front().second.data(), _writingList.front().second.totalSize()),
                                      _writingList.front().first, boost::bind(&NetworkAbstract::BoostUdpInputManager<ClientCallback>::handleWrited, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
            }
            _writingLocker.unlock();
        }

        void    handleWrited(boost::system::error_code const& error, std::size_t writed) {
            _writingLocker.lock();
            _writingList.pop();
            if (!_writingList.empty()) {
                _socket.async_send_to(boost::asio::buffer(_writingList.front().second.data(), _writingList.front().second.totalSize()),
                                      _writingList.front().first, boost::bind(&NetworkAbstract::BoostUdpInputManager<ClientCallback>::handleWrited, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

            }
            _writingLocker.unlock();
        }

    private:
        void    broadcastToAllClient(NetworkAbstract::Message const& message) {
            auto iterator = _acceptedClient.begin();

            while (iterator != _acceptedClient.end()) {
                writeToClient((*iterator)->getEndpoint(), message);
                ++iterator;
            }
        }

    public:
        bool    haveAcceptedClient() {
            _clientLocker.lock();
            auto iterator = _acceptedClient.begin();

            while (iterator != _acceptedClient.end()) {
                if ((*iterator)->isAuthorized()) {
                    _clientLocker.unlock();
                    return true;
                }
                ++iterator;
            }
            _clientLocker.unlock();
            return false;
        }

        void    sendUpdateForEnemies(std::vector<std::shared_ptr<rtp::AEnemy> >& enemyList) override {
            NetworkAbstract::Message    deleteEntityMessage;
            NetworkAbstract::Message    updateEntityMessage;
            NetworkAbstract::Message    spawnEntityMessage;

            deleteEntityMessage.setType(ClientCallback::Command::DELETE_ENTITY);
            updateEntityMessage.setType(ClientCallback::Command::UPDATE_ENTITY);
            spawnEntityMessage.setType(ClientCallback::Command::SPAWN_ENTITY);
            auto enemy = enemyList.begin();
            while (enemy != enemyList.end()) {
                if (!(*enemy)->isAlive()) {
                    deleteEntityMessage.setBody(std::to_string((*enemy)->getEntityId()).c_str(), std::to_string((*enemy)->getEntityId()).length());
                    broadcastToAllClient(deleteEntityMessage);
                    enemyList.erase(enemy);
                }
                else if (!(*enemy)->isSpawned()) {
                    std::string bodySpawn;

                    *(*enemy).get() >> bodySpawn;
                    spawnEntityMessage.setBody(bodySpawn.c_str(), bodySpawn.length());
                    broadcastToAllClient(spawnEntityMessage);
                    (*enemy)->spawn();
                    ++enemy;
                }
                else {
                    updateEntityMessage.setBody((*enemy)->getInfos().c_str(), (*enemy)->getInfos().length());
                    broadcastToAllClient(updateEntityMessage);
                    ++enemy;
                }
            }

        }

        void    handleCollision(std::vector<std::shared_ptr<rtp::AEnemy> >& enemyList) override {
            _clientLocker.lock();
            auto    iteratorPlayer = _acceptedClient.begin();

            while (iteratorPlayer != _acceptedClient.end()) {
                auto    iteratorBullet = (*iteratorPlayer)->getSubEntities().begin();

                while (iteratorBullet != (*iteratorPlayer)->getSubEntities().end()) {
                    auto    iteratorEnemy = enemyList.begin();

                    while (iteratorEnemy != enemyList.end()) {
                        if ((*iteratorBullet)->collide(*(*iteratorEnemy).get())) {
                            (*iteratorBullet)->onCollide(*(*iteratorEnemy).get());
                            if (!(*iteratorEnemy)->isExpectedToBeDeleted()) {
                                deleteEntity(*(*iteratorEnemy).get());
                            }
                        }
                        ++iteratorEnemy;
                    }
                    ++iteratorBullet;
                }
                auto    iteratorEnemy = enemyList.begin();
                bool    deleted = false;

                while (!deleted && iteratorEnemy != enemyList.end()) {
                    if ((*iteratorPlayer)->collide(*(*iteratorEnemy).get())) {
                        deletePlayer(*iteratorPlayer);
                        deleted = true;
                        _acceptedClient.erase(iteratorPlayer);
                    }
                    ++iteratorEnemy;
                }
                if (!deleted) {
                    ++iteratorPlayer;
                }
            }
            _clientLocker.unlock();
        }

        void    deleteEntity(rtp::AEnemy& e) {
            NetworkAbstract::Message    deleteEntityMessage;

            deleteEntityMessage.setType(ClientCallback::Command::DELETE_ENTITY);
            deleteEntityMessage.setBody(std::to_string(e.getEntityId()).c_str(), std::to_string(e.getEntityId()).length());
            broadcastToAllClient(deleteEntityMessage);
        }

        void    deletePlayer(std::shared_ptr<BoostUdpClient<rtp::APlayer> >& e) {
            NetworkAbstract::Message    deletePlayerMessage;

            deletePlayerMessage.setType(ClientCallback::Command::DELETE_PLAYER);
            deletePlayerMessage.setBody(std::to_string(e->getId()).c_str(), std::to_string(e->getId()).length());
            broadcastToAllClient(deletePlayerMessage);
        }

        void    updateAllPlayer(double diff) override {
            _clientLocker.lock();
            auto iterator = _acceptedClient.begin();

            while (iterator != _acceptedClient.end()) {
                if ((*iterator)->isAuthorized()) {
                    (*iterator)->update(diff);
                }
                ++iterator;
            }
            _clientLocker.unlock();
        }

        void    sendUpdate() override {
            _clientLocker.lock();
            auto iterator = _acceptedClient.begin();

            while (iterator != _acceptedClient.end()) {
                if ((*iterator)->isUpdated() && (*iterator)->isAuthorized()) {
                    NetworkAbstract::Message    updateMessage;
                    std::string body;

                    updateMessage.setType(ClientCallback::Command::UPDATE_PLAYER);
                    body = (*iterator)->getInfos();
                    updateMessage.setBody(body.c_str(), body.length());
                    broadcastToAllClient(updateMessage);
                    (*iterator)->setUpdated(false);
                    auto subEntitiesList = (*iterator)->getNeededUpdateEntities();
                    auto subEntitiesIterator = subEntitiesList.begin();
                    NetworkAbstract::Message    deleteEntityMessage;
                    NetworkAbstract::Message    updateEntityMessage;
                    NetworkAbstract::Message    spawnEntityMessage;

                    deleteEntityMessage.setType(ClientCallback::Command::DELETE_ENTITY);
                    updateEntityMessage.setType(ClientCallback::Command::UPDATE_ENTITY);
                    spawnEntityMessage.setType(ClientCallback::Command::SPAWN_ENTITY);
                    while (subEntitiesIterator != subEntitiesList.end()) {
                        if ((*subEntitiesIterator)->isExpectedToBeDeleted()) {
                            deleteEntityMessage.setBody(std::to_string((*subEntitiesIterator)->getEntityId()).c_str(), std::to_string((*subEntitiesIterator)->getEntityId()).length());
                            broadcastToAllClient(deleteEntityMessage);
                        }
                        else if (!(*subEntitiesIterator)->isSpawned()) {
                            std::string bodySpawn;

                            *(*subEntitiesIterator).get() >> bodySpawn;
                            spawnEntityMessage.setBody(bodySpawn.c_str(), bodySpawn.length());
                            broadcastToAllClient(spawnEntityMessage);
                            (*subEntitiesIterator)->spawn();
                        }
                        else {
                            updateEntityMessage.setBody((*subEntitiesIterator)->getInfos().c_str(), (*subEntitiesIterator)->getInfos().length());
                            broadcastToAllClient(updateEntityMessage);
                        }
                        ++subEntitiesIterator;
                    }
                }
                ++iterator;
            }
            _clientLocker.unlock();
        }

        ~BoostUdpInputManager() override {
        }

    private:
        bool    authorizeWithToken(std::shared_ptr<BoostUdpClient<ClientCallback> > registratedClient, std::string const& token) {
            std::vector<std::string>    tokenList = rtp::GameServer::getTokenFrom(token, ' ');
            NetworkAbstract::Message    reply;

            std::cout << _authToken << std::endl;
            reply.setType(ClientCallback::Command::AUTHORIZE);
            if (_authToken.compare(tokenList[1]) == 0) {
                registratedClient->setPseudo(tokenList[0]);
                NetworkAbstract::Message    newClientMessage;
                std::string newClientBody;

                *(registratedClient.get()) >> newClientBody;
                reply.setBody(newClientBody.c_str(), newClientBody.length());
                writeToClient(registratedClient->getEndpoint(), reply);
                newClientMessage.setType(ClientCallback::Command::SPAWN_PLAYER);
                newClientMessage.setBody(newClientBody.c_str(), newClientBody.length());
                reply.setType(ClientCallback::Command::SPAWN_PLAYER);
                _clientLocker.lock();
                auto iterator = _acceptedClient.begin();
                while (iterator != _acceptedClient.end()) {
                    if ((*iterator)->getEndpoint() != registratedClient->getEndpoint()) {
                        writeToClient((*iterator)->getEndpoint(), newClientMessage);
                        std::string body;

                        *((*iterator).get()) >> body;
                        reply.setBody(body.c_str(), body.length());
                        writeToClient(registratedClient->getEndpoint(), reply);
                    }
                    ++iterator;
                }
                _clientLocker.unlock();
                return true;
            }
            else {
                reply.setBody("failure", 7);
                writeToClient(registratedClient->getEndpoint(), reply);
            }
            return true;
        }

    private:
        unsigned short  _port;
        boost::asio::ip::udp::socket    _socket;
        boost::asio::ip::udp::endpoint  _listeningEndpoint;
        boost::asio::ip::udp::endpoint  _clientEndpoint;

    private:
        std::string _authToken;
        bool    _acceptClient;
        std::vector<std::shared_ptr<BoostUdpClient<ClientCallback> > >   _acceptedClient;
        std::mutex  _clientLocker;

    private:
        std::queue<std::pair<boost::asio::ip::udp::endpoint, NetworkAbstract::Message> >    _writingList;
        std::mutex  _writingLocker;

    private:
        NetworkAbstract::Message    _readM;
    };
}


#endif //SERVERGAME_BOOSTUDPINPUTMANAGER_HH
