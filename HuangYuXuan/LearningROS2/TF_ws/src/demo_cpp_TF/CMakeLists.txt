cmake_minimum_required(VERSION 3.8)
project(demo_cpp_TF)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2 REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
add_executable(status_TF_broadcastors src/status_TF_broadcastors.cpp)
add_executable(dynamic_tf2_broadcastors src/dynamic_tf2_broadcastors.cpp)
add_executable(tf2_listener src/tf2_listener.cpp)
ament_target_dependencies(status_TF_broadcastors rclcpp tf2_ros geometry_msgs tf2_geometry_msgs tf2)
ament_target_dependencies(dynamic_tf2_broadcastors rclcpp tf2_ros geometry_msgs tf2_geometry_msgs tf2)
ament_target_dependencies(tf2_listener rclcpp tf2_ros geometry_msgs tf2_geometry_msgs tf2)
install(TARGETS status_TF_broadcastors dynamic_tf2_broadcastors tf2_listener DESTINATION lib/${PROJECT_NAME})
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
