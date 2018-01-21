cmake_minimum_required(VERSION 3.5.1)
project(ServerGame)
set(CMAKE_CXX_STANDARD 14)

include_directories(
        ./GameServer/Entities/
)

IF(WIN32)
	execute_process(COMMAND "symlinks.bat")
ELSE(WIN32)
	execute_process(COMMAND bash "symlinks.sh")
ENDIF(WIN32)

add_executable(
	ServerGame
	main.cpp
	NetworkAbstract/Message/endpacked.h		NetworkAbstract/Message/Message.cpp
	NetworkAbstract/Message/Message.h		NetworkAbstract/Message/packed.h
	NetworkAbstract/IAcceptor.cpp			NetworkAbstract/IAcceptor.hh
	NetworkAbstract/ISocket.cpp			NetworkAbstract/ISocket.h
	NetworkAbstract/BoostSocket.h			NetworkAbstract/BoostSocket.cpp
	NetworkAbstract/BoostAcceptor.hh		NetworkAbstract/BoostAcceptor.cpp
	NetworkAbstract/SocketReceiver.cpp		NetworkAbstract/SocketReceiver.hh
	NetworkAbstract/IServer.hh 			NetworkAbstract/BoostSocketManager.cpp
	NetworkAbstract/BoostSocketManager.hh		NetworkAbstract/ISocketManager.hh
	NetworkAbstract/BoostUdpInputManager.hpp	NetworkAbstract/BoostUdpClient.hpp
	NetworkAbstract/IUdpInputManager.hh		NetworkAbstract/BoostSocketUdp.cpp
	NetworkAbstract/BoostSocketUdp.hh
	ArgumentLoader/ArgumentLoader.cpp		ArgumentLoader/ArgumentLoader.hh
	ArgumentLoader/Configure.cpp			ArgumentLoader/Configure.hh
	Logger/Logger.hpp    Logger/LoggerHelper.cpp	Logger/LoggerHelper.hh
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
	Exceptions/Exception.hh			Exceptions/Exception.cpp
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
