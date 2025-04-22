#include <memory>

#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit/task_constructor/task.h>
#include <moveit/task_constructor/solvers.h>
#include <moveit/task_constructor/stages.h>


// #include "crackle_moveit/moveit_task_constructor.hpp"

// CrackleMoveitTaskConstructorNode::CrackleMoveitTaskConstructorNode(const rclcpp::Node::SharedPtr& node, const std::string& group_name)
//     : node_(node)
// {
//     init(group_name);
// }

// CrackleMoveitTaskConstructorNode::CrackleMoveitTaskConstructorNode(const std::string& group_name)
// {
//     node_ = rclcpp::Node::make_shared("xarm_planner_move_group_node");
//     init(group_name);
// }

// void CrackleMoveitTaskConstructorNode::init(const std::string& group_name) 
// {
//     is_trajectory_ = false;
//     move_group_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(node_, group_name);
//     RCLCPP_INFO(node_->get_logger(), "Planning frame: %s", move_group_->getPlanningFrame().c_str());
//     RCLCPP_INFO(node_->get_logger(), "End effector link: %s", move_group_->getEndEffectorLink().c_str());
//     RCLCPP_INFO(node_->get_logger(), "Available Planning Groups:");
//     std::copy(move_group_->getJointModelGroupNames().begin(), move_group_->getJointModelGroupNames().end(), std::ostream_iterator<std::string>(std::cout, ", "));
// }

// bool CrackleMoveitTaskConstructorNode::planPoseTarget(const geometry_msgs::msg::Pose& pose_target)
// {
//     bool success = move_group_->setPoseTarget(pose_target);
//     if (!success)
//         RCLCPP_WARN(node_->get_logger(), "setPoseTarget: out of bounds");
//     success = (move_group_->plan(xarm_plan_) == moveit::core::MoveItErrorCode::SUCCESS);
//     if (!success)
//         RCLCPP_ERROR(node_->get_logger(), "planPoseTarget: plan failed");
//     is_trajectory_ = false;
//     return success;
// }

// bool CrackleMoveitTaskConstructorNode::planCartesianPath(const std::vector<geometry_msgs::msg::Pose>& pose_target_vector)
// {
//     move_group_->setStartStateToCurrentState();
//     move_group_->setMaxVelocityScalingFactor(0.1);
//     move_group_->setMaxAccelerationScalingFactor(0.1);
//     moveit::planning_interface::MoveGroupInterface::Plan plan;
//     double fraction = move_group_->computeCartesianPath(pose_target_vector, 0.01, 0.0, plan.trajectory_);
//     if (fraction < 1.0)
//         RCLCPP_ERROR(node_->get_logger(), "planCartesianPath: plan failed");
//     is_trajectory_ = true;
//     return fraction >= 1.0;
// }
// bool CrackleMoveitTaskConstructorNode::executePath(bool wait)
// {
//     if (is_trajectory_) {
//         move_group_->execute(xarm_plan_, wait);
//     } else {
//         move_group_->move();
//     }
//     return true;
// }

// mtc::Task CrackleMoveitTaskConstructorNode::createTask(const std::string& group_name)
// {
//     mtc::Task task;
//     task.setRobotModel(move_group_->getRobotModel());
//     task.setName("xarm_planner_task");
//     task.setMaxTime(10.0);
//     task.setMaxAttempts(5);
//     task.setMaxCost(1000.0);
//     task.setMaxPlanningTime(5.0);
//     task.setMaxVelocityScalingFactor(0.1);
//     task.setMaxAccelerationScalingFactor(0.1);

//     return task;
// }

namespace mtc = moveit::task_constructor;

int main(int argc, char** argv)
{
    // Initialize ROS and create the Node
  rclcpp::init(argc, argv);
  auto const node = std::make_shared<rclcpp::Node>(
    "hello_moveit",
    rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true)
  );

  // Create a ROS logger
  auto const logger = rclcpp::get_logger("hello_moveit");
  RCLCPP_INFO(logger, "Hello MoveIt!");


  // Create the MoveIt MoveGroup Interface
  using moveit::planning_interface::MoveGroupInterface;
  auto move_group_interface = MoveGroupInterface(node, "lite6");

  // Set a target Pose
  auto const target_pose = []{
    geometry_msgs::msg::Pose msg;
    msg.orientation.w = 1.0;
    msg.position.x = 0.28;
    msg.position.y = -0.2;
    msg.position.z = 0.5;
    return msg;
  }();

  move_group_interface.setPoseTarget(target_pose);

  // Create a plan to that target pose
  auto const [success, plan] = [&move_group_interface]{
    moveit::planning_interface::MoveGroupInterface::Plan msg;
    auto const ok = static_cast<bool>(move_group_interface.plan(msg));
    return std::make_pair(ok, msg);
  }();

  // Execute the plan
  if(success) {
    move_group_interface.execute(plan);
    RCLCPP_INFO(logger, "Planning succeeded!");
  } else {
    RCLCPP_ERROR(logger, "Planning failed!");
  }

  mtc::Task task;
  task.stages()->setName("xarm_planner_task");
  task.setRobotModel(move_group_interface.getRobotModel());

  const auto& arm_group_name = "lite6";
  
  const auto& hand_frame = "gripper_base";

  // Set task properties
  task.setProperty("group", arm_group_name);
  task.setProperty("ik_frame", hand_frame);

// Disable warnings for this line, as it's a variable that's set but not used in this example
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    mtc::Stage* current_state_ptr = nullptr;  // Forward current_state on to grasp pose generator
  #pragma GCC diagnostic pop

  auto stage_state_current = std::make_unique<mtc::stages::CurrentState>("current");
  current_state_ptr = stage_state_current.get();
  task.add(std::move(stage_state_current));

  auto sampling_planner = std::make_shared<mtc::solvers::PipelinePlanner>(node);
  auto interpolation_planner = std::make_shared<mtc::solvers::JointInterpolationPlanner>();


  auto cartesian_planner = std::make_shared<mtc::solvers::CartesianPath>();
  cartesian_planner->setMaxVelocityScalingFactor(1.0);
  cartesian_planner->setMaxAccelerationScalingFactor(1.0);
  cartesian_planner->setStepSize(.01);

  const geometry_msgs::msg::PoseStamped target_pose_2 = []{
    geometry_msgs::msg::PoseStamped msg;
    msg.header.frame_id = "gripper_base";
    msg.pose.orientation.w = 1.0;
    msg.pose.position.x = 0.28;
    msg.pose.position.y = -0.2;
    msg.pose.position.z = 0.5;
    return msg;
  }();

  auto move_to = std::make_unique<mtc::stages::MoveTo>("move_to");
  move_to->setGroup(arm_group_name);
  move_to->setGoal(target_pose_2);
  move_to->setProperty("group", arm_group_name);
  
  task.add(std::move(move_to));
  
  
  RCLCPP_INFO(logger, "Task created!");
  task.init();

  rclcpp::spin(node);

  // Shutdown ROS
  rclcpp::shutdown();
  return 0;
}
