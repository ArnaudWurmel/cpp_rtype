cmake_minimum_required(VERSION 3.9)
project(ServerGame)

set(CMAKE_CXX_STANDARD 17)

add_executable(ServerGame main.cpp NetworkAbstract/Message/endpacked.h NetworkAbstract/Message/Message.cpp NetworkAbstract/Message/Message.h NetworkAbstract/Message/packed.h NetworkAbstract/IAcceptor.cpp NetworkAbstract/IAcceptor.hh NetworkAbstract/ISocket.cpp NetworkAbstract/ISocket.h NetworkAbstract/LinuxAcceptor.cpp NetworkAbstract/LinuxAcceptor.hh NetworkAbstract/LinuxSocket.cpp NetworkAbstract/LinuxSocket.hh NetworkAbstract/SocketReceiver.cpp NetworkAbstract/SocketReceiver.hh ArgumentLoader/ArgumentLoader.cpp ArgumentLoader/ArgumentLoader.hh Logger/Logger.hpp Logger/LoggerHelper.cpp Logger/LoggerHelper.hh ArgumentLoader/Configure.cpp ArgumentLoader/Configure.hh)