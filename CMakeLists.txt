cmake_minimum_required (VERSION 2.6)
project (IrcBot)
set (IrcBot_VERSION_MAJOR 1)
set (IrcBot_VERSION_MINOR 0)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
add_definitions(-DRELEASE)
find_package(Boost REQUIRED COMPONENTS
             regex thread system)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

include_directories ("${PROJECT_SOURCE_DIR}/libirc")
add_subdirectory (libirc)

add_executable(IrcBot main.cpp)

target_link_libraries (IrcBot LibBot)
target_link_libraries (IrcBot LibMessage)
target_link_libraries (IrcBot Miaowbot)
if(UNIX)
        target_link_libraries(IrcBot ${Boost_LIBRARIES} pthread)
else()
        target_link_libraries(IrcBot ${Boost_LIBRARIES})
endif()
