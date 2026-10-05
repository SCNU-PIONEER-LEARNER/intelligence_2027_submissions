cmake_minimum_required(VERSION 3.20)

project(PIONEER_VISION_TASK LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(vision_task
    src/main.cpp
    src/Robot.cpp
)

target_include_directories(vision_task PRIVATE include)
