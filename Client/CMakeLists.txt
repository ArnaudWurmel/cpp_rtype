cmake_minimum_required(VERSION 3.5.1)
project(ClientNetwork)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g")

include_directories(
    ./dependency/imgui/
)

IF(WIN32)
	execute_process(COMMAND "symlinks.bat")
ELSE(WIN32)
	execute_process(COMMAND bash "symlinks.sh")
ENDIF(WIN32)

add_executable(
	ClientNetwork
	main.cpp
	ArgumentLoader/ArgumentLoader.hh	ArgumentLoader/ArgumentLoader.cpp
	ArgumentLoader/Configure.hh		ArgumentLoader/Configure.cpp
	Logger/LoggerHelper.cpp		Logger/Logger.hpp
	Logger/LoggerHelper.cpp 		NetworkAbstract/Message/Message.cpp
	NetworkAbstract/Message/Message.h	NetworkAbstract/BoostSocket.cpp
	NetworkAbstract/BoostAcceptor.cpp	NetworkAbstract/BoostAcceptor.hh
	NetworkAbstract/IAcceptor.hh 	NetworkAbstract/IAcceptor.cpp
	NetworkAbstract/ISocket.cpp		NetworkAbstract/ISocket.h
	NetworkAbstract/SocketReceiver.hh	NetworkAbstract/SocketReceiver.cpp
	NetworkAbstract/BoostUdpSocket.hh	NetworkAbstract/BoostUdpSocket.cpp
	DataGetter/DataGetter.cpp		DataGetter/DataGetter.hh
	Exceptions/Exception.cpp		Exceptions/Exception.hh
	Graphics/RootViewController.cpp 	Graphics/RootViewController.hh
	dependency/imgui/imgui-SFML.cpp		dependency/imgui/imgui.cpp 
	dependency/imgui/imgui_draw.cpp		dependency/imgui/imgui_demo.cpp
	Graphics/LoginViewController.cpp	Graphics/LoginViewController.hh
	Graphics/AViewController.hh		Graphics/RoomListViewController.cpp
	Graphics/RoomListViewController.hh	Graphics/Star.cpp
	Graphics/Star.hpp			Graphics/StarField.cpp
	Graphics/StarField.hpp			DataGetter/Room.cpp
	DataGetter/Room.hh			Graphics/WaitingRoomViewController.cpp
	Graphics/WaitingRoomViewController.hh	Graphics/GameViewController.hh
	Graphics/GameViewController.cpp		GameHandler/GameHandler.cpp
	GameHandler/GameHandler.hh		GameHandler/Entities/Player.cpp
	GameHandler/Entities/Player.hh		GameHandler/Entities/ADrawableEntity.cpp
	GameHandler/Entities/ADrawableEntity.hh Graphics/AViewController.cpp
	GameHandler/Entities/AEntity.cpp	GameHandler/Entities/AEntity.hh)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     OFF)
find_package(Boost 1.58.0 REQUIRED COMPONENTS thread regex system)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(ClientNetwork ${Boost_LIBRARIES})
endif()

if(WIN32)
	target_link_libraries(ClientNetwork pthreadVC2)
else(WIN32)
	target_link_libraries(ClientNetwork pthread)
endif(WIN32)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED audio graphics window system)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(ClientNetwork ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif(SFML_FOUND)

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
	include_directories(${OPENGL_INCLUDE_DIRS})
	target_link_libraries(ClientNetwork ${OPENGL_LIBRARIES})
endif(OPENGL_FOUND)

find_package(GLUT REQUIRED)
if(GLUT_FOUND)
	include_directories(${GLUT_INCLUDE_DIRS})
	target_link_libraries(ClientNetwork ${GLUT_LIBRARY})
endif(GLUT_FOUND)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ClientNetwork)

if(WIN32)
    target_compile_options(
	ClientNetwork PRIVATE 
	"/EHsc"
    )
    set(SFML_AUDIO_DEBUG		$ENV{SFML_ROOT}/bin/sfml-audio-d-2.dll)
    set(SFML_GRAPHICS_DEBUG		$ENV{SFML_ROOT}/bin/sfml-graphics-d-2.dll)
    set(SFML_SYSTEM_DEBUG		$ENV{SFML_ROOT}/bin/sfml-system-d-2.dll)
    set(SFML_WINDOW_DEBUG		$ENV{SFML_ROOT}/bin/sfml-window-d-2.dll)
    set(SFML_AUDIO_RELEASE		$ENV{SFML_ROOT}/bin/sfml-audio-d-2.dll)
    set(SFML_GRAPHICS_RELEASE		$ENV{SFML_ROOT}/bin/sfml-graphics-2.dll)
    set(SFML_SYSTEM_RELEASE		$ENV{SFML_ROOT}/bin/sfml-system-2.dll)
    set(SFML_WINDOW_RELEASE		$ENV{SFML_ROOT}/bin/sfml-window-2.dll)
    add_custom_command(
            TARGET ClientNetwork POST_BUILD
            COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different
            "$<$<CONFIG:Debug>:${SFML_AUDIO_DEBUG}>$<$<CONFIG:Release>:${SFML_AUDIO_RELEASE}>"
            "$<$<CONFIG:Debug>:${SFML_GRAPHICS_DEBUG}>$<$<CONFIG:Release>:${SFML_GRAPHICS_RELEASE}>"
            "$<$<CONFIG:Debug>:${SFML_WINDOW_DEBUG}>$<$<CONFIG:Release>:${SFML_WINDOW_RELEASE}>"
            "$<$<CONFIG:Debug>:${SFML_SYSTEM_DEBUG}>$<$<CONFIG:Release>:${SFML_SYSTEM_RELEASE}>"
            $<TARGET_FILE_DIR:ClientNetwork>
    )

    set (RESOURCES
		AllyBullet.png
		BasicMonster0.png
		BasicMonster1.png
		BasicMonster2.png
		BasicMonster3.png
		BossMonster.png
		Button.png
		ShipSprite.png
		zorque.ttf
            )
    foreach (file_i ${RESOURCES})
        add_custom_command(
                TARGET ClientNetwork POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ressources/${file_i}
                $<TARGET_FILE_DIR:ClientNetwork>/ressources/${file_i})
    endforeach()
endif(WIN32)
