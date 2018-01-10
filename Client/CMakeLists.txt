cmake_minimum_required(VERSION 3.5.1)
project(ClientGame)

set(CMAKE_CXX_STANDARD 14)

add_executable(
	ClientGame
	Graphics/main.cpp
	Graphics/Star.cpp
	Graphics/StarField.cpp

	Graphics/Star.hpp
	Graphics/StarField.hpp
)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED audio graphics window system)
if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
	target_link_libraries(ClientGame ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif(SFML_FOUND)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ClientGame)