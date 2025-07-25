cmake_minimum_required(VERSION 3.14)

project(r3minder LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(lib)
add_subdirectory(tests)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_subdirectory(linux/service)
else()
    add_subdirectory(android)
endif()
