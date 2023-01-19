cmake_minimum_required(VERSION 3.14)

project(
  qt-lzstring
  LANGUAGES CXX
  VERSION 1.4.5
  DESCRIPTION "Qt6 port of LZString compression")

add_subdirectory(src)
add_subdirectory(tests)
