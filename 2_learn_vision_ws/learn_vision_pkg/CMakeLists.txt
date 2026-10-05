cmake_minimum_required(VERSION 3.8)
project(learn_vision_pkg)
add_executable(node_image_show src/node_image_show.cpp)
add_executable(node_video_show src/node_video_show.cpp)
add_executable(node_video_publisher src/node_video_publisher.cpp)
add_executable(node_video_subscriber src/node_video_subscriber.cpp)
add_executable(node_threshold src/node_threshold.cpp)
add_executable(node_ogb_show src/node_ogb_show.cpp)
add_executable(node_image_clean src/node_image_clean.cpp)
add_executable(node_draw_contour src/node_draw_contour.cpp)
add_executable(node_min_rect src/node_min_rect.cpp)
add_executable(node_perspective_transform src/node_perspective_transform.cpp)
add_executable(node_perspective_transform2 src/node_perspective_transform2.cpp)
add_executable(byds_perspective_transform src/byds_perspective_transform.cpp)
add_executable(byds_debug_npt2 src/byds_debug_npt2.cpp)
add_executable(node_task src/node_task.cpp)
add_executable(node_task2 src/node_task2.cpp)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
find_package(OpenCV REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
ament_target_dependencies(node_image_show OpenCV rclcpp) 
ament_target_dependencies(node_video_show OpenCV rclcpp) 
ament_target_dependencies(node_video_publisher rclcpp cv_bridge sensor_msgs)
ament_target_dependencies(node_video_subscriber rclcpp cv_bridge sensor_msgs)
ament_target_dependencies(node_threshold rclcpp cv_bridge sensor_msgs )
ament_target_dependencies(node_ogb_show rclcpp)
ament_target_dependencies(node_image_clean rclcpp)
ament_target_dependencies(node_draw_contour rclcpp )
ament_target_dependencies(node_min_rect rclcpp)
ament_target_dependencies(node_perspective_transform rclcpp)
ament_target_dependencies(node_perspective_transform2 rclcpp)
ament_target_dependencies(byds_perspective_transform rclcpp)
ament_target_dependencies(byds_debug_npt2 rclcpp)
ament_target_dependencies(node_task rclcpp )
ament_target_dependencies(node_task2 rclcpp )
target_link_libraries(node_video_publisher ${OpenCV_LIBRARIES})
target_link_libraries(node_video_subscriber ${OpenCV_LIBRARIES})
target_link_libraries(node_threshold ${OpenCV_LIBRARIES})
target_link_libraries(node_ogb_show ${OpenCV_LIBRARIES})
target_link_libraries(node_image_clean ${OpenCV_LIBRARIES})
target_link_libraries(node_draw_contour ${OpenCV_LIBRARIES})
target_link_libraries(node_min_rect ${OpenCV_LIBRARIES})
target_link_libraries(node_perspective_transform ${OpenCV_LIBRARIES})
target_link_libraries(node_perspective_transform2 ${OpenCV_LIBRARIES})
target_link_libraries(byds_perspective_transform ${OpenCV_LIBRARIES})
target_link_libraries(byds_debug_npt2 ${OpenCV_LIBRARIES})
target_link_libraries(node_task ${OpenCV_LIBRARIES})
target_link_libraries(node_task2 ${OpenCV_LIBRARIES})
install(TARGETS node_image_show
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS node_video_show
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS node_video_publisher
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS node_video_subscriber
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS node_threshold
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS node_ogb_show
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS node_image_clean
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS node_draw_contour
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS node_min_rect
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS node_perspective_transform
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS node_perspective_transform2
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS byds_perspective_transform
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS byds_debug_npt2
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS node_task
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS node_task2
  DESTINATION lib/${PROJECT_NAME})
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
