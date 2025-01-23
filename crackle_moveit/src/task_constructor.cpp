#include <memory>
#include <signal.h>
#include <rclcpp/rclcpp.hpp>

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <moveit_msgs/msg/display_robot_state.hpp>
#include <moveit_msgs/msg/display_trajectory.hpp>
// #include <moveit_visual_tools/moveit_visual_tools.h>

#include <std_msgs/msg/bool.hpp>
#include <xarm_msgs/srv/plan_pose.hpp>
#include <xarm_msgs/srv/plan_joint.hpp>
#include <xarm_msgs/srv/plan_exec.hpp>
#include <xarm_msgs/srv/plan_single_straight.hpp>

int main(int argc, char** argv)
{
  const double jump_threshold = 0.0;
  const double eef_step = 0.005;
  const double max_velocity_scaling_factor = 0.3;  // [move_group_interface] default is 0.1
  const double max_acceleration_scaling_factor = 0.1;  // [move_group_interface] default is 0.1
  // Initialize ROS 2
  rclcpp::init(argc, argv);
  
  // Create a Node
  auto node = rclcpp::Node::make_shared("xarm_moveit_planner");
  auto logger = node->get_logger();

  // 1. Create the MoveGroup Interface for your xArm
  //    Replace "xarm_group_name" with the group name in your xarm SRDF.
  using moveit::planning_interface::MoveGroupInterface;
  // auto move_group_interface = MoveGroupInterface(node, "lite6");
  std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_interface = 
      std::make_shared<moveit::planning_interface::MoveGroupInterface>(node, "lite6");
  RCLCPP_INFO(node->get_logger(), "Planning frame: %s", move_group_interface->getPlanningFrame().c_str());
  RCLCPP_INFO(node->get_logger(), "End effector link: %s", move_group_interface->getEndEffectorLink().c_str());
  RCLCPP_INFO(node->get_logger(), "Available Planning Groups:");
  std::copy(move_group_interface->getJointModelGroupNames().begin(), move_group_interface->getJointModelGroupNames().end(), std::ostream_iterator<std::string>(std::cout, ", "));
  move_group_interface->setMaxVelocityScalingFactor(max_velocity_scaling_factor);
  move_group_interface->setMaxAccelerationScalingFactor(max_acceleration_scaling_factor);

  // 2. Set a target Pose
  //    For simplicity, define it in a lambda so it's easily returned as a Pose.
  auto const target_pose = []{
    geometry_msgs::msg::Pose msg;
    msg.orientation.w = 1.0;     // No rotation, identity quaternion
    msg.position.x    = 0.28;
    msg.position.y    = -0.2;
    msg.position.z    = 0.5;
    return msg;
  }();

  move_group_interface->setPoseTarget(target_pose);

  // 3. Create a plan to that target pose
  auto const [success, plan] = [&move_group_interface] {
    moveit::planning_interface::MoveGroupInterface::Plan plan_msg;
    bool ok = static_cast<bool>(move_group_interface->plan(plan_msg));
    return std::make_pair(ok, plan_msg);
  }();

  // 4. Execute the plan if planning succeeded
  if (success) {
    RCLCPP_INFO(logger, "Planning successful! Executing...");
    move_group_interface->execute(plan);
  } else {
    RCLCPP_ERROR(logger, "Planning failed!");
  }

  rclcpp::shutdown();
  return 0;
}
  