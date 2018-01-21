cmake_minimum_required(VERSION 3.5.1)
project(ServerAuth)
set(CMAKE_CXX_STANDARD 14)


IF(WIN32)
	execute_process(COMMAND "symlinks.bat")
ELSE(WIN32)
	execute_process(COMMAND bash "symlinks.sh")
ENDIF(WIN32)

add_executable(
		ServerAuth
		main.cpp
		ServerRegister/ServerRegister.cpp ServerRegister/ServerRegister.hh
		ServerRegister/RegisteredServer.cpp ServerRegister/RegisteredServer.hh
		ServerRegister/IServerRegister.hh
		Logger/Logger.hpp Logger/LoggerHelper.cpp
		Logger/LoggerHelper.hh
		ArgumentLoader/ArgumentLoader.cpp	ArgumentLoader/ArgumentLoader.hh
		ArgumentLoader/Configure.cpp		ArgumentLoader/Configure.hh
		NetworkAbstract/IAcceptor.hh		NetworkAbstract/Message/Message.cpp
		NetworkAbstract/Message/Message.h	NetworkAbstract/Message/endpacked.h
		NetworkAbstract/Message/packed.h	NetworkAbstract/ISocket.cpp
		NetworkAbstract/ISocket.h		NetworkAbstract/SocketReceiver.cpp
		NetworkAbstract/SocketReceiver.hh	NetworkAbstract/IAcceptor.cpp
		NetworkAbstract/BoostAcceptor.cpp	NetworkAbstract/BoostAcceptor.hh
		NetworkAbstract/BoostSocket.cpp		NetworkAbstract/BoostSocket.h
		NetworkAbstract/BoostSocketUdp.hh	NetworkAbstract/BoostSocketUdp.cpp
		RTypeManager/RTypeManager.cpp RTypeManager/RTypeManager.hh 
		BaseServer/BaseServer.cpp BaseServer/BaseServer.hh
		ClientRegister/ClientRegister.cpp ClientRegister/ClientRegister.hh
		ClientRegister/RegisteredClient.cpp ClientRegister/RegisteredClient.hh
		ClientRegister/Room.cpp ClientRegister/Room.hh
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ServerAuth)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.58.0 REQUIRED COMPONENTS thread regex system)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(ServerAuth ${Boost_LIBRARIES})
endif()
