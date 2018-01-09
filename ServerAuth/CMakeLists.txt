cmake_minimum_required(VERSION 3.5.1)
project(ServerAuth)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -g)

add_executable(ServerAuth main.cpp ServerRegister/ServerRegister.cpp ServerRegister/ServerRegister.hh Logger/Logger.hpp Logger/LoggerHelper.cpp Logger/LoggerHelper.hh ArgumentLoader/ArgumentLoader.cpp ArgumentLoader/ArgumentLoader.hh ArgumentLoader/Configure.cpp ArgumentLoader/Configure.hh NetworkAbstract/IAcceptor.hh NetworkAbstract/Message/Message.cpp NetworkAbstract/Message/Message.h NetworkAbstract/Message/endpacked.h NetworkAbstract/Message/packed.h NetworkAbstract/ISocket.cpp NetworkAbstract/ISocket.h RTypeManager/RTypeManager.cpp RTypeManager/RTypeManager.hh ServerRegister/RegisteredServer.cpp ServerRegister/RegisteredServer.hh NetworkAbstract/SocketReceiver.cpp NetworkAbstract/SocketReceiver.hh NetworkAbstract/LinuxAcceptor.cpp NetworkAbstract/LinuxAcceptor.hh NetworkAbstract/LinuxSocket.cpp NetworkAbstract/LinuxSocket.hh NetworkAbstract/IAcceptor.cpp)

#BOOST
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost REQUIRED COMPONENTS thread regex system)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(ServerAuth ${Boost_LIBRARIES})
endif()
