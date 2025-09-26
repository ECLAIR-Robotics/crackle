cmake_minimum_required(VERSION 3.8)
project(crackle_moveit)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(rclcpp REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(std_msgs REQUIRED)
find_package(xarm_msgs REQUIRED)
find_package(xarm_planner REQUIRED)  # Add this line
find_package(crackle_interfaces REQUIRED)
find_package(moveit REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(moveit_msgs REQUIRED)
find_package(xarm_msgs REQUIRED)
find_package(example_interfaces REQUIRED)


set(dependencies "rclcpp" "std_msgs" "shape_msgs" "geometry_msgs" "moveit_msgs" "xarm_msgs" "xarm_planner" "moveit")

include_directories(
  include
  ${ament_INCLUDE_DIRS}
)

# Define the executable

add_executable(moveit_scene_updater src/moveit_scene_updater.cpp)
add_executable(moveit_manipulation src/moveit_manipulation.cpp)

ament_target_dependencies(moveit_manipulation
  moveit_ros_planning_interface
  crackle_interfaces
  example_interfaces
  ${dependencies}
)



ament_target_dependencies(moveit_scene_updater
  moveit_ros_planning_interface
  ${dependencies}
)

target_link_libraries(moveit_scene_updater
  ${ament_LIBRARIES}
)

install(
  DIRECTORY include/
  DESTINATION include
)

install(TARGETS moveit_scene_updater
  DESTINATION lib/${PROJECT_NAME})

install(TARGETS moveit_manipulation
  DESTINATION lib/${PROJECT_NAME})



install( DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)

ament_package()
