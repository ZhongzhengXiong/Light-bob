cmake_minimum_required(VERSION 3.6)
project(Light_bot)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Map.cpp Map.h Robot.cpp Robot.h Game.cpp Game.h)
add_executable(Light_bot ${SOURCE_FILES})