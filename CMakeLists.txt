cmake_minimum_required(VERSION 3.25)

project(AdventOfCode2024 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

file(GLOB_RECURSE COMMON_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/common/*.cpp")
add_executable(Day01 src/day_01/day_01.cpp ${COMMON_SOURCES})
add_executable(Day02 src/day_02/day_02.cpp ${COMMON_SOURCES})

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
install(TARGETS Day01 Day02 DESTINATION bin)
