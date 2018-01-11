cmake_minimum_required(VERSION 3.9)
project(ClientNetwork)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    ./dependency/imgui/
)

add_executable(ClientNetwork main.cpp ArgumentLoader/ArgumentLoader.hh ArgumentLoader/ArgumentLoader.cpp ArgumentLoader/Configure.hh ArgumentLoader/Configure.cpp Logger/LoggerHelper.cpp Logger/Logger.hpp Logger/LoggerHelper.cpp NetworkAbstract/Message/Message.cpp NetworkAbstract/Message/Message.h NetworkAbstract/BoostSocket.cpp NetworkAbstract/BoostAcceptor.cpp NetworkAbstract/BoostAcceptor.hh NetworkAbstract/IAcceptor.hh NetworkAbstract/IAcceptor.cpp NetworkAbstract/ISocket.cpp NetworkAbstract/ISocket.h NetworkAbstract/SocketReceiver.hh NetworkAbstract/SocketReceiver.cpp DataGetter/DataGetter.cpp DataGetter/DataGetter.hh Exceptions/Exception.cpp Exceptions/Exception.hh Graphics/RootViewController.cpp Graphics/RootViewController.hh 	dependency/imgui/imgui-SFML.cpp dependency/imgui/imgui.cpp dependency/imgui/imgui_draw.cpp dependency/imgui/imgui_demo.cpp Graphics/LoginViewController.cpp Graphics/LoginViewController.hh Graphics/AViewController.hh)
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.65.0 REQUIRED COMPONENTS thread system)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(ClientNetwork ${Boost_LIBRARIES})
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED audio graphics window system)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(ClientNetwork ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif(SFML_FOUND)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(ClientNetwork ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ClientNetwork)

if(WIN32)
    set(SFML_AUDIO_DEBUG		$ENV{SFML_ROOT}/bin/sfml-audio-d-2.dll)
    set(SFML_GRAPHICS_DEBUG		$ENV{SFML_ROOT}/bin/sfml-graphics-d-2.dll)
    set(SFML_SYSTEM_DEBUG		$ENV{SFML_ROOT}/bin/sfml-system-d-2.dll)
    set(SFML_WINDOW_DEBUG		$ENV{SFML_ROOT}/bin/sfml-window-d-2.dll)
    set(SFML_AUDIO_RELEASE		$ENV{SFML_ROOT}/bin/sfml-audio-d-2.dll)
    set(SFML_GRAPHICS_RELEASE	$ENV{SFML_ROOT}/bin/sfml-graphics-2.dll)
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
            Button.png
            zorque.ttf
            )
    foreach (file_i ${RESOURCES})
        add_custom_command(
                TARGET ClientNetwork POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                Graphics/utils/${file_i}
                $<TARGET_FILE_DIR:ClientNetwork>/utils/${file_i})
    endforeach()
endif(WIN32)
