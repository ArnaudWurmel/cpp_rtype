cmake_minimum_required(VERSION 3.5.1)
project(ServerAuth)
set(CMAKE_CXX_STANDARD 14)


IF(WIN32)
	set(ARGUMENT_LOADER	ArgumentLoaderWin)
	set(LOGGER		LoggerWin)
	set(NETWORK_ABSTRACT	NetworkAbstractWin)
ELSE(WIN32)
	set(ARGUMENT_LOADER	ArgumentLoader)
	set(LOGGER		Logger)
	set(NETWORK_ABSTRACT	NetworkAbstract)
ENDIF(WIN32)

add_executable(
		ServerAuth
		main.cpp
		ServerRegister/ServerRegister.cpp ServerRegister/ServerRegister.hh
		ServerRegister/RegisteredServer.cpp ServerRegister/RegisteredServer.hh
		ServerRegister/IServerRegister.hh
		${LOGGER}/Logger.hpp ${LOGGER}/LoggerHelper.cpp
		${LOGGER}/LoggerHelper.hh
		${ARGUMENT_LOADER}/ArgumentLoader.cpp	${ARGUMENT_LOADER}/ArgumentLoader.hh
		${ARGUMENT_LOADER}/Configure.cpp	${ARGUMENT_LOADER}/Configure.hh
		${NETWORK_ABSTRACT}/IAcceptor.hh	${NETWORK_ABSTRACT}/Message/Message.cpp
		${NETWORK_ABSTRACT}/Message/Message.h	${NETWORK_ABSTRACT}/Message/endpacked.h
		${NETWORK_ABSTRACT}/Message/packed.h	${NETWORK_ABSTRACT}/ISocket.cpp
		${NETWORK_ABSTRACT}/ISocket.h		${NETWORK_ABSTRACT}/SocketReceiver.cpp
		${NETWORK_ABSTRACT}/SocketReceiver.hh	${NETWORK_ABSTRACT}/IAcceptor.cpp
		${NETWORK_ABSTRACT}/BoostAcceptor.cpp	${NETWORK_ABSTRACT}/BoostAcceptor.hh
		${NETWORK_ABSTRACT}/BoostSocket.cpp	${NETWORK_ABSTRACT}/BoostSocket.h
		${NETWORK_ABSTRACT}/BoostSocketUdp.hh	${NETWORK_ABSTRACT}/BoostSocketUdp.cpp
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
