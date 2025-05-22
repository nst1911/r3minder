cmake_minimum_required(VERSION 3.14)

project(lib LANGUAGES CXX)
set(target_name lib)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Sql)

add_library(${target_name} STATIC)

target_sources(${target_name}
    PRIVATE
    src/Helper.h
    src/Reminder.cpp
    src/Reminder.h
    src/ReminderManager.cpp
    src/ReminderManager.h
)

target_include_directories(${target_name}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(${target_name}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Sql

)

include(GNUInstallDirs)
install(TARGETS ${target_name}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
