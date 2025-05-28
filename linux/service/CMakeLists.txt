cmake_minimum_required(VERSION 3.14)

project(service LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TARGET_NAME r3minder)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core DBus)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core DBus)

add_executable(${TARGET_NAME}
  main.cpp
  ReminderManagerDBusAdaptor.h
  ReminderManagerDBusAdaptor.cpp
)
target_link_libraries(${TARGET_NAME} Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::DBus lib)

include(GNUInstallDirs)
install(TARGETS ${TARGET_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


