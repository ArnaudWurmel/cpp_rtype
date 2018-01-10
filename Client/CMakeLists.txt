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

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ClientGame)

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
		TARGET ClientGame POST_BUILD
		COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different
			"$<$<CONFIG:Debug>:${SFML_AUDIO_DEBUG}>$<$<CONFIG:Release>:${SFML_AUDIO_RELEASE}>"
			"$<$<CONFIG:Debug>:${SFML_GRAPHICS_DEBUG}>$<$<CONFIG:Release>:${SFML_GRAPHICS_RELEASE}>"
			"$<$<CONFIG:Debug>:${SFML_WINDOW_DEBUG}>$<$<CONFIG:Release>:${SFML_WINDOW_RELEASE}>"
			"$<$<CONFIG:Debug>:${SFML_SYSTEM_DEBUG}>$<$<CONFIG:Release>:${SFML_SYSTEM_RELEASE}>"
			$<TARGET_FILE_DIR:ClientGame>
	)

	set (RESOURCES
		Button.png
		zorque.ttf
	)
	foreach (file_i ${RESOURCES})
	add_custom_command(
		TARGET ClientGame POST_BUILD
	        COMMAND ${CMAKE_COMMAND} -E copy
        	        Graphics/utils/${file_i}
               		$<TARGET_FILE_DIR:ClientGame>/utils/${file_i})
	endforeach()
endif(WIN32)