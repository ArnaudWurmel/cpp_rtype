cmake_minimum_required(VERSION 3.5.1)
project(ServerAuth)

set(CMAKE_CXX_STANDARD 14)

add_executable(ServerAuth main.cpp ServerRegister/ServerRegister.cpp ServerRegister/ServerRegister.hh Logger/Logger.hpp Logger/LoggerHelper.cpp Logger/LoggerHelper.hh ArgumentLoader/ArgumentLoader.cpp ArgumentLoader/ArgumentLoader.hh ArgumentLoader/Configure.cpp ArgumentLoader/Configure.hh)

#BOOST
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.55.0 REQUIRED COMPONENTS thread regex system)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(ServerAuth ${Boost_LIBRARIES})
endif()
