cmake_minimum_required(VERSION 3.10)
project(receive_core_tests LANGUAGES CXX)

# This separate entry point does not require ROS 2 or any team packages.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(receive_core_test
  receive_core_test.cpp
  ../src/default_receive_protocol.cpp
  ../src/crc.cpp
)
target_include_directories(receive_core_test PRIVATE ../include)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(receive_core_test PRIVATE -Wall -Wextra -Werror -pedantic)
endif()
enable_testing()
add_test(NAME receive_core_test COMMAND receive_core_test)
