cmake_minimum_required(VERSION 3.5)


project(lzstring_tests LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt6 REQUIRED COMPONENTS Test)
find_package(Qt6 REQUIRED COMPONENTS Core5Compat)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lzstring_tests lzstring_test.cpp)
add_test(NAME lzstring_tests COMMAND lzstring_tests)

target_include_directories(lzstring_tests PRIVATE "../../src")

add_custom_command(
    TARGET lzstring_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/tests/lzstring_test/data_compressed.json
            ${CMAKE_CURRENT_BINARY_DIR}/data_compressed.json)

add_custom_command(
        TARGET lzstring_tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/tests/lzstring_test/data.json
                ${CMAKE_CURRENT_BINARY_DIR}/data.json)

target_link_libraries(lzstring_tests PRIVATE Qt6::Test)
target_link_libraries(lzstring_tests PRIVATE Qt6::Core5Compat)
target_link_libraries(lzstring_tests PRIVATE qt-lzstring)
