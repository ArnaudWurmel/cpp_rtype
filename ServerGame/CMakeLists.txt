cmake_minimum_required(VERSION 3.5.1)
project(ServerGame)
set(CMAKE_CXX_STANDARD 14)


include_directories(
        ./GameServer/Entities/
)

IF(WIN32)
	set(ARGUMENT_LOADER	ArgumentLoaderWin)
	set(LOGGER		LoggerWin)
	set(NETWORK_ABSTRACT	NetworkAbstractWin)
	set(EXCEPTIONS		ExceptionsWin)
ELSE(WIN32)
	set(ARGUMENT_LOADER	ArgumentLoader)
	set(LOGGER		Logger)
	set(NETWORK_ABSTRACT	NetworkAbstract)
	set(EXCEPTIONS		Exceptions)
ENDIF(WIN32)

add_executable(
	ServerGame
	main.cpp
	${NETWORK_ABSTRACT}/Message/endpacked.h		${NETWORK_ABSTRACT}/Message/Message.cpp
	${NETWORK_ABSTRACT}/Message/Message.h		${NETWORK_ABSTRACT}/Message/packed.h
	${NETWORK_ABSTRACT}/IAcceptor.cpp		${NETWORK_ABSTRACT}/IAcceptor.hh
	${NETWORK_ABSTRACT}/ISocket.cpp			${NETWORK_ABSTRACT}/ISocket.h
	${NETWORK_ABSTRACT}/BoostSocket.h		${NETWORK_ABSTRACT}/BoostSocket.cpp
	${NETWORK_ABSTRACT}/BoostAcceptor.hh		${NETWORK_ABSTRACT}/BoostAcceptor.cpp
	${NETWORK_ABSTRACT}/SocketReceiver.cpp		${NETWORK_ABSTRACT}/SocketReceiver.hh
	${NETWORK_ABSTRACT}/IServer.hh 			${NETWORK_ABSTRACT}/BoostSocketManager.cpp
	${NETWORK_ABSTRACT}/BoostSocketManager.hh	${NETWORK_ABSTRACT}/ISocketManager.hh
	${NETWORK_ABSTRACT}/BoostUdpInputManager.hpp	${NETWORK_ABSTRACT}/BoostUdpClient.hpp
	${NETWORK_ABSTRACT}/IUdpInputManager.hh		${NETWORK_ABSTRACT}/BoostSocketUdp.cpp
	${NETWORK_ABSTRACT}/BoostSocketUdp.hh
	${ARGUMENT_LOADER}/ArgumentLoader.cpp		${ARGUMENT_LOADER}/ArgumentLoader.hh
	${ARGUMENT_LOADER}/Configure.cpp		${ARGUMENT_LOADER}/Configure.hh
	${LOGGER}/Logger.hpp 	${LOGGER}/LoggerHelper.cpp	${LOGGER}/LoggerHelper.hh
	GameServer/GameServer.cpp GameServer/GameServer.hh
	GameServer/Entities/APlayer.hh GameServer/Entities/APlayer.cpp
	GameServer/Entities/APlayer.hh GameServer/Entities/AEntity.hh
	GameServer/Entities/AEntity.cpp GameServer/Entities/CollideRect.cpp
	GameServer/Entities/CollideRect.hh GameServer/Entities/Vector2.hh
	GameServer/Entities/AEntityShooter.cpp GameServer/Entities/AEntityShooter.hh
	GameServer/Entities/Bullet/ABullet.cpp GameServer/Entities/Bullet/ABullet.hh
	GameServer/Entities/ALivingEntity.cpp GameServer/Entities/ALivingEntity.hh
	GameServer/Entities/Bullet/AllyBullet.cpp GameServer/Entities/Bullet/AllyBullet.hh
	GameServer/Entities/SpawnableEntity.cpp GameServer/Entities/SpawnableEntity.hh
	GameServer/MonsterInstanciater.cpp GameServer/MonsterInstanciater.hh
	GameServer/Entities/AEnemy.hh GameServer/Entities/AEnemy.cpp
	GameServer/DLLManager/ADLLManager.hh GameServer/DLLManager/ADLLManager.cpp
	GameServer/DLLManager/LinuxDLLManager.cpp GameServer/DLLManager/LinuxDLLManager.hh
	GameServer/DLLManager/WindowsDLLManager.cpp GameServer/DLLManager/WindowsDLLManager.hh
	${EXCEPTIONS}/Exception.hh			${EXCEPTIONS}/Exception.cpp
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ServerGame)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.65.0 REQUIRED COMPONENTS thread regex system)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(ServerGame ${Boost_LIBRARIES})
endif()

if(WIN32)
	target_link_libraries(ServerGame pthreadVC2)
else(WIN32)
	target_link_libraries(ServerGame pthread)
endif(WIN32)
