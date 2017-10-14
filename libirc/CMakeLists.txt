cmake_minimum_required (VERSION 2.6)
project (LibIrc)
set (LibIrc_VERSION_MAJOR 1)
set (LibIrc_VERSION_MINOR 0)
include_directories("${PROJECT_BINARY_DIR}")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(Boost REQUIRED COMPONENTS
             regex thread system)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

add_library(LibBot ircbot.cpp)

add_library(LibMessage ircmessage.cpp)
target_link_libraries (LibMessage ${Boost_LIBRARIES})

add_library(Miaowbot miaowbot.cpp)
target_link_libraries (Miaowbot LibBot)
target_link_libraries (Miaowbot LibMessage)

