cmake_minimum_required(VERSION 3.14)

project(lib LANGUAGES CXX)
set(target_name lib)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(DBUS_PACKAGE DBus)
    set(DBUS_LIBRARY Qt${QT_VERSION_MAJOR}::DBus)
else()
    set(DBUS_PACKAGE "")
    set(DBUS_LIBRARY "")
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Sql ${DBUS_PACKAGE})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Sql ${DBUS_PACKAGE})

add_library(${target_name} STATIC)

target_sources(${target_name}
    PRIVATE
    Common.h
    Reminder.cpp
    Reminder.h
    ReminderManager.cpp
    ReminderManager.h
)

target_include_directories(${target_name}
    PUBLIC
    ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${target_name}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Sql
    ${DBUS_LIBRARY}
)

include(GNUInstallDirs)
install(TARGETS ${target_name}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
