cmake_minimum_required(VERSION 3.14)

project(qt-lzstring LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core5Compat)

add_library(qt-lzstring SHARED
  lzstring.cpp
  lzstring.h
)

target_link_libraries(qt-lzstring PRIVATE Qt6::Core)
target_link_libraries(qt-lzstring PRIVATE Qt6::Core5Compat)

target_compile_definitions(qt-lzstring PRIVATE QT6LZSTRING_LIBRARY)
