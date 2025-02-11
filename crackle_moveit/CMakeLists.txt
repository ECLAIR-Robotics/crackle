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
find_package(moveit_msgs REQUIRED)
find_package(xarm_planner REQUIRED)  # Add this line
find_package(crackle_interfaces REQUIRED)


set(dependencies "rclcpp" "std_msgs" "geometry_msgs" "moveit_msgs" "xarm_msgs" "xarm_planner")

# Define the executable
add_executable(moveit_task_constructor src/moveit_task_constructor.cpp)

# Include directories
target_include_directories(moveit_task_constructor PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
$<INSTALL_INTERFACE:include>)


ament_target_dependencies(moveit_task_constructor ${dependencies})
# target_link_libraries(moveit_task_constructor ${ament_LIBRARIES} xarm_planner)
target_include_directories(moveit_task_constructor PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
   # add xarm_planner library include directory
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/xarm_planner/include>
  )

# Set the required compile features
target_compile_features(moveit_task_constructor PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17


# Install the target
install(TARGETS moveit_task_constructor
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # Skip linter checks for now
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
