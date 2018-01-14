cmake_minimum_required(VERSION 3.5.1)
project(ServerGame)

set(CMAKE_CXX_STANDARD 14)



add_executable(ServerGame main.cpp NetworkAbstract/Message/endpacked.h NetworkAbstract/Message/Message.cpp NetworkAbstract/Message/Message.h NetworkAbstract/Message/packed.h NetworkAbstract/IAcceptor.cpp NetworkAbstract/IAcceptor.hh NetworkAbstract/ISocket.cpp NetworkAbstract/ISocket.h NetworkAbstract/BoostSocket.h NetworkAbstract/BoostSocket.cpp NetworkAbstract/BoostAcceptor.hh NetworkAbstract/BoostAcceptor.cpp NetworkAbstract/SocketReceiver.cpp NetworkAbstract/SocketReceiver.hh ArgumentLoader/ArgumentLoader.cpp ArgumentLoader/ArgumentLoader.hh Logger/Logger.hpp Logger/LoggerHelper.cpp Logger/LoggerHelper.hh ArgumentLoader/Configure.cpp ArgumentLoader/Configure.hh GameServer/GameServer.cpp GameServer/GameServer.hh NetworkAbstract/BoostUdpServer.hpp GameServer/APlayer.hh GameServer/APlayer.cpp GameServer/APlayer.hh NetworkAbstract/BoostUdpClient.hpp NetworkAbstract/IServer.hh)
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.65.0 REQUIRED COMPONENTS thread system)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(ServerGame ${Boost_LIBRARIES})
endif()

target_link_libraries(ServerGame pthread)
