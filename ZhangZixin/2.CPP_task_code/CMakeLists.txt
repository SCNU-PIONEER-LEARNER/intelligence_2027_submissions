cmake_minimum_required(VERSION 3.10)
project(robot_battle LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(robot_battle
    src/main.cpp
    src/Robot.cpp
)

target_include_directories(robot_battle PRIVATE include)