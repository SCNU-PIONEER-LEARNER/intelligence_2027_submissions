# generated from rosidl_cmake/cmake/rosidl_cmake_aggregate_target-extras.cmake.in

# Create a convenience aggregate target status_interface::status_interface
# that links all generated interface targets, so downstream packages can use
# a single modern CMake target name instead of ${status_interface_TARGETS}.
if(status_interface_TARGETS AND NOT TARGET status_interface::status_interface)
  add_library(status_interface::status_interface INTERFACE IMPORTED)
  set_target_properties(status_interface::status_interface PROPERTIES
    INTERFACE_LINK_LIBRARIES "${status_interface_TARGETS}")
endif()
