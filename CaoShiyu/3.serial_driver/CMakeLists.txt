cmake_minimum_required(VERSION 3.10)
project(rm_serial_driver)

## Use C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(BUILD_TESTING "Build receive-core tests" ON)

## By adding -Wall and -Werror, the compiler does not ignore warnings anymore,
## enforcing cleaner code.
add_definitions(-Wall -Werror)

## Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#######################
## Find dependencies ##
#######################

find_package(ament_cmake_auto REQUIRED)
ament_auto_find_build_dependencies()

###########
## Build ##
###########

ament_auto_add_library(${PROJECT_NAME} SHARED
  src/rm_serial_driver.cpp
  src/crc.cpp
  src/default_receive_protocol.cpp
  src/vitural_serial.cpp
)

if(BUILD_TESTING)
  add_executable(receive_core_test
    tests/receive_core_test.cpp
    src/default_receive_protocol.cpp
    src/crc.cpp
  )
  target_include_directories(receive_core_test PRIVATE include)
  enable_testing()
  add_test(NAME receive_core_test COMMAND receive_core_test)
endif()

rclcpp_components_register_node(${PROJECT_NAME}
  PLUGIN rm_serial_driver::RMSerialDriver
  EXECUTABLE ${PROJECT_NAME}_node
)
rclcpp_components_register_node(${PROJECT_NAME}
PLUGIN rm_serial_driver::VirtualSerialNode
EXECUTABLE virtual_serial_node
)


#############
## Install ##
#############

ament_auto_package(
  INSTALL_TO_SHARE
  config
  launch
)
