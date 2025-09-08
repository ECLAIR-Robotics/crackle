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
find_package(moveit_task_constructor_core REQUIRED)
find_package(moveit_msgs REQUIRED)
find_package(xarm_msgs REQUIRED)
find_package(example_interfaces REQUIRED)


set(dependencies "rclcpp" "std_msgs" "shape_msgs" "geometry_msgs" "moveit_msgs" "xarm_msgs" "xarm_planner" "moveit")

include_directories(
  include
  ${ament_INCLUDE_DIRS}
)

# Define the executable
add_executable(moveit_task_constructor src/moveit_task_constructor.cpp)

add_executable(moveit_scene_updater src/moveit_scene_updater.cpp)
add_executable(moveit_manipulation src/moveit_manipulation.cpp)

ament_target_dependencies(moveit_manipulation
  moveit_ros_planning_interface
  moveit_task_constructor_core
  crackle_interfaces
  example_interfaces
  ${dependencies}
)

ament_target_dependencies(moveit_task_constructor
  moveit_ros_planning_interface
  moveit_task_constructor_core
  crackle_interfaces
  example_interfaces
  ${dependencies}
)

ament_target_dependencies(moveit_scene_updater
  moveit_ros_planning_interface
  ${dependencies}
)

target_link_libraries(moveit_task_constructor
  ${ament_LIBRARIES}
)

target_link_libraries(moveit_scene_updater
  ${ament_LIBRARIES}
)

# Include directories
target_include_directories(moveit_task_constructor PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
$<INSTALL_INTERFACE:include>)


# target_link_libraries(moveit_task_constructor ${ament_LIBRARIES} xarm_planner)
target_include_directories(moveit_task_constructor PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
   # add xarm_planner library include directory
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/xarm_planner/include>
  )

# Set the required compile features
target_compile_features(moveit_task_constructor PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

install(
  DIRECTORY include/
  DESTINATION include
)


# Install the target
install(TARGETS moveit_task_constructor
  DESTINATION lib/${PROJECT_NAME})

install(TARGETS moveit_scene_updater
  DESTINATION lib/${PROJECT_NAME})

install(TARGETS moveit_manipulation
  DESTINATION lib/${PROJECT_NAME})



install( DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)

ament_package()
