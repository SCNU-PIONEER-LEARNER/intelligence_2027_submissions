cmake_minimum_required(VERSION 3.40)
project(taskCode CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 把 src 下所有 .cpp 都包含进来
file(GLOB SOURCES "src/*.cpp")
# 或者手动列出：add_executable(taskCode src/main.cpp src/Robot.cpp)

add_executable(taskCode ${SOURCES})

target_include_directories(taskCode PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})