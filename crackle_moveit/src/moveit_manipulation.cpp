
/**
 * @file moveit_manipulation.cpp
 * @brief Implements basic MoveIt! manipulation functionalities for the Crackle
 * robot.
 *
 * This file contains functions to initialize the MoveIt! MoveGroupInterface,
 * plan and execute motions to a target pose, and a main function to run the
 * manipulation node.
 *
 * Functions:
 * - initialize: Sets up the MoveGroupInterface, logs planning frame, end
 * effector link, and available planning groups.
 * - move_to_pose: Plans and executes a motion to a specified target pose using
 * MoveIt!.
 * - main: Initializes ROS 2, creates the node and logger, initializes MoveIt!,
 * defines a target pose, and commands the robot to move to the target pose.
 *
 * Usage:
 * Compile and run as part of a ROS 2 package. The node will attempt to move the
 * robot's end effector to a hardcoded target pose using MoveIt! planning and
 * execution.
 *
 * @author Tanay Garg
 * @date 09/08/2025
 */
#include <algorithm>
#include <chrono>
#include <cmath>
#include <limits>
#include <thread>
#include <crackle_moveit/moveit_manipulation.hpp>
#include <tf2/LinearMath/Matrix3x3.h>
#include <cstdio>
#include <vector>
#include <Eigen/Geometry>
#include <Eigen/Eigenvalues>

// MTC stages used to build the pickup task.
#include <moveit/task_constructor/container.h>
#include <moveit/task_constructor/stages/current_state.h>
#include <moveit/task_constructor/stages/connect.h>
#include <moveit/task_constructor/stages/fixed_cartesian_poses.h>
#include <moveit/task_constructor/stages/compute_ik.h>
#include <moveit/task_constructor/stages/modify_planning_scene.h>
#include <moveit/task_constructor/stages/move_relative.h>
#include <moveit_task_constructor_msgs/msg/solution.hpp>
#include <geometry_msgs/msg/vector3_stamped.hpp>

const double jump_threshold = 0.0;
const double eef_step = 0.001;
const double max_velocity_scaling_factor =
    0.1; // [move_group_interface] default is 0.1
const double max_acceleration_scaling_factor =
    0.1; // [move_group_interface] default is 0.1

static const Eigen::Vector3d kToolForwardInTool(0.0, 0.0, 1.0); // Z axis

// ---------------------------------------------------------------------------
// File-local helpers
// ---------------------------------------------------------------------------

// Retreat from `grasp` along the negative approach direction by `dist` metres.
// The approach direction is the tool-forward axis rotated into world frame.
static geometry_msgs::msg::Pose
compute_pregrasp(const geometry_msgs::msg::Pose &grasp, double dist)
{
  Eigen::Quaterniond q(grasp.orientation.w, grasp.orientation.x,
                       grasp.orientation.y, grasp.orientation.z);
  Eigen::Vector3d approach_dir = (q * kToolForwardInTool).normalized();
  geometry_msgs::msg::Pose pregrasp = grasp;
  pregrasp.position.x -= approach_dir.x() * dist;
  pregrasp.position.y -= approach_dir.y() * dist;
  pregrasp.position.z -= approach_dir.z() * dist;
  return pregrasp;
}

// Build a robot state seeded from the last waypoint of a trajectory message.
static moveit::core::RobotState
state_from_trajectory_end(const moveit_msgs::msg::RobotTrajectory &traj,
                           const moveit::core::RobotModelConstPtr &model)
{
  moveit::core::RobotState state(model);
  state.setToDefaultValues();
  const auto &jt = traj.joint_trajectory;
  if (jt.points.empty())
    return state;
  const auto &last = jt.points.back();
  for (size_t i = 0; i < jt.joint_names.size(); ++i)
    state.setJointPositions(jt.joint_names[i], &last.positions[i]);
  state.update();
  return state;
}

// XY half-extents of a solid primitive (used for 2-D footprint collision checks).
static std::pair<double, double>
primitive_footprint_xy(const shape_msgs::msg::SolidPrimitive &p) {
  using P = shape_msgs::msg::SolidPrimitive;
  switch (p.type) {
  case P::BOX:
    return {p.dimensions[P::BOX_X] / 2.0, p.dimensions[P::BOX_Y] / 2.0};
  case P::CYLINDER:
    return {p.dimensions[P::CYLINDER_RADIUS], p.dimensions[P::CYLINDER_RADIUS]};
  case P::SPHERE:
    return {p.dimensions[P::SPHERE_RADIUS], p.dimensions[P::SPHERE_RADIUS]};
  default:
    return {0.05, 0.05};
  }
}

static double primitive_height(const shape_msgs::msg::SolidPrimitive &p) {
  using P = shape_msgs::msg::SolidPrimitive;
  switch (p.type) {
  case P::BOX:      return p.dimensions[P::BOX_Z];
  case P::CYLINDER: return p.dimensions[P::CYLINDER_HEIGHT];
  case P::SPHERE:   return p.dimensions[P::SPHERE_RADIUS] * 2.0;
  default:          return 0.10;
  }
}

CrackleManipulation::CrackleManipulation(const std::string &group_name)
    : logger_(rclcpp::get_logger("crackle_moveit_manipulation_node")) {
  // Auto-declare parameters from overrides so the planning-pipeline parameters
  // (e.g. ompl.planning_plugin, ompl.<group>) forwarded by the launch file are
  // visible to the MTC PipelinePlanner built on this node.
  rclcpp::NodeOptions node_options;
  node_options.automatically_declare_parameters_from_overrides(true);
  node_ = rclcpp::Node::make_shared("crackle_moveit_manipulation_node",
                                    node_options);

  have_joint_state_ = false;
  last_joint_state_stamp_ = node_->now();
  joint_state_sub_ = node_->create_subscription<sensor_msgs::msg::JointState>(
      "/joint_states", 10,
      [this](const sensor_msgs::msg::JointState::SharedPtr msg) {
        last_joint_state_stamp_ = msg->header.stamp;
        have_joint_state_ = true;
      });

  services_cb_group_ = node_->create_callback_group(
      rclcpp::CallbackGroupType::MutuallyExclusive);

  pickup_service_ =
      node_->create_service<crackle_interfaces::srv::PickupObject>(
          "crackle_manipulation/pickup_object",
          std::bind(&CrackleManipulation::pick_up_object_mtc, this,
                    std::placeholders::_1, std::placeholders::_2),
          rmw_qos_profile_services_default, services_cb_group_);
  place_service_ =
      node_->create_service<crackle_interfaces::srv::PlaceObject>(
          "crackle_manipulation/place_object",
          std::bind(&CrackleManipulation::place_object_mtc, this,
                    std::placeholders::_1, std::placeholders::_2),
          rmw_qos_profile_services_default, services_cb_group_);
  look_at_service_ = node_->create_service<crackle_interfaces::srv::LookAt>(
      "crackle_manipulation/look_at",
      std::bind(&CrackleManipulation::look_at, this, std::placeholders::_1,
                std::placeholders::_2),
      rmw_qos_profile_services_default, services_cb_group_);
  plan_pose_service_ = node_->create_service<crackle_interfaces::srv::PlanPose>(
      "crackle_manipulation/plan_pose",
      std::bind(&CrackleManipulation::plan_pose_service, this,
                std::placeholders::_1, std::placeholders::_2),
      rmw_qos_profile_services_default, services_cb_group_);
  execute_plan_service_ =
      node_->create_service<crackle_interfaces::srv::ExecutePlan>(
          "crackle_manipulation/execute_plan",
          std::bind(&CrackleManipulation::execute_plan_service, this,
                    std::placeholders::_1, std::placeholders::_2),
          rmw_qos_profile_services_default, services_cb_group_);
  plan_trajectory_service_ =
      node_->create_service<crackle_interfaces::srv::PlanTrajectory>(
          "crackle_manipulation/plan_trajectory",
          std::bind(&CrackleManipulation::plan_trajectory_service, this,
                    std::placeholders::_1, std::placeholders::_2),
          rmw_qos_profile_services_default, services_cb_group_);
  set_joint_angles_service_ =
      node_->create_service<crackle_interfaces::srv::SetJointAngles>(
          "crackle_manipulation/set_joint_angles",
          std::bind(&CrackleManipulation::set_joint_angles_service, this,
                    std::placeholders::_1, std::placeholders::_2),
          rmw_qos_profile_services_default, services_cb_group_);
  face_direction_relative_service_ =
      node_->create_service<crackle_interfaces::srv::FaceDirectionRelative>(
          "crackle_manipulation/face_direction_relative",
          std::bind(&CrackleManipulation::face_direction_relative_service, this,
                    std::placeholders::_1, std::placeholders::_2),
          rmw_qos_profile_services_default, services_cb_group_);
  move_relative_service_ =
      node_->create_service<crackle_interfaces::srv::MoveRelative>(
          "crackle_manipulation/move_relative",
          std::bind(&CrackleManipulation::move_relative_service, this,
                    std::placeholders::_1, std::placeholders::_2),
          rmw_qos_profile_services_default, services_cb_group_);
  get_end_effector_pose_service_ =
      node_->create_service<crackle_interfaces::srv::GetEndEffectorPose>(
          "crackle_manipulation/get_end_effector_pose",
          std::bind(&CrackleManipulation::get_end_effector_pose_service, this,
                    std::placeholders::_1, std::placeholders::_2),
          rmw_qos_profile_services_default, services_cb_group_);
    
    marker_publisher_ = node_->create_publisher<visualization_msgs::msg::Marker>("/crackle_manipulation/object_position", 10);
    grasp_markers_publisher_ = node_->create_publisher<visualization_msgs::msg::MarkerArray>(
        "/crackle_manipulation/grasp_candidates", 10);
    grip_point_publisher_ = node_->create_publisher<visualization_msgs::msg::MarkerArray>(
        "/crackle_manipulation/grip_point", 10);
    // Republish the grip-point marker at 2 Hz. It lives in the EEF frame, so
    // rviz keeps it pinned to the moving tool; republishing just keeps it fresh
    // and reaches clients that connect late.
    grip_point_timer_ = node_->create_wall_timer(
        std::chrono::milliseconds(500),
        std::bind(&CrackleManipulation::publish_grip_point_marker, this));

  // TODO: Make a special trajectory service that takes high level parameters
  // and then does the same thing more or less as the demo_trajectory_service
  // but around the current pose instead of a fixed pose, and with more options
  // for types of trajectories. The demo_trajectory_service can call that one
  // internally.
  demo_trajectory_service_ =
      node_->create_service<crackle_interfaces::srv::DemoTrajectory>(
          "crackle_manipulation/demo_trajectory",
          std::bind(&CrackleManipulation::demo_trajectory_service, this,
                    std::placeholders::_1, std::placeholders::_2),
          rmw_qos_profile_services_default, services_cb_group_);
  dance_service_ = node_->create_service<std_srvs::srv::Trigger>(
      "crackle_manipulation/dance",
      std::bind(&CrackleManipulation::dance_dance, this, std::placeholders::_1,
                std::placeholders::_2),
      rmw_qos_profile_services_default, services_cb_group_);
  get_scan_pose_service_ = node_->create_service<std_srvs::srv::Trigger>(
      "crackle_manipulation/get_scan_pose",
      std::bind(&CrackleManipulation::get_scan_pose_service, this,
                std::placeholders::_1, std::placeholders::_2),
      rmw_qos_profile_services_default, services_cb_group_);
  gripper_cb_group_ =
      node_->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
  gripper_client_ = node_->create_client<std_srvs::srv::SetBool>(
      "/claw/set_gripper", rmw_qos_profile_services_default, gripper_cb_group_);
  initialize(group_name);
}

// Command the gripper and block until the firmware confirms completion.
bool CrackleManipulation::set_gripper_blocking(bool close, double timeout_s) {
  const char *action = close ? "close" : "open";

  if (!gripper_client_->wait_for_service(std::chrono::seconds(2))) {
    RCLCPP_ERROR(node_->get_logger(),
                 "/claw/set_gripper unavailable; cannot %s gripper", action);
    return false;
  }

  auto request = std::make_shared<std_srvs::srv::SetBool::Request>();
  request->data = close;
  auto future = gripper_client_->async_send_request(request);

  if (future.wait_for(std::chrono::duration<double>(timeout_s)) !=
      std::future_status::ready) {
    RCLCPP_ERROR(node_->get_logger(), "gripper %s timed out", action);
    return false;
  }

  auto response = future.get();
  if (!response->success) {
    RCLCPP_WARN(node_->get_logger(), "gripper %s reported failure: %s", action,
                response->message.c_str());
  } else {
    RCLCPP_INFO(node_->get_logger(), "gripper %s: %s", action,
                response->message.c_str());
  }
  return response->success;
}

// Assumption: This will be the exact point the arm needs to look at
bool CrackleManipulation::look_at(
    crackle_interfaces::srv::LookAt::Request::SharedPtr request,
    crackle_interfaces::srv::LookAt::Response::SharedPtr response) {
  const double OFFSET = 0.15; // meters
  geometry_msgs::msg::Vector3Stamped look_point = request->look_direction;
  Eigen::Vector3d offsetted_position = Eigen::Vector3d(
      look_point.vector.x, look_point.vector.y, look_point.vector.z);
  if (offsetted_position.x() > 0)
    offsetted_position.x() -= OFFSET;
  else
    offsetted_position.x() += OFFSET;
  if (offsetted_position.y() > 0)
    offsetted_position.y() -= OFFSET;
  else
    offsetted_position.y() += OFFSET;

  Eigen::Vector3d to_dir_world(look_point.vector.x - offsetted_position.x(),
                               look_point.vector.y - offsetted_position.y(),
                               look_point.vector.z - offsetted_position.z());
  to_dir_world.normalize();
  Eigen::Vector3d forward_dir_world =
      kToolForwardInTool; // Assuming tool frame aligns with world frame
  geometry_msgs::msg::Pose target_pose;
  target_pose.position.x = offsetted_position.x();
  target_pose.position.y = offsetted_position.y();
  target_pose.position.z = offsetted_position.z();
  target_pose.orientation =
      lookAtQuat(to_dir_world, Eigen::Vector3d::UnitZ(), kToolForwardInTool);

  bool success = plan_to_pose(target_pose);
  if (!success) {
    RCLCPP_ERROR(node_->get_logger(),
                 "look_at: planning to look at point failed");
    response->success = false;
    return false;
  }
  moveit::core::MoveItErrorCode exec_status = move_group_->execute(plan_);
  if (exec_status != moveit::core::MoveItErrorCode::SUCCESS) {
    RCLCPP_ERROR(node_->get_logger(),
                 "look_at: execution to look at point failed");
    response->success = false;
    return false;
  }
  response->success = true;
  return true;
}

bool CrackleManipulation::plan_pose_service(
    crackle_interfaces::srv::PlanPose::Request::SharedPtr request,
    crackle_interfaces::srv::PlanPose::Response::SharedPtr response) {
  bool success = plan_to_pose(request->target_pose);
  response->success = success;
  return true;
}

bool CrackleManipulation::execute_plan_service(
    crackle_interfaces::srv::ExecutePlan::Request::SharedPtr request,
    crackle_interfaces::srv::ExecutePlan::Response::SharedPtr response) {
  bool success = execute_plan(request->wait);
  response->success = success;
  return true;
}

bool CrackleManipulation::plan_trajectory_service(
    crackle_interfaces::srv::PlanTrajectory::Request::SharedPtr request,
    crackle_interfaces::srv::PlanTrajectory::Response::SharedPtr response) {
  bool success = plan_cartesian_path(request->waypoints);
  response->success = success;
  return true;
}

bool CrackleManipulation::set_joint_angles_service(
    crackle_interfaces::srv::SetJointAngles::Request::SharedPtr request,
    crackle_interfaces::srv::SetJointAngles::Response::SharedPtr response) {
  const auto &joint_angles = request->joint_angles;
  auto joint_model_group =
      move_group_->getRobotModel()->getJointModelGroup(move_group_->getName());
  if (!joint_model_group) {
    RCLCPP_ERROR(node_->get_logger(),
                 "set_joint_angles: joint model group not found");
    response->success = false;
    return true;
  }

  const size_t expected = joint_model_group->getVariableCount();
  if (joint_angles.size() != expected) {
    RCLCPP_ERROR(node_->get_logger(),
                 "set_joint_angles: expected %zu joints, got %zu", expected,
                 joint_angles.size());
    response->success = false;
    return true;
  }

  if (!wait_for_current_state("set_joint_angles")) {
    response->success = false;
    return true;
  }
  move_group_->setStartStateToCurrentState();
  bool target_ok = move_group_->setJointValueTarget(joint_angles);
  if (!target_ok) {
    RCLCPP_ERROR(node_->get_logger(),
                 "set_joint_angles: failed to set joint targets");
    response->success = false;
    return true;
  }

  bool success =
      (move_group_->plan(plan_) == moveit::core::MoveItErrorCode::SUCCESS);
  if (!success)
    RCLCPP_ERROR(node_->get_logger(), "set_joint_angles: planning failed");
  is_trajectory_ = false;
  response->success = success;
  return true;
}

bool CrackleManipulation::face_direction_relative_service(
    crackle_interfaces::srv::FaceDirectionRelative::Request::SharedPtr request,
    crackle_interfaces::srv::FaceDirectionRelative::Response::SharedPtr
        response) {
  geometry_msgs::msg::PoseStamped current_pose = move_group_->getCurrentPose();
  Eigen::Vector3d dir_tool(request->direction_tool.x, request->direction_tool.y,
                           request->direction_tool.z);
  if (dir_tool.norm() < 1e-6) {
    RCLCPP_ERROR(node_->get_logger(),
                 "face_direction_relative: zero direction provided");
    response->success = false;
    return true;
  }

  Eigen::Quaterniond q(
      current_pose.pose.orientation.w, current_pose.pose.orientation.x,
      current_pose.pose.orientation.y, current_pose.pose.orientation.z);
  Eigen::Vector3d dir_world = (q * dir_tool).normalized();

  geometry_msgs::msg::Pose target_pose = current_pose.pose;
  target_pose.orientation =
      lookAtQuat(dir_world, Eigen::Vector3d::UnitZ(), kToolForwardInTool);

  bool success = plan_to_pose(target_pose);
  response->success = success;
  return true;
}

bool CrackleManipulation::move_relative_service(
    crackle_interfaces::srv::MoveRelative::Request::SharedPtr request,
    crackle_interfaces::srv::MoveRelative::Response::SharedPtr response) {
  geometry_msgs::msg::PoseStamped current_pose = move_group_->getCurrentPose();
  Eigen::Vector3d offset_tool(request->offset_tool.x, request->offset_tool.y,
                              request->offset_tool.z);

  Eigen::Quaterniond q(
      current_pose.pose.orientation.w, current_pose.pose.orientation.x,
      current_pose.pose.orientation.y, current_pose.pose.orientation.z);
  Eigen::Vector3d offset_world = q * offset_tool;

  geometry_msgs::msg::Pose target_pose = current_pose.pose;
  target_pose.position.x += offset_world.x();
  target_pose.position.y += offset_world.y();
  target_pose.position.z += offset_world.z();

  bool success = plan_to_pose(target_pose);
  response->success = success;
  return true;
}

bool CrackleManipulation::get_end_effector_pose_service(
    crackle_interfaces::srv::GetEndEffectorPose::Request::SharedPtr request,
    crackle_interfaces::srv::GetEndEffectorPose::Response::SharedPtr response) {
  (void)request;
  if (!wait_for_current_state("get_end_effector_pose")) {
    response->success = false;
    return true;
  }
  geometry_msgs::msg::PoseStamped current_pose = move_group_->getCurrentPose();
  response->pose = current_pose.pose;
  response->success = true;
  return true;
}

bool CrackleManipulation::get_scan_pose_service(
    std_srvs::srv::Trigger::Request::SharedPtr request,
    std_srvs::srv::Trigger::Response::SharedPtr response) {
  (void)request;
  if (!wait_for_current_state("get_scan_pose")) {
    response->success = false;
    response->message = "no current state";
    return true;
  }
  const geometry_msgs::msg::PoseStamped ps = move_group_->getCurrentPose();
  const auto &p = ps.pose.position;

  // Quaternion -> roll/pitch/yaw. tf2's getRPY is the exact inverse of the
  // rpy->quaternion conversion the pose loader uses (_api.py _load_poses_from_
  // file), so a captured pose round-trips back to the same orientation.
  tf2::Quaternion q(ps.pose.orientation.x, ps.pose.orientation.y,
                    ps.pose.orientation.z, ps.pose.orientation.w);
  double roll = 0.0, pitch = 0.0, yaw = 0.0;
  tf2::Matrix3x3(q).getRPY(roll, pitch, yaw);
  const double kRadToDeg = 180.0 / M_PI;

  // Format as one scan_poses.json entry, ready to paste into search_poses.json /
  // idle_scan_poses.json (position in metres, rpy in DEGREES).
  char buf[256];
  std::snprintf(
      buf, sizeof(buf),
      "{\"position\": {\"x\": %.4f, \"y\": %.4f, \"z\": %.4f}, "
      "\"rpy\": {\"r\": %.1f, \"p\": %.1f, \"y\": %.1f}}",
      p.x, p.y, p.z, roll * kRadToDeg, pitch * kRadToDeg, yaw * kRadToDeg);

  RCLCPP_INFO(node_->get_logger(), "get_scan_pose (frame=%s): %s",
              ps.header.frame_id.c_str(), buf);
  response->success = true;
  response->message = buf;
  return true;
}

bool CrackleManipulation::demo_trajectory_service(
    crackle_interfaces::srv::DemoTrajectory::Request::SharedPtr request,
    crackle_interfaces::srv::DemoTrajectory::Response::SharedPtr response) {
  if (!wait_for_current_state("demo_trajectory")) {
    response->success = false;
    response->message = "No current state";
    return true;
  }

  const std::string type = request->type;
  const double size = request->size > 0.0 ? request->size : 0.03;
  const double height = request->height;
  const int points = request->points >= 4 ? request->points : 40;
  constexpr double kPi = 3.141592653589793;

  geometry_msgs::msg::PoseStamped current_pose = move_group_->getCurrentPose();
  const geometry_msgs::msg::Pose base_pose = current_pose.pose;

  std::vector<geometry_msgs::msg::Pose> waypoints;
  waypoints.reserve(static_cast<size_t>(points) + 1);

  if (type == "circle_xy") {
    for (int i = 0; i <= points; ++i) {
      const double t =
          2.0 * kPi * static_cast<double>(i) / static_cast<double>(points);
      geometry_msgs::msg::Pose p = base_pose;
      p.position.x += size * std::cos(t);
      p.position.y += size * std::sin(t);
      waypoints.push_back(p);
    }
  } else if (type == "square_xy") {
    const double d = size;
    std::vector<geometry_msgs::msg::Point> corners(5);
    corners[0].x = base_pose.position.x + d;
    corners[0].y = base_pose.position.y + d;
    corners[0].z = base_pose.position.z;
    corners[1].x = base_pose.position.x - d;
    corners[1].y = base_pose.position.y + d;
    corners[1].z = base_pose.position.z;
    corners[2].x = base_pose.position.x - d;
    corners[2].y = base_pose.position.y - d;
    corners[2].z = base_pose.position.z;
    corners[3].x = base_pose.position.x + d;
    corners[3].y = base_pose.position.y - d;
    corners[3].z = base_pose.position.z;
    corners[4] = corners[0];

    for (const auto &c : corners) {
      geometry_msgs::msg::Pose p = base_pose;
      p.position = c;
      waypoints.push_back(p);
    }
  } else if (type == "figure8_xy") {
    for (int i = 0; i <= points; ++i) {
      const double t =
          2.0 * kPi * static_cast<double>(i) / static_cast<double>(points);
      geometry_msgs::msg::Pose p = base_pose;
      p.position.x += size * std::sin(t);
      p.position.y += size * std::sin(t) * std::cos(t);
      waypoints.push_back(p);
    }
  } else if (type == "helix_z") {
    for (int i = 0; i <= points; ++i) {
      const double t =
          2.0 * kPi * static_cast<double>(i) / static_cast<double>(points);
      geometry_msgs::msg::Pose p = base_pose;
      p.position.x += size * std::cos(t);
      p.position.y += size * std::sin(t);
      p.position.z +=
          height * (static_cast<double>(i) / static_cast<double>(points));
      waypoints.push_back(p);
    }
  } else if (type == "heart_xy") {
    // Classic parametric heart curve, normalized so its half-width is `size`.
    for (int i = 0; i <= points; ++i) {
      const double t =
          2.0 * kPi * static_cast<double>(i) / static_cast<double>(points);
      const double hx = 16.0 * std::pow(std::sin(t), 3);
      const double hy = 13.0 * std::cos(t) - 5.0 * std::cos(2.0 * t) -
                        2.0 * std::cos(3.0 * t) - std::cos(4.0 * t);
      geometry_msgs::msg::Pose p = base_pose;
      p.position.x += size * (hx / 16.0);
      p.position.y += size * (hy / 16.0);
      waypoints.push_back(p);
    }
  } else if (type == "star_xy") {
    // 5-pointed star: 10 vertices alternating outer radius `size` and inner
    // radius 0.4*size, connected by straight edges. Starts pointing up (+Y).
    constexpr int kTips = 5;
    const double inner = size * 0.4;
    for (int i = 0; i <= 2 * kTips; ++i) {
      const double r = (i % 2 == 0) ? size : inner;
      const double a = kPi / 2.0 + kPi * static_cast<double>(i) /
                                       static_cast<double>(kTips);
      geometry_msgs::msg::Pose p = base_pose;
      p.position.x += r * std::cos(a);
      p.position.y += r * std::sin(a);
      waypoints.push_back(p);
    }
  } else {
    response->success = false;
    response->message =
        "Unknown type. Use circle_xy, square_xy, figure8_xy, helix_z, "
        "heart_xy, star_xy";
    return true;
  }

  bool plan_ok = plan_cartesian_path(waypoints);
  if (!plan_ok) {
    response->success = false;
    response->message = "Planning failed";
    return true;
  }

  if (request->execute) {
    bool exec_ok = execute_plan(true);
    response->success = exec_ok;
    response->message = exec_ok ? "Executed" : "Execution failed";
    return true;
  }

  response->success = true;
  response->message = "Planned";
  return true;
}

bool CrackleManipulation::pick_up_object(
    crackle_interfaces::srv::PickupObject::Request::SharedPtr request,
    crackle_interfaces::srv::PickupObject::Response::SharedPtr response) {
  const std::string &object_name = request->object_name;
  RCLCPP_INFO(node_->get_logger(), "pick_up_object: requested to pick up '%s'",
              object_name.c_str());

  if (!wait_for_current_state("pick_up_object")) {
    response->success = false;
    return true;
  }

  // ---- Locate object in planning scene ----------------------------------------
  auto scene_objects = planning_scene_->getObjects({object_name});
  if (scene_objects.find(object_name) == scene_objects.end()) {
    RCLCPP_ERROR(node_->get_logger(),
                 "pick_up_object: object '%s' not found in planning scene",
                 object_name.c_str());
    response->success = false;
    return true;
  }
  const moveit_msgs::msg::CollisionObject &obj = scene_objects[object_name];

  // ---- Generate grasp candidates -----------------------------------------------
  // Gripper offset from the object = the grip-point offset shown by the rviz
  // marker, so what the operator sees is exactly where the EEF is placed.
  constexpr double approach_dist = kGripPointOffset;
  constexpr double pregrasp_dist = 0.20; // metres – retreat along approach axis
  constexpr double lift_dist = 0.25;     // metres – straight-up post-grasp lift
  constexpr double tool_width = 0.10; // gripper finger opening (max graspable width)

  clear_grasp_markers();
  std::vector<int> face_ids;
  std::vector<geometry_msgs::msg::Pose> grasp_candidates =
      get_grasp_poses(obj, approach_dist, tool_width, &face_ids);

  // Prepend a hard-coded top-down grasp (approach along world -Z) so it's
  // always tried first regardless of object orientation, and is visible in
  // the rviz markers. Uses face_id=-1 as a sentinel — std::map orders that
  // key before any face-index key so the pickup loop hits this first.
  {
    Eigen::Quaterniond q_obj(
        obj.pose.orientation.w, obj.pose.orientation.x,
        obj.pose.orientation.y, obj.pose.orientation.z);
    if (q_obj.norm() < 1e-6) q_obj = Eigen::Quaterniond::Identity();
    const Eigen::Matrix3d R_obj = q_obj.normalized().toRotationMatrix();

    Eigen::Vector3d he(0.05, 0.05, 0.05);
    if (!obj.primitives.empty()) {
      using P = shape_msgs::msg::SolidPrimitive;
      const auto &prim = obj.primitives[0];
      switch (prim.type) {
      case P::BOX:
        if (prim.dimensions.size() >= 3)
          he = {prim.dimensions[P::BOX_X] / 2.0,
                prim.dimensions[P::BOX_Y] / 2.0,
                prim.dimensions[P::BOX_Z] / 2.0};
        break;
      case P::CYLINDER:
        if (prim.dimensions.size() >= 2) {
          const double r = prim.dimensions[P::CYLINDER_RADIUS];
          he = {r, r, prim.dimensions[P::CYLINDER_HEIGHT] / 2.0};
        }
        break;
      case P::SPHERE:
        if (!prim.dimensions.empty()) {
          const double r = prim.dimensions[P::SPHERE_RADIUS];
          he = {r, r, r};
        }
        break;
      default:
        break;
      }
    }
    // World-AABB half-height of a rotated box.
    const double dz = std::abs(R_obj(2, 0)) * he.x() +
                      std::abs(R_obj(2, 1)) * he.y() +
                      std::abs(R_obj(2, 2)) * he.z();

    // Only offer the top-down grasp if the object's narrower horizontal span
    // fits the jaws, and roll the gripper to close along that narrower axis.
    const double hx_w = std::abs(R_obj(0, 0)) * he.x() +
                        std::abs(R_obj(0, 1)) * he.y() +
                        std::abs(R_obj(0, 2)) * he.z();
    const double hy_w = std::abs(R_obj(1, 0)) * he.x() +
                        std::abs(R_obj(1, 1)) * he.y() +
                        std::abs(R_obj(1, 2)) * he.z();
    const bool narrow_x = (hx_w <= hy_w);
    const double top_grasp_width = 2.0 * (narrow_x ? hx_w : hy_w);
    if (top_grasp_width <= tool_width) {
      geometry_msgs::msg::Pose top_down;
      top_down.position.x = obj.pose.position.x;
      top_down.position.y = obj.pose.position.y;
      top_down.position.z = obj.pose.position.z + dz + approach_dist;
      top_down.orientation = lookAtQuat(
          Eigen::Vector3d(0.0, 0.0, -1.0),
          narrow_x ? Eigen::Vector3d::UnitX() : Eigen::Vector3d::UnitY(),
          kToolForwardInTool);
      grasp_candidates.insert(grasp_candidates.begin(), top_down);
      face_ids.insert(face_ids.begin(), -1);
    }
  }

  if (grasp_candidates.empty()) {
    RCLCPP_ERROR(node_->get_logger(),
                 "pick_up_object: no grasp candidates generated for '%s'",
                 object_name.c_str());
    response->success = false;
    return true;
  }

  // Status vector shared with the marker publisher: 0=proposed, 1=planning,
  // 2=feasible, 3=infeasible, 4=selected, 5=skipped. Republish after each
  // status change so we get MTC-style stepwise feedback in rviz.
  std::vector<int> statuses(grasp_candidates.size(), 0);
  publish_grasp_markers(grasp_candidates, statuses);

  // Open gripper before planning so the scene state is correct
  set_gripper_blocking(false);

  // Execute a fully-planned pickup (pregrasp joint move → Cartesian approach →
  // close gripper + attach → Cartesian lift). Sets response->success and returns
  // true when the caller should return from pick_up_object (either because
  // execution completed or an execution phase failed).
  auto execute_pickup = [&](PickPhases &phases) -> bool {
    plan_ = phases.pregrasp_plan;
    is_trajectory_ = false;
    if (!execute_plan(true)) {
      RCLCPP_ERROR(node_->get_logger(),
                   "pick_up_object: pregrasp execution failed");
      response->success = false;
      return true;
    }

    trajectory_ = phases.approach_traj;
    is_trajectory_ = true;
    if (!execute_plan(true)) {
      RCLCPP_ERROR(node_->get_logger(),
                   "pick_up_object: approach execution failed");
      response->success = false;
      return true;
    }

    // Close the gripper and wait until the firmware reports it is closed
    // before attaching/lifting.
    set_gripper_blocking(true);

    moveit_msgs::msg::AttachedCollisionObject attached;
    attached.link_name = "gripper_base";
    attached.object = obj;
    attached.object.operation = moveit_msgs::msg::CollisionObject::ADD;
    attached.touch_links = {"gripper_base"};
    planning_scene_->applyAttachedCollisionObject(attached);

    trajectory_ = phases.lift_traj;
    is_trajectory_ = true;
    if (!execute_plan(true)) {
      RCLCPP_ERROR(node_->get_logger(),
                   "pick_up_object: lift execution failed");
      response->success = false;
      return true;
    }

    RCLCPP_INFO(node_->get_logger(),
                "pick_up_object: successfully picked up '%s'",
                object_name.c_str());
    response->success = true;
    return true;
  };

  // ---- Plan phase by face: try candidates in order and return as soon as one
  //      plans successfully. The top-down probe sits at face_id=-1 and is hit
  //      first thanks to std::map ordering.
  std::map<int, std::vector<size_t>> by_face;
  for (size_t i = 0; i < grasp_candidates.size(); ++i) {
    const int fid = (i < face_ids.size()) ? face_ids[i] : -1;
    by_face[fid].push_back(i);
  }

  for (auto &kv : by_face) {
    const int fid = kv.first;
    const std::vector<size_t> &indices = kv.second;
    RCLCPP_INFO(node_->get_logger(),
                "pick_up_object: face %d – %zu candidate(s)", fid,
                indices.size());

    for (size_t pos = 0; pos < indices.size(); ++pos) {
      const size_t i = indices[pos];

      const geometry_msgs::msg::Pose &grasp = grasp_candidates[i];
      geometry_msgs::msg::Pose pregrasp = compute_pregrasp(grasp, pregrasp_dist);
      geometry_msgs::msg::Pose lift = grasp;
      lift.position.z += lift_dist;

      RCLCPP_INFO(node_->get_logger(),
                  "pick_up_object: face %d, candidate %zu/%zu ...", fid,
                  pos + 1, indices.size());
      statuses[i] = 1;
      publish_grasp_markers(grasp_candidates, statuses);

      PickPhases phases;
      if (plan_pickup_phases(pregrasp, grasp, lift, phases)) {
        RCLCPP_INFO(node_->get_logger(),
                    "pick_up_object: face %d satisfied on candidate %zu, "
                    "score=%.4f — executing",
                    fid, pos + 1, phases.score);
        statuses[i] = 4;
        publish_grasp_markers(grasp_candidates, statuses);
        execute_pickup(phases);
        return true;
      }
      statuses[i] = 3;
      publish_grasp_markers(grasp_candidates, statuses);
    }
  }

  RCLCPP_ERROR(node_->get_logger(),
               "pick_up_object: no feasible grasp found for '%s'",
               object_name.c_str());
  response->success = false;
  return true;
}

// ---------------------------------------------------------------------------
// MTC-based pickup pipeline
// ---------------------------------------------------------------------------

namespace mtc = moveit::task_constructor;

// Attach link the object is bound to when grasped (matches legacy pick_up_object).
static const char *kAttachLink = "gripper_base";

bool CrackleManipulation::build_manipulation_task(
    const std::string &object_id,
    const std::vector<geometry_msgs::msg::Pose> &target_poses,
    double approach_move, double retreat_move, bool attach, mtc::Task &task,
    const std::vector<std::string> &allow_object_collisions) {
  const std::string arm_group = move_group_->getName();
  const std::string ik_frame = move_group_->getEndEffectorLink();
  const std::string planning_frame = move_group_->getPlanningFrame();

  task.stages()->setName((attach ? "pick_" : "place_") + object_id);
  // Reuse the move group's stable robot model instance rather than loading a
  // fresh one each call. loadRobotModel() would create a new model every time,
  // which then mismatches the cached PipelinePlanner (initialised with the
  // first call's model) and aborts planning on the second task.
  task.setRobotModel(move_group_->getRobotModel());

  // ---- Solvers (lazy, reused across calls) ----------------------------------
  if (!mtc_sampling_planner_) {
    mtc_sampling_planner_ =
        std::make_shared<mtc::solvers::PipelinePlanner>(node_, "ompl");
  }
  mtc_sampling_planner_->setPlannerId("RRTConnect");
  mtc_sampling_planner_->setMaxVelocityScalingFactor(max_velocity_scaling_factor);
  mtc_sampling_planner_->setMaxAccelerationScalingFactor(
      max_acceleration_scaling_factor);

  if (!mtc_cartesian_planner_) {
    mtc_cartesian_planner_ = std::make_shared<mtc::solvers::CartesianPath>();
  }
  mtc_cartesian_planner_->setMaxVelocityScalingFactor(max_velocity_scaling_factor);
  mtc_cartesian_planner_->setMaxAccelerationScalingFactor(
      max_acceleration_scaling_factor);
  mtc_cartesian_planner_->setStepSize(0.005); // 5 mm – far coarser than legacy 1 mm
  mtc_cartesian_planner_->setJumpThreshold(0.0);
  // Accept a partially-completed straight-line approach/retreat. On this small
  // arm, grasping objects near the base/table, a fixed-orientation Cartesian
  // move often can only progress a couple of cm before a joint limit; the
  // per-stage min_distance (below) enforces the real usable minimum, so keep the
  // solver's fraction gate low enough that those short-but-valid moves survive.
  mtc_cartesian_planner_->setMinFraction(0.1);

  // Dedicated approach planner: same settings, but demands a near-complete
  // straight line so the pre-grasp insertion actually happens as a controlled
  // move from a standoff (fingers don't clip the object on the way in).
  if (!mtc_approach_planner_) {
    mtc_approach_planner_ = std::make_shared<mtc::solvers::CartesianPath>();
  }
  mtc_approach_planner_->setMaxVelocityScalingFactor(max_velocity_scaling_factor);
  mtc_approach_planner_->setMaxAccelerationScalingFactor(
      max_acceleration_scaling_factor);
  mtc_approach_planner_->setStepSize(0.005);
  mtc_approach_planner_->setJumpThreshold(0.0);
  // Demand most of the approach as a straight line (a real controlled insertion),
  // but not so much that this small arm can't reach the standoff — the earlier
  // Cartesian-reach struggles were on the retreat, and the approach descends onto
  // the object which is the easier direction.
  mtc_approach_planner_->setMinFraction(0.7);

  // ---- 1. Current state -----------------------------------------------------
  mtc::Stage *current_state_ptr = nullptr;
  {
    auto current = std::make_unique<mtc::stages::CurrentState>("current");
    current_state_ptr = current.get();
    task.add(std::move(current));
  }

  // ---- 2. Connect current -> pre-grasp (free-space, RRTConnect) -------------
  {
    auto connect = std::make_unique<mtc::stages::Connect>(
        "move to pre-grasp",
        mtc::stages::Connect::GroupPlannerVector{{arm_group, mtc_sampling_planner_}});
    connect->setTimeout(5.0);
    task.add(std::move(connect));
  }

  // ---- 3. Serial: approach -> target IK -> attach/detach -> retreat ---------
  {
    auto seq = std::make_unique<mtc::SerialContainer>(attach ? "pick" : "place");

    // 3-. Allow the gripper to touch THIS object during the approach and grasp.
    //     The gripper links now carry collision geometry (so they can't pass
    //     through the table), which means the grasp pose — fingers wrapped
    //     around the object — would otherwise be rejected as a collision. This
    //     whitelists only object<->gripper contact; the table and all other
    //     obstacles stay collision-checked. The modified ACM propagates forward
    //     to the IK and retreat stages.
    {
      auto allow = std::make_unique<mtc::stages::ModifyPlanningScene>(
          "allow gripper-object collision");
      allow->allowCollisions(
          object_id,
          std::vector<std::string>{"gripper_base", "left_rack", "right_rack"},
          true);
      seq->insert(std::move(allow));
    }

    // 3-. Let the TARGET object overlap other collision geometry. Perception
    //     meshes are imperfect (partial clouds), so the object commonly clips the
    //     table it rests on or a duplicate/neighbor detection — which would make
    //     the IK/attach stages reject the grasp ("<obj> colliding with <other>")
    //     even though those overlaps are pre-existing data artifacts, not caused
    //     by the motion. Whitelisting the object<->world contacts here does NOT
    //     relax gripper<->world checking, so the gripper still can't hit the
    //     table or other objects. For place this includes the support surface so
    //     setting the object down (near-contact) isn't rejected.
    if (!allow_object_collisions.empty()) {
      auto allow_world = std::make_unique<mtc::stages::ModifyPlanningScene>(
          "allow object-world collision");
      allow_world->allowCollisions(object_id, allow_object_collisions, true);
      seq->insert(std::move(allow_world));
    }

    // 3a. Cartesian approach along the tool-forward (+Z) axis toward the target.
    //     The IK target sits at the grasp; this stage is solved backward from it,
    //     so the free-space Connect delivers the gripper to a standoff
    //     `approach_move` behind the grasp and this stage drives a straight line
    //     the rest of the way in. Uses the strict approach planner and requires
    //     most of `approach_move`, so it's a genuine controlled insertion from a
    //     standoff (fingers approach straight and don't knock the object) rather
    //     than a token nudge. For a top-down target this descends onto the object.
    {
      auto approach =
          std::make_unique<mtc::stages::MoveRelative>("approach", mtc_approach_planner_);
      approach->setGroup(arm_group);
      approach->setIKFrame(ik_frame);
      approach->setMinMaxDistance(approach_move * 0.6, approach_move);
      geometry_msgs::msg::Vector3Stamped dir;
      dir.header.frame_id = ik_frame; // tool frame
      dir.vector.z = 1.0;             // +Z = tool forward, toward the target
      approach->setDirection(dir);
      seq->insert(std::move(approach));
    }

    // 3b. Target-pose generator (all candidates) wrapped in ComputeIK. IK +
    //     collision checking here prunes unreachable / colliding poses cheaply
    //     and MTC ranks the survivors by cost.
    {
      auto gen = std::make_unique<mtc::stages::FixedCartesianPoses>("target poses");
      gen->setMonitoredStage(current_state_ptr);
      for (const auto &p : target_poses) {
        geometry_msgs::msg::PoseStamped ps;
        ps.header.frame_id = planning_frame;
        ps.pose = p;
        gen->addPose(ps);
      }

      auto ik = std::make_unique<mtc::stages::ComputeIK>("target IK", std::move(gen));
      ik->setGroup(arm_group);
      ik->setIKFrame(ik_frame);
      ik->setMaxIKSolutions(4);
      ik->setIgnoreCollisions(false);
      // The target pose is supplied per-candidate by the generator on the
      // interface state; ComputeIK must be told to source it from there.
      ik->properties().configureInitFrom(mtc::Stage::INTERFACE, {"target_pose"});
      seq->insert(std::move(ik));
    }

    // 3c. Attach (pick) or detach (place) the object so the retreat is planned
    //     collision-correctly. On attach, RE-ASSERT the object's collision
    //     allowances in the SAME stage. Attaching turns the world object into a
    //     new attached body, and the object<->world / object<->gripper allowances
    //     we set before the approach do NOT carry over onto it. The object still
    //     rests on (and its perception mesh intersects) the table, so those edits
    //     must be applied together with the attach — otherwise the attach stage's
    //     own validity check rejects it ("attach object: red_box colliding with
    //     table"), and even if it passed the retreat would immediately be in
    //     collision. Doing both in one ModifyPlanningScene means the allowances
    //     are live when the attached state is checked, and they propagate to the
    //     retreat.
    {
      auto modify =
          std::make_unique<mtc::stages::ModifyPlanningScene>(attach ? "attach object"
                                                                    : "detach object");
      if (attach) {
        modify->attachObject(object_id, kAttachLink);
        modify->allowCollisions(
            object_id,
            std::vector<std::string>{"gripper_base", "left_rack", "right_rack"},
            true);
        if (!allow_object_collisions.empty())
          modify->allowCollisions(object_id, allow_object_collisions, true);
      } else {
        modify->detachObject(object_id, kAttachLink);
      }
      seq->insert(std::move(modify));
    }

    // 3d. Cartesian retreat straight up (world +Z). Ask for `retreat_move`, but
    //     accept as little as kMinRetreat: on this small arm a fixed-orientation
    //     straight-up lift from a low grasp frequently maxes out at ~2 cm before
    //     a joint limit. That's still enough to break table contact and hand off
    //     to the next (free-space, full-DOF) motion, which does the real lifting.
    //     Requiring the full `retreat_move` here just makes every otherwise-good
    //     grasp fail ("retreat: min_distance not reached").
    {
      constexpr double kMinRetreat = 0.015; // metres – smallest useful lift
      auto retreat =
          std::make_unique<mtc::stages::MoveRelative>("retreat", mtc_cartesian_planner_);
      retreat->setGroup(arm_group);
      retreat->setIKFrame(ik_frame);
      retreat->setMinMaxDistance(kMinRetreat, retreat_move);
      geometry_msgs::msg::Vector3Stamped dir;
      dir.header.frame_id = planning_frame; // world
      dir.vector.z = 1.0;
      retreat->setDirection(dir);
      seq->insert(std::move(retreat));
    }

    task.add(std::move(seq));
  }

  return true;
}

bool CrackleManipulation::execute_manipulation_solution(
    const mtc::SolutionBase &solution,
    const moveit_msgs::msg::CollisionObject &obj, bool is_pick, std::string &err) {
  moveit_task_constructor_msgs::msg::Solution msg;
  solution.toMsg(msg, nullptr);

  // Collect the trajectory-bearing segments in execution order. Generator and
  // scene-modification stages contribute empty trajectories, which we skip.
  // The ordered result is [connect (current->pre), approach (->target),
  // retreat (->up)]; the final segment is always the retreat.
  std::vector<moveit_msgs::msg::RobotTrajectory> segments;
  for (const auto &st : msg.sub_trajectory) {
    if (!st.trajectory.joint_trajectory.points.empty())
      segments.push_back(st.trajectory);
  }
  if (segments.empty()) {
    err = "solution contained no executable trajectory";
    return false;
  }

  // For pick, open the gripper before moving so the scene state is correct.
  // For place, the object is held, so leave the gripper closed until release.
  if (is_pick)
    set_gripper_blocking(false);

  // Execute everything up to (but not including) the retreat: reach the target.
  for (size_t i = 0; i + 1 < segments.size(); ++i) {
    trajectory_ = segments[i];
    is_trajectory_ = true;
    if (!execute_plan(true)) {
      err = "reach segment " + std::to_string(i) + " failed";
      return false;
    }
  }

  if (is_pick) {
    // Close the gripper and wait until it reports fully closed BEFORE lifting.
    // Bail out if the close doesn't confirm, so we never retreat with the object
    // still ungrasped (which looked like "it lifted before the gripper closed").
    if (!set_gripper_blocking(true)) {
      err = "gripper failed to confirm close before retreat";
      return false;
    }

        moveit_msgs::msg::AttachedCollisionObject attached;
    attached.link_name = kAttachLink;
    attached.object = obj;
    attached.object.operation = moveit_msgs::msg::CollisionObject::ADD;
    // The object is held between the jaw racks, so all three gripper links must
    // be touch links or the retreat lift trips an object<->jaw collision.
    attached.touch_links = {kAttachLink, "left_rack", "right_rack"};
    planning_scene_->applyAttachedCollisionObject(attached);

    // Give the jaws time to fully seat on the object after the close ack, before
    // attaching and lifting — the firmware "DONE" can arrive ahead of the fingers
    // mechanically finishing their travel, so wait a full few seconds.
    constexpr auto kPostCloseSettle = std::chrono::seconds(5);
    std::this_thread::sleep_for(kPostCloseSettle);


  } else {
    // Open the gripper and detach the object from the real planning scene.
    set_gripper_blocking(false);

    moveit_msgs::msg::AttachedCollisionObject detach;
    detach.object.id = obj.id;
    detach.object.operation = moveit_msgs::msg::CollisionObject::REMOVE;
    planning_scene_->applyAttachedCollisionObject(detach);
  }

  // Execute the retreat.
  trajectory_ = segments.back();
  is_trajectory_ = true;
  if (!execute_plan(true)) {
    err = "retreat segment failed";
    return false;
  }
  return true;
}

bool CrackleManipulation::pick_up_object_mtc(
    crackle_interfaces::srv::PickupObject::Request::SharedPtr request,
    crackle_interfaces::srv::PickupObject::Response::SharedPtr response) {
  const std::string &object_name = request->object_name;
  RCLCPP_INFO(node_->get_logger(),
              "pick_up_object_mtc: requested to pick up '%s'",
              object_name.c_str());

  if (!wait_for_current_state("pick_up_object_mtc")) {
    response->success = false;
    return true;
  }

  // ---- Locate object in planning scene --------------------------------------
  auto scene_objects = planning_scene_->getObjects({object_name});
  if (scene_objects.find(object_name) == scene_objects.end()) {
    RCLCPP_ERROR(node_->get_logger(),
                 "pick_up_object_mtc: object '%s' not found in planning scene",
                 object_name.c_str());
    response->success = false;
    return true;
  }
  const moveit_msgs::msg::CollisionObject &obj = scene_objects[object_name];

  // ---- Generate grasp candidates (reuses the legacy geometry) ---------------
  constexpr double approach_dist = kGripPointOffset; // gripper offset baked into grasp pose
  constexpr double tool_width = 0.10; // gripper finger opening (max graspable width)

  clear_grasp_markers();
  std::vector<int> face_ids;
  std::vector<geometry_msgs::msg::Pose> grasp_candidates =
      get_grasp_poses(obj, approach_dist, tool_width, &face_ids);

  // Prepend a hard-coded top-down grasp so it's always among the candidates and
  // visible in rviz (identical to the legacy pipeline).
  {
    Eigen::Quaterniond q_obj(obj.pose.orientation.w, obj.pose.orientation.x,
                             obj.pose.orientation.y, obj.pose.orientation.z);
    if (q_obj.norm() < 1e-6) q_obj = Eigen::Quaterniond::Identity();
    const Eigen::Matrix3d R_obj = q_obj.normalized().toRotationMatrix();

    Eigen::Vector3d he(0.05, 0.05, 0.05);
    if (!obj.primitives.empty()) {
      using P = shape_msgs::msg::SolidPrimitive;
      const auto &prim = obj.primitives[0];
      switch (prim.type) {
      case P::BOX:
        if (prim.dimensions.size() >= 3)
          he = {prim.dimensions[P::BOX_X] / 2.0, prim.dimensions[P::BOX_Y] / 2.0,
                prim.dimensions[P::BOX_Z] / 2.0};
        break;
      case P::CYLINDER:
        if (prim.dimensions.size() >= 2) {
          const double r = prim.dimensions[P::CYLINDER_RADIUS];
          he = {r, r, prim.dimensions[P::CYLINDER_HEIGHT] / 2.0};
        }
        break;
      case P::SPHERE:
        if (!prim.dimensions.empty()) {
          const double r = prim.dimensions[P::SPHERE_RADIUS];
          he = {r, r, r};
        }
        break;
      default:
        break;
      }
    }
    const double dz = std::abs(R_obj(2, 0)) * he.x() +
                      std::abs(R_obj(2, 1)) * he.y() +
                      std::abs(R_obj(2, 2)) * he.z();

    // Only offer the top-down grasp if the object's narrower horizontal span
    // fits the jaws, and roll the gripper to close along that narrower axis.
    const double hx_w = std::abs(R_obj(0, 0)) * he.x() +
                        std::abs(R_obj(0, 1)) * he.y() +
                        std::abs(R_obj(0, 2)) * he.z();
    const double hy_w = std::abs(R_obj(1, 0)) * he.x() +
                        std::abs(R_obj(1, 1)) * he.y() +
                        std::abs(R_obj(1, 2)) * he.z();
    const bool narrow_x = (hx_w <= hy_w);
    const double top_grasp_width = 2.0 * (narrow_x ? hx_w : hy_w);
    if (top_grasp_width <= tool_width) {
      geometry_msgs::msg::Pose top_down;
      top_down.position.x = obj.pose.position.x;
      top_down.position.y = obj.pose.position.y;
      top_down.position.z = obj.pose.position.z + dz + approach_dist;
      top_down.orientation = lookAtQuat(
          Eigen::Vector3d(0.0, 0.0, -1.0),
          narrow_x ? Eigen::Vector3d::UnitX() : Eigen::Vector3d::UnitY(),
          kToolForwardInTool);
      grasp_candidates.insert(grasp_candidates.begin(), top_down);
      face_ids.insert(face_ids.begin(), -1);
    }
  }

  if (grasp_candidates.empty()) {
    RCLCPP_ERROR(node_->get_logger(),
                 "pick_up_object_mtc: no grasp candidates generated for '%s'",
                 object_name.c_str());
    response->success = false;
    return true;
  }

  std::vector<int> statuses(grasp_candidates.size(), 0);
  publish_grasp_markers(grasp_candidates, statuses);

  // ---- Build + plan the MTC task --------------------------------------------
  // Distances mirror the legacy pipeline: approach = pregrasp retreat closed by
  // the Cartesian move (0.20); retreat = straight-up post-grasp lift (0.25).
  // Let the target object overlap the table and any other scene objects
  // (duplicate/neighbor detections) so a slightly-oversized perception mesh
  // doesn't self-reject the grasp. The gripper is still checked against them.
  std::vector<std::string> allow_object_touch;
  for (const auto &name : planning_scene_->getKnownObjectNames())
    if (name != object_name)
      allow_object_touch.push_back(name);

  // Approach standoff = 0.12 m: far enough back that the gripper opens clear of
  // the object and comes straight in (see the strict approach planner), so the
  // fingers don't knock it, but still short enough for this small arm to reach
  // the pre-grasp near the table.
  mtc::Task task;
  if (!build_manipulation_task(object_name, grasp_candidates, /*approach_move=*/0.12,
                               /*retreat_move=*/0.12, /*attach=*/true, task,
                               allow_object_touch)) {
    RCLCPP_ERROR(node_->get_logger(),
                 "pick_up_object_mtc: failed to build task for '%s'",
                 object_name.c_str());
    response->success = false;
    return true;
  }

  try {
    const size_t kMaxSolutions = 5;
    moveit::core::MoveItErrorCode rc = task.plan(kMaxSolutions);
    if (!rc || task.solutions().empty()) {
      RCLCPP_ERROR(node_->get_logger(),
                   "pick_up_object_mtc: no feasible grasp found for '%s'",
                   object_name.c_str());
      response->success = false;
      return true;
    }
  } catch (const std::exception &e) {
    RCLCPP_ERROR(node_->get_logger(),
                 "pick_up_object_mtc: planning failed for '%s': %s",
                 object_name.c_str(), e.what());
    response->success = false;
    return true;
  }

  RCLCPP_INFO(node_->get_logger(),
              "pick_up_object_mtc: %zu solution(s) found, executing best "
              "(cost=%.4f)",
              task.solutions().size(), task.solutions().front()->cost());

  // Repaint the grasp markers: GREEN for every candidate MTC found a feasible
  // solution for, MAGENTA for the lowest-cost one we're about to execute — so
  // rviz shows exactly which grasp points work and which one is chosen.
  {
    const auto &model = move_group_->getRobotModel();
    const std::string ik_frame = move_group_->getEndEffectorLink();
    std::vector<int> statuses(grasp_candidates.size(), 0);
    auto nearest_candidate = [&](const Eigen::Vector3d &p) {
      int best = -1;
      double best_d = std::numeric_limits<double>::max();
      for (size_t i = 0; i < grasp_candidates.size(); ++i) {
        const auto &c = grasp_candidates[i].position;
        const double d = (Eigen::Vector3d(c.x, c.y, c.z) - p).norm();
        if (d < best_d) { best_d = d; best = static_cast<int>(i); }
      }
      return best;
    };
    int best_idx = -1;
    for (const auto &sol : task.solutions()) {
      moveit_task_constructor_msgs::msg::Solution smsg;
      sol->toMsg(smsg, nullptr);
      // Grasp waypoint = end of the approach segment = the last trajectory-
      // bearing sub-solution before the (final) retreat segment.
      std::vector<const moveit_msgs::msg::RobotTrajectory *> segs;
      for (const auto &st : smsg.sub_trajectory)
        if (!st.trajectory.joint_trajectory.points.empty())
          segs.push_back(&st.trajectory);
      if (segs.size() < 2) continue;
      const moveit::core::RobotState grasp_state =
          state_from_trajectory_end(*segs[segs.size() - 2], model);
      const Eigen::Vector3d grasp_p =
          grasp_state.getGlobalLinkTransform(ik_frame).translation();
      const int idx = nearest_candidate(grasp_p);
      if (idx < 0) continue;
      if (best_idx < 0) best_idx = idx;          // solutions are cost-sorted
      if (statuses[idx] != 4) statuses[idx] = 2; // feasible = green
    }
    if (best_idx >= 0) statuses[best_idx] = 4;   // selected = magenta
    publish_grasp_markers(grasp_candidates, statuses);
  }

  // ---- Execute the lowest-cost solution -------------------------------------
  std::string err;
  if (!execute_manipulation_solution(*task.solutions().front(), obj,
                                     /*is_pick=*/true, err)) {
    RCLCPP_ERROR(node_->get_logger(),
                 "pick_up_object_mtc: execution failed for '%s': %s",
                 object_name.c_str(), err.c_str());
    response->success = false;
    return true;
  }

  // Record that the arm is now holding this object so place_object can act on
  // it without being told the object name.
  holding_object_ = true;
  held_object_ = obj;

  RCLCPP_INFO(node_->get_logger(),
              "pick_up_object_mtc: successfully picked up '%s'",
              object_name.c_str());
  response->success = true;
  return true;
}

bool CrackleManipulation::place_object_mtc(
    crackle_interfaces::srv::PlaceObject::Request::SharedPtr request,
    crackle_interfaces::srv::PlaceObject::Response::SharedPtr response) {
  const std::string &table_name =
      request->table_name.empty() ? "table" : request->table_name;

  if (!wait_for_current_state("place_object_mtc")) {
    response->success = false;
    return true;
  }

  // ---- Act on the held object tracked by node state --------------------------
  if (!holding_object_) {
    RCLCPP_ERROR(node_->get_logger(),
                 "place_object_mtc: the arm is not holding anything");
    response->success = false;
    return true;
  }
  const moveit_msgs::msg::CollisionObject held_obj = held_object_;
  const std::string object_name = held_obj.id;

  RCLCPP_INFO(node_->get_logger(), "place_object_mtc: placing '%s' on '%s'",
              object_name.c_str(), table_name.c_str());

  // ---- Discover free spots on the table -------------------------------------
  std::vector<geometry_msgs::msg::Pose> place_candidates =
      find_place_poses_on_table(object_name, table_name);
  if (place_candidates.empty()) {
    RCLCPP_ERROR(node_->get_logger(),
                 "place_object_mtc: no free spots found on '%s'",
                 table_name.c_str());
    response->success = false;
    return true;
  }

  clear_grasp_markers();
  std::vector<int> statuses(place_candidates.size(), 0);
  publish_grasp_markers(place_candidates, statuses);

  // ---- Build + plan the MTC task --------------------------------------------
  // Distances mirror the legacy place pipeline: approach = pre-place descent
  // (0.15); retreat = post-release retreat straight up (0.20).
  mtc::Task task;
  if (!build_manipulation_task(object_name, place_candidates, /*approach_move=*/0.08,
                               /*retreat_move=*/0.12, /*attach=*/false, task,
                               /*allow_object_collisions=*/{table_name})) {
    RCLCPP_ERROR(node_->get_logger(),
                 "place_object_mtc: failed to build task for '%s'",
                 object_name.c_str());
    response->success = false;
    return true;
  }

  try {
    const size_t kMaxSolutions = 5;
    moveit::core::MoveItErrorCode rc = task.plan(kMaxSolutions);
    if (!rc || task.solutions().empty()) {
      RCLCPP_ERROR(node_->get_logger(),
                   "place_object_mtc: no feasible placement found for '%s'",
                   object_name.c_str());
      response->success = false;
      return true;
    }
  } catch (const std::exception &e) {
    RCLCPP_ERROR(node_->get_logger(),
                 "place_object_mtc: planning failed for '%s': %s",
                 object_name.c_str(), e.what());
    response->success = false;
    return true;
  }

  RCLCPP_INFO(node_->get_logger(),
              "place_object_mtc: %zu solution(s) found, executing best "
              "(cost=%.4f)",
              task.solutions().size(), task.solutions().front()->cost());

  // ---- Execute the lowest-cost solution -------------------------------------
  std::string err;
  if (!execute_manipulation_solution(*task.solutions().front(), held_obj,
                                     /*is_pick=*/false, err)) {
    RCLCPP_ERROR(node_->get_logger(),
                 "place_object_mtc: execution failed for '%s': %s",
                 object_name.c_str(), err.c_str());
    response->success = false;
    return true;
  }

  // The object has been released; the arm is no longer holding anything.
  holding_object_ = false;
  held_object_ = moveit_msgs::msg::CollisionObject();

  RCLCPP_INFO(node_->get_logger(),
              "place_object_mtc: successfully placed '%s' on '%s'",
              object_name.c_str(), table_name.c_str());
  response->success = true;
  return true;
}

/**
 * @brief Place a currently held object at the requested pose.
 *
 * The place pipeline mirrors the pickup pipeline:
 *
 * Phase A – Joint-space plan to a pre-place pose retreated from `place_pose`
 *           along the negative approach axis by `preplace_dist`.
 * Phase B – Cartesian descent from pre-place to `place_pose`.
 * Phase C – Open gripper, detach the attached collision object from the scene,
 *           then Cartesian retreat back to the pre-place pose.
 *
 * All three phases are planned ahead of time with chained robot states, scored
 * by total joint displacement, and the safest feasible plan is executed.
 *
 * The `place_pose` orientation determines the approach direction — whatever
 * orientation the arm held the object at, passing the same orientation ensures
 * the descent and retreat are collinear.
 */
/**
 * @brief Sample free placement spots on a named table collision object.
 *
 * Builds a uniform grid over the table's top surface (in the table's local
 * frame), transforms each cell to world frame, then discards any cell whose
 * 2-D axis-aligned footprint overlaps another collision object in the scene
 * (including a clearance margin).  The returned poses are ready to use as
 * place targets: they are oriented top-down with Z set so the held object's
 * bottom lands on the table surface.
 *
 * Candidates are sorted by distance from the current end-effector position
 * so that the arm prefers nearby spots, reducing travel and planning time.
 * At most MAX_PLACE_CANDIDATES poses are returned.
 */
std::vector<geometry_msgs::msg::Pose>
CrackleManipulation::find_place_poses_on_table(
    const std::string &object_name, const std::string &table_name) {
  constexpr double approach_dist_place = 0.25; // EEF offset above object centre
  constexpr double clearance = 0.04;           // extra safety margin (m)
  constexpr double grid_step_fine = 0.05;      // dense spacing near the pickup (m)
  constexpr double grid_step_coarse = 0.15;    // sparse spacing across workarea (m)
  constexpr double near_radius = 0.20;         // dense-sampling radius around pickup (m)
  constexpr double workarea_half = 0.5;        // half of the 1 m x 1 m workarea (m)
  constexpr double edge_margin = 0.05;         // keep away from table edge (m)
  // Lift the placed object a few mm above the table so its (often slightly
  // oversized perception) mesh doesn't intersect the table top at the place
  // pose — that penetration is what makes the IK stage reject every candidate
  // with "eef in collision: <obj> - table", independent of the XY spot.
  constexpr double place_clearance = 0.008;
  constexpr size_t MAX_PLACE_CANDIDATES = 40;

  std::vector<geometry_msgs::msg::Pose> result;

  // ---- Get the table --------------------------------------------------------
  auto all_objects = planning_scene_->getObjects();
  if (all_objects.find(table_name) == all_objects.end()) {
    RCLCPP_ERROR(node_->get_logger(),
                 "find_place_poses_on_table: '%s' not found in planning scene",
                 table_name.c_str());
    return result;
  }
  const auto &table = all_objects[table_name];

  if (table.primitives.empty() ||
      table.primitives[0].type != shape_msgs::msg::SolidPrimitive::BOX) {
    RCLCPP_ERROR(node_->get_logger(),
                 "find_place_poses_on_table: table '%s' must have a BOX primitive",
                 table_name.c_str());
    return result;
  }
  using P = shape_msgs::msg::SolidPrimitive;
  const auto &tp = table.primitives[0];
  const double table_hx = tp.dimensions[P::BOX_X] / 2.0;
  const double table_hy = tp.dimensions[P::BOX_Y] / 2.0;
  const double table_hz = tp.dimensions[P::BOX_Z] / 2.0;

  const Eigen::Vector3d table_pos(table.pose.position.x,
                                  table.pose.position.y,
                                  table.pose.position.z);
  const Eigen::Quaterniond table_q(table.pose.orientation.w,
                                   table.pose.orientation.x,
                                   table.pose.orientation.y,
                                   table.pose.orientation.z);

  // ---- Get the dimensions of the object being placed ------------------------
  // It is currently an attached collision object.
  double obj_height = 0.10;
  double obj_hx = 0.05, obj_hy = 0.05;
  auto attached = planning_scene_->getAttachedObjects({object_name});
  if (attached.find(object_name) != attached.end()) {
    const auto &prims = attached[object_name].object.primitives;
    if (!prims.empty()) {
      obj_height = primitive_height(prims[0]);
      auto fp = primitive_footprint_xy(prims[0]);
      obj_hx = fp.first;
      obj_hy = fp.second;
    }
  } else {
    RCLCPP_WARN(node_->get_logger(),
                "find_place_poses_on_table: '%s' not found as attached object, "
                "using default dimensions",
                object_name.c_str());
  }

  // ---- Build occupied-region list from other scene objects ------------------
  struct OccupiedRect { double cx, cy, hx, hy; };
  std::vector<OccupiedRect> occupied;
  for (const auto &kv : all_objects) {
    if (kv.first == table_name) continue;
    double ohx = 0.05, ohy = 0.05;
    if (!kv.second.primitives.empty()) {
      auto fp = primitive_footprint_xy(kv.second.primitives[0]);
      ohx = fp.first;
      ohy = fp.second;
    }
    occupied.push_back({kv.second.pose.position.x,
                        kv.second.pose.position.y, ohx, ohy});
  }

  // ---- Place height: put the object's ACTUAL lowest point just above the table.
  // table top in world frame is table_pos + table_q * (0,0, table_hz)
  const double table_top_z =
      (table_pos + table_q * Eigen::Vector3d(0, 0, table_hz)).z();

  // The held object descends onto the table at the place pose. Sizing the descent
  // from a nominal half-height leaves the object's REAL geometry — especially the
  // slightly-oversized perception MESH, or a tilted grasp — intersecting the
  // table, which makes MTC's IK reject every candidate ("eef in collision:
  // <obj> - table"). So measure, at the current held state, the true vertical gap
  // from the end-effector down to the object's LOWEST point (over all of its
  // meshes AND primitives), and place so that lowest point sits `place_clearance`
  // above the table. Falls back to the nominal half-height if unavailable.
  double eef_to_lowest = approach_dist_place + obj_height / 2.0;
  {
    moveit::core::RobotStatePtr state = move_group_->getCurrentState(1.0);
    if (state && attached.find(object_name) != attached.end()) {
      auto to_iso = [](const geometry_msgs::msg::Pose &p) {
        Eigen::Quaterniond q(p.orientation.w, p.orientation.x, p.orientation.y,
                             p.orientation.z);
        if (q.norm() < 1e-6) q = Eigen::Quaterniond::Identity();
        Eigen::Isometry3d T = Eigen::Isometry3d::Identity();
        T.linear() = q.normalized().toRotationMatrix();
        T.translation() =
            Eigen::Vector3d(p.position.x, p.position.y, p.position.z);
        return T;
      };

      const auto &aco = attached[object_name];
      const std::string &attach_link = aco.link_name;
      // world <- object frame (object.pose is relative to attach_link).
      const Eigen::Isometry3d T_obj =
          state->getGlobalLinkTransform(attach_link) * to_iso(aco.object.pose);

      double lowest_z = std::numeric_limits<double>::max();

      // Mesh vertices — the true perception geometry, including any bleed.
      for (size_t i = 0; i < aco.object.meshes.size(); ++i) {
        const Eigen::Isometry3d T_m =
            T_obj * (i < aco.object.mesh_poses.size()
                         ? to_iso(aco.object.mesh_poses[i])
                         : Eigen::Isometry3d::Identity());
        for (const auto &v : aco.object.meshes[i].vertices)
          lowest_z =
              std::min(lowest_z, (T_m * Eigen::Vector3d(v.x, v.y, v.z)).z());
      }

      // Primitive corners (box/cylinder/sphere via footprint + height).
      for (size_t i = 0; i < aco.object.primitives.size(); ++i) {
        const Eigen::Isometry3d T_p =
            T_obj * (i < aco.object.primitive_poses.size()
                         ? to_iso(aco.object.primitive_poses[i])
                         : Eigen::Isometry3d::Identity());
        auto fp = primitive_footprint_xy(aco.object.primitives[i]);
        const double hx = fp.first, hy = fp.second;
        const double hz = primitive_height(aco.object.primitives[i]) / 2.0;
        for (double sx : {-1.0, 1.0})
          for (double sy : {-1.0, 1.0})
            for (double sz : {-1.0, 1.0})
              lowest_z = std::min(
                  lowest_z,
                  (T_p * Eigen::Vector3d(sx * hx, sy * hy, sz * hz)).z());
      }

      if (lowest_z < std::numeric_limits<double>::max()) {
        const double eef_z =
            state->getGlobalLinkTransform(move_group_->getEndEffectorLink())
                .translation()
                .z();
        eef_to_lowest = eef_z - lowest_z;
        RCLCPP_INFO(node_->get_logger(),
                    "place: measured EEF-above-lowest-point = %.3f m",
                    eef_to_lowest);
      }
    }
  }
  // EEF z so the object's lowest point clears the table by place_clearance.
  const double place_z = table_top_z + place_clearance + eef_to_lowest;

  // Top-down orientation for all place poses
  const geometry_msgs::msg::Quaternion top_down_q =
      lookAtQuat(-Eigen::Vector3d::UnitZ(), Eigen::Vector3d::UnitY(),
                 kToolForwardInTool);

  // ---- Sample placement spots: dense near the pickup, sparse across the arm's
  //      1 m x 1 m workarea ------------------------------------------------------
  // Sampling is done in WORLD XY. A point is kept only if it lands on the usable
  // table top, inside the workarea box centred on the arm base, and clear of
  // other objects. We lay a fine grid around where the object was picked up (most
  // placements should return near the source) plus a coarse grid across the whole
  // workarea (a few spread-out fallbacks).

  // Arm base = workarea centre; fall back to the world origin.
  Eigen::Vector3d arm_base_world = Eigen::Vector3d::Zero();
  try {
    moveit::core::RobotStatePtr st = move_group_->getCurrentState(1.0);
    if (st) {
      const auto *jmg =
          move_group_->getRobotModel()->getJointModelGroup(move_group_->getName());
      if (jmg && !jmg->getLinkModelNames().empty())
        arm_base_world =
            st->getGlobalLinkTransform(jmg->getLinkModelNames().front()).translation();
    }
  } catch (const std::exception &e) {
    RCLCPP_WARN(node_->get_logger(),
                "find_place_poses_on_table: arm base lookup failed (%s), using origin",
                e.what());
  }

  // Dense-sampling centre = where the object was picked up (fall back to the
  // current EEF, then the arm base).
  double pickup_x = arm_base_world.x(), pickup_y = arm_base_world.y();
  if (holding_object_) {
    pickup_x = held_object_.pose.position.x;
    pickup_y = held_object_.pose.position.y;
  } else {
    geometry_msgs::msg::PoseStamped ee = move_group_->getCurrentPose();
    pickup_x = ee.pose.position.x;
    pickup_y = ee.pose.position.y;
  }

  const double lx_max = table_hx - edge_margin - obj_hx;
  const double ly_max = table_hy - edge_margin - obj_hy;

  std::vector<geometry_msgs::msg::Pose> candidates;
  const double dedup_dist = grid_step_fine * 0.5;

  auto keep = [&](double wx, double wy) {
    // Inside the 1 m x 1 m workarea around the arm base?
    if (std::abs(wx - arm_base_world.x()) > workarea_half ||
        std::abs(wy - arm_base_world.y()) > workarea_half)
      return;
    // Over the usable table top? (world -> table-local)
    const Eigen::Vector3d local =
        table_q.conjugate() * (Eigen::Vector3d(wx, wy, table_top_z) - table_pos);
    if (std::abs(local.x()) > lx_max || std::abs(local.y()) > ly_max) return;
    // Clear of other objects?
    for (const auto &reg : occupied) {
      if (std::abs(wx - reg.cx) < obj_hx + reg.hx + clearance &&
          std::abs(wy - reg.cy) < obj_hy + reg.hy + clearance)
        return;
    }
    // Not a near-duplicate of an already-kept spot?
    for (const auto &c : candidates)
      if (std::hypot(c.position.x - wx, c.position.y - wy) < dedup_dist) return;

    geometry_msgs::msg::Pose pose;
    pose.position.x = wx;
    pose.position.y = wy;
    pose.position.z = place_z;
    pose.orientation = top_down_q;
    candidates.push_back(pose);
  };

  // Fine grid near the pickup location.
  for (double wx = pickup_x - near_radius; wx <= pickup_x + near_radius; wx += grid_step_fine)
    for (double wy = pickup_y - near_radius; wy <= pickup_y + near_radius; wy += grid_step_fine)
      keep(wx, wy);

  // Coarse grid across the whole workarea.
  for (double wx = arm_base_world.x() - workarea_half; wx <= arm_base_world.x() + workarea_half; wx += grid_step_coarse)
    for (double wy = arm_base_world.y() - workarea_half; wy <= arm_base_world.y() + workarea_half; wy += grid_step_coarse)
      keep(wx, wy);

  RCLCPP_INFO(node_->get_logger(),
              "find_place_poses_on_table: %zu candidate spots (dense near "
              "[%.2f, %.2f], sparse over %.1fx%.1f m) on '%s'",
              candidates.size(), pickup_x, pickup_y, workarea_half * 2.0,
              workarea_half * 2.0, table_name.c_str());

  if (candidates.empty())
    return result;

  // ---- Order: nearest-to-pickup first, but keep some far spread -------------
  std::sort(candidates.begin(), candidates.end(),
            [&](const geometry_msgs::msg::Pose &a,
                const geometry_msgs::msg::Pose &b) {
              const double da = std::hypot(a.position.x - pickup_x, a.position.y - pickup_y);
              const double db = std::hypot(b.position.x - pickup_x, b.position.y - pickup_y);
              return da < db;
            });

  if (candidates.size() > MAX_PLACE_CANDIDATES) {
    // Keep the nearest ~75%, then an even spread of the farther ~25%, so we
    // retain spread-out fallbacks instead of only clustering at the pickup spot.
    const size_t n_far = MAX_PLACE_CANDIDATES / 4;
    const size_t n_near = MAX_PLACE_CANDIDATES - n_far;
    std::vector<geometry_msgs::msg::Pose> trimmed(candidates.begin(),
                                                  candidates.begin() + n_near);
    const size_t rem = candidates.size() - n_near;
    for (size_t i = 0; i < n_far; ++i)
      trimmed.push_back(candidates[n_near + (i * rem) / n_far]);
    candidates.swap(trimmed);
  }

  return candidates;
}

bool CrackleManipulation::place_object(
    crackle_interfaces::srv::PlaceObject::Request::SharedPtr request,
    crackle_interfaces::srv::PlaceObject::Response::SharedPtr response) {
  const std::string &table_name =
      request->table_name.empty() ? "table" : request->table_name;

  if (!wait_for_current_state("place_object")) {
    response->success = false;
    return true;
  }

  // Ignore request->object_name — place whatever is currently attached to the
  // EEF. If multiple objects are attached, take the first one.
  std::string object_name;
  {
    auto attached = planning_scene_->getAttachedObjects();
    if (attached.empty()) {
      RCLCPP_ERROR(node_->get_logger(),
                   "place_object: nothing is attached to the gripper");
      response->success = false;
      return true;
    }
    object_name = attached.begin()->first;
    if (attached.size() > 1) {
      RCLCPP_WARN(node_->get_logger(),
                  "place_object: %zu attached objects, placing '%s'",
                  attached.size(), object_name.c_str());
    }
  }

  RCLCPP_INFO(node_->get_logger(), "place_object: placing '%s' on '%s'",
              object_name.c_str(), table_name.c_str());

  // ---- Discover free spots on the table -------------------------------------
  std::vector<geometry_msgs::msg::Pose> place_candidates =
      find_place_poses_on_table(object_name, table_name);

  if (place_candidates.empty()) {
    RCLCPP_ERROR(node_->get_logger(),
                 "place_object: no free spots found on '%s'",
                 table_name.c_str());
    response->success = false;
    return true;
  }

  // ---- Plan all three phases for each candidate; collect feasible ones ------
  constexpr double preplace_dist = 0.15; // retreat from place along approach axis
  constexpr double retreat_dist = 0.20;  // post-release retreat distance

  struct Candidate {
    geometry_msgs::msg::Pose place;
    PickPhases phases;
  };
  std::vector<Candidate> valid;
  valid.reserve(place_candidates.size());

  for (size_t i = 0; i < place_candidates.size(); ++i) {
    const auto &place_pose = place_candidates[i];
    // Preplace and retreat are both retreats along the negative approach axis
    geometry_msgs::msg::Pose preplace = compute_pregrasp(place_pose, preplace_dist);
    geometry_msgs::msg::Pose retreat = compute_pregrasp(place_pose, retreat_dist);

    RCLCPP_DEBUG(node_->get_logger(),
                 "place_object: planning candidate %zu/%zu ...", i + 1,
                 place_candidates.size());

    PickPhases phases;
    if (plan_pickup_phases(preplace, place_pose, retreat, phases)) {
      RCLCPP_INFO(node_->get_logger(),
                  "place_object: candidate %zu feasible, score=%.4f", i + 1,
                  phases.score);
      valid.push_back({place_pose, std::move(phases)});
    }
  }

  if (valid.empty()) {
    RCLCPP_ERROR(node_->get_logger(),
                 "place_object: no feasible plan found for placing '%s'",
                 object_name.c_str());
    response->success = false;
    return true;
  }

  // ---- Execute the safest candidate (lowest joint displacement) --------------
  std::sort(valid.begin(), valid.end(), [](const Candidate &a, const Candidate &b) {
    return a.phases.score < b.phases.score;
  });
  RCLCPP_INFO(node_->get_logger(),
              "place_object: %zu feasible candidates, executing safest "
              "(score=%.4f)",
              valid.size(), valid.front().phases.score);

  const Candidate &best = valid.front();

  // Phase A: joint-space move to pre-place
  plan_ = best.phases.pregrasp_plan;
  is_trajectory_ = false;
  if (!execute_plan(true)) {
    RCLCPP_ERROR(node_->get_logger(), "place_object: pre-place execution failed");
    response->success = false;
    return true;
  }

  // Phase B: Cartesian descent to place pose
  trajectory_ = best.phases.approach_traj;
  is_trajectory_ = true;
  if (!execute_plan(true)) {
    RCLCPP_ERROR(node_->get_logger(), "place_object: descent execution failed");
    response->success = false;
    return true;
  }

  // Open gripper and detach the object from the scene
  set_gripper_blocking(false);

  moveit_msgs::msg::AttachedCollisionObject detach;
  detach.object.id = object_name;
  detach.object.operation = moveit_msgs::msg::CollisionObject::REMOVE;
  planning_scene_->applyAttachedCollisionObject(detach);

  // Phase C: Cartesian retreat
  trajectory_ = best.phases.lift_traj;
  is_trajectory_ = true;
  if (!execute_plan(true)) {
    RCLCPP_ERROR(node_->get_logger(), "place_object: retreat execution failed");
    response->success = false;
    return true;
  }

  RCLCPP_INFO(node_->get_logger(),
              "place_object: successfully placed '%s' on '%s'",
              object_name.c_str(), table_name.c_str());
  response->success = true;
  return true;
}

void CrackleManipulation::initialize(const std::string &group_name) {

  move_group_ =
      std::make_shared<moveit::planning_interface::MoveGroupInterface>(
          node_, group_name);
  planning_scene_ =
      std::make_shared<moveit::planning_interface::PlanningSceneInterface>();
  RCLCPP_INFO(logger_, "Planning frame: %s",
              move_group_->getPlanningFrame().c_str());
  RCLCPP_INFO(logger_, "End effector link: %s",
              move_group_->getEndEffectorLink().c_str());
  RCLCPP_INFO(logger_, "Available Planning Groups:");
  std::copy(move_group_->getJointModelGroupNames().begin(),
            move_group_->getJointModelGroupNames().end(),
            std::ostream_iterator<std::string>(std::cout, ", "));
  move_group_->setMaxVelocityScalingFactor(max_velocity_scaling_factor);
  move_group_->setMaxAccelerationScalingFactor(max_acceleration_scaling_factor);
  move_group_->setPlanningTime(20.0);
  move_group_->setNumPlanningAttempts(20);
  move_group_->allowReplanning(true);
  move_group_->setGoalPositionTolerance(0.01);
  move_group_->setGoalOrientationTolerance(0.05);
  move_group_->setGoalJointTolerance(0.01);
  is_trajectory_ = false;
}

bool CrackleManipulation::wait_for_current_state(const std::string &caller,
                                                 double timeout_sec) {
  auto state = move_group_->getCurrentState(timeout_sec);
  if (!state) {
    if (have_joint_state_) {
      const auto now = node_->now();
      const double age = (now - last_joint_state_stamp_).seconds();
      RCLCPP_ERROR(node_->get_logger(),
                   "%s: last /joint_states age=%.3fs (now=%.3f, stamp=%.3f)",
                   caller.c_str(), age, now.seconds(),
                   last_joint_state_stamp_.seconds());
    }
    RCLCPP_ERROR(node_->get_logger(),
                 "%s: failed to fetch current robot state (timeout=%.1fs). "
                 "Check /joint_states and clock sync.",
                 caller.c_str(), timeout_sec);
    return false;
  }
  return true;
}

/**
 * @brief Plans and executes a motion to the specified target pose using MoveIt.
 *
 * This function sets the given target pose for the MoveGroupInterface, attempts
 * to plan a trajectory, and if successful, executes the planned trajectory. It
 * logs the outcome of the planning process.
 *
 * @param target_pose The desired end-effector pose to move to.
 * @param move_group_interface Reference to the MoveGroupInterface used for
 * planning and execution.
 * @param logger Logger for outputting informational and error messages.
 * @return true if planning (and execution) succeeded, false otherwise.
 */
bool CrackleManipulation::plan_to_pose(
    const geometry_msgs::msg::Pose &target_pose) {
  if (!wait_for_current_state("plan_to_pose"))
    return false;
  move_group_->setStartStateToCurrentState();

  // Prefer the joint configuration CLOSEST to where the arm is now. Handing OMPL
  // a bare pose goal (setPoseTarget) lets it satisfy the goal with ANY inverse-
  // kinematics solution, so it often picks a far branch (elbow/wrist flip) and
  // the joint-space path is long and roundabout — which is what made the moves to
  // the scan poses swing around. Instead, solve IK seeded from the current state
  // (the solver returns the solution nearest the seed) and plan to that single
  // joint goal, so the motion is the least-joint-travel way there. With the
  // configured num_planning_attempts, OMPL then keeps the shortest attempt. Falls
  // back to the plain pose goal if IK can't find a nearby solution.
  bool used_joint_goal = false;
  move_group_->clearPoseTargets();
  moveit::core::RobotStatePtr current = move_group_->getCurrentState(1.0);
  const auto *jmg =
      move_group_->getRobotModel()->getJointModelGroup(move_group_->getName());
  if (current && jmg) {
    moveit::core::RobotState goal_state(*current);
    const std::string ik_frame = move_group_->getEndEffectorLink();
    if (goal_state.setFromIK(jmg, target_pose, ik_frame, 0.1)) {
      move_group_->setJointValueTarget(goal_state);
      used_joint_goal = true;
    } else {
      RCLCPP_WARN(node_->get_logger(),
                  "plan_to_pose: IK for nearest joint goal failed; "
                  "falling back to pose target");
    }
  }
  if (!used_joint_goal) {
    if (!move_group_->setPoseTarget(target_pose))
      RCLCPP_WARN(node_->get_logger(), "setPoseTarget: out of bounds");
  }

  bool success =
      (move_group_->plan(plan_) == moveit::core::MoveItErrorCode::SUCCESS);
  if (!success)
    RCLCPP_ERROR(node_->get_logger(), "planPoseTarget: plan failed");
  is_trajectory_ = false;
  return success;
}

/**
 * @brief Plan a Cartesian path through a sequence of end-effector poses.
 *
 * This function attempts to compute a Cartesian trajectory that follows the
 * ordered poses in pose_target_vector using the MoveIt computeCartesianPath
 * API. It performs basic validation, invokes the planner with the
 * class-configured step size and jump threshold, stores the resulting
 * trajectory, and records whether a valid trajectory was produced.
 *
 * Behavior:
 * - If pose_target_vector is empty, a warning is logged and the function
 * returns false.
 * - Calls move_group_->computeCartesianPath(pose_target_vector, eef_step,
 * jump_threshold, trajectory_).
 * - If the returned fraction of the path planned is less than 0.99 (i.e. <
 * 99%), an error is logged and the function returns false.
 * - On successful planning (fraction >= 0.99), sets is_trajectory_ to true and
 * returns true.
 *
 * Side effects / class members used:
 * - Reads/writes: trajectory_ (stores computed trajectory), is_trajectory_
 * (flag set on success).
 * - Reads: move_group_, eef_step, jump_threshold, node_ (for logging).
 * - Logs warnings/errors to node_->get_logger().
 *
 * Parameters:
 * @param pose_target_vector Vector of target end-effector poses (in order) to
 * follow.
 *
 * Returns:
 * @return true if a valid Cartesian trajectory covering >= 99% of the requested
 * path was computed and stored in trajectory_; false otherwise.
 */
bool CrackleManipulation::plan_cartesian_path(
    const std::vector<geometry_msgs::msg::Pose> &pose_target_vector) {
  if (pose_target_vector.empty()) {
    RCLCPP_WARN(node_->get_logger(), "planCartesianPath: empty target vector");
    return false;
  }

  if (!wait_for_current_state("plan_cartesian_path"))
    return false;
  move_group_->setStartStateToCurrentState();
  moveit_msgs::msg::RobotTrajectory cart_traj_msg;
  double fraction = move_group_->computeCartesianPath(
      pose_target_vector, eef_step, jump_threshold, cart_traj_msg);

  if (fraction < 0.99) {
    RCLCPP_ERROR(node_->get_logger(),
                 "planCartesianPath: only %.2f%% of path planned",
                 fraction * 100.0);
    return false;
  }

  // 🚫 No getCurrentState() here – avoids the timeout problem
  robot_trajectory::RobotTrajectory cart_traj(move_group_->getRobotModel(),
                                              move_group_->getName());

  // Make a default state just to satisfy the API
  moveit::core::RobotState dummy_state(move_group_->getRobotModel());
  dummy_state.setToDefaultValues();
  cart_traj.setRobotTrajectoryMsg(dummy_state, cart_traj_msg);

  trajectory_processing::IterativeParabolicTimeParameterization iptp;
  bool timing_ok = iptp.computeTimeStamps(
      cart_traj, max_velocity_scaling_factor, max_acceleration_scaling_factor);

  if (!timing_ok) {
    RCLCPP_WARN(node_->get_logger(),
                "planCartesianPath: failed to compute time stamps");
    return false;
  }

  cart_traj.getRobotTrajectoryMsg(trajectory_);
  is_trajectory_ = true;
  return true;
}

/**
 * @brief Perform a dance routine by executing a predefined sequence of poses.
 */
bool CrackleManipulation::dance_dance(
    std_srvs::srv::Trigger::Request::SharedPtr request,
    std_srvs::srv::Trigger::Response::SharedPtr response) {
  // Placeholder for dance logic
  RCLCPP_INFO(rclcpp::get_logger("crackle_moveit_manipulation_node"),
              "Crackle is dancing!");

  std::vector<geometry_msgs::msg::Pose> dance_poses;
  // Add dance poses here

  geometry_msgs::msg::Pose pose2;
  pose2.position.x = 0.2945;
  pose2.position.y = -0.1386;
  pose2.position.z = 0.3941;
  // [ -0.8141054, -0.4435977, 0.1902415, 0.3228957 ]
  pose2.orientation.x = -0.0;
  pose2.orientation.y = -0.0;
  pose2.orientation.z = 0.0;
  pose2.orientation.w = 1.0;
  // RCLCPP_INFO(node_->get_logger(), "Moving to dance pose 2");
  this->plan_to_pose(pose2);
  this->execute_plan(true);

  geometry_msgs::msg::Pose pose3;
  pose3.position.x = 0.1358;
  pose3.position.y = -0.2401;
  pose3.position.z = 0.6326;
  // [ -0.4618424, 0.1499424, 0.8700734, 0.0848008 ]
  pose3.orientation.x = -0.0;
  ;
  pose3.orientation.y = -0.0;
  pose3.orientation.z = 0.0;
  pose3.orientation.w = 1.0;

  // RCLCPP_INFO(node_->get_logger(), "Moving to dance pose 3");
  // this->plan_to_pose(pose3);
  // this->execute_plan(true);

  geometry_msgs::msg::Pose pose4;
  pose4.position.x = -0.0332;
  pose4.position.y = -0.3228;
  pose4.position.z = 0.3372;
  // [ 0.6733465, 0.6007782, -0.348407, -0.2535403 ]
  pose4.orientation.x = -0.0;
  ;
  pose4.orientation.y = -0.0;
  pose4.orientation.z = 0.0;
  pose4.orientation.w = 1.0;

  for (int i = 0; i < 2; i++) {
    dance_poses.push_back(pose2);
    dance_poses.push_back(pose3);
    dance_poses.push_back(pose4);
    dance_poses.push_back(pose3);
  }
  bool success = plan_cartesian_path(dance_poses);
  if (!success) {
    RCLCPP_ERROR(node_->get_logger(),
                 "dance_dance: planning dance path failed");
    response->success = false;
    return false;
  }
  bool exec_success = this->execute_plan(true);
  if (!exec_success) {
    RCLCPP_ERROR(node_->get_logger(),
                 "dance_dance: execution of dance path failed");
    response->success = false;
    return false;
  }

  response->success = true;
  return true;
}

/**
 * @brief Execute the currently stored plan or trajectory using MoveIt.
 *
 * Attempts to execute either a precomputed motion plan (plan_) or a stored
 * RobotTrajectory (trajectory_) via the configured MoveGroupInterface
 * (move_group_). The flag is_trajectory_ selects which object is executed.
 *
 * If wait is true a synchronous/blocking execution is performed and the call
 * will return after completion. If wait is false an asynchronous/non-blocking
 * execution is started.
 *
 * The outcome is mapped to a boolean: the function returns true only if the
 * MoveIt error code equals moveit::core::MoveItErrorCode::SUCCESS. On failure
 * an error is logged using node_->get_logger().
 *
 * @param wait When true perform a blocking execution; when false start async
 * execution.
 * @return true if execution succeeded (MoveItErrorCode::SUCCESS), false
 * otherwise.
 */
bool CrackleManipulation::execute_plan(bool wait) {
  moveit::core::MoveItErrorCode code;
  if (wait)
    code = is_trajectory_ ? move_group_->execute(trajectory_)
                          : move_group_->execute(plan_);
  else
    code = is_trajectory_ ? move_group_->asyncExecute(trajectory_)
                          : move_group_->asyncExecute(plan_);
  bool success = (code == moveit::core::MoveItErrorCode::SUCCESS);
  if (!success)
    RCLCPP_ERROR(node_->get_logger(),
                 "executePath: execute failed, wait=%d, MoveItErrorCode=%d",
                 wait, code.val);
  return success;
}

geometry_msgs::msg::Quaternion
CrackleManipulation::lookAtQuat(const Eigen::Vector3d &to_dir_world,
                                const Eigen::Vector3d &world_up,
                                const Eigen::Vector3d &tool_forward_in_tool) {
  // Convert quaternion to roll, pitch, yaw
  geometry_msgs::msg::Quaternion q = geometry_msgs::msg::Quaternion();

  Eigen::Vector3d f = to_dir_world.normalized();

  // Keep 'up' not parallel to forward
  Eigen::Vector3d up = world_up;
  if (std::abs(f.dot(up)) > 0.95)
    up = Eigen::Vector3d::UnitY();

  // Build an orthonormal world basis where 'f' is the forward (tool forward)
  // axis
  Eigen::Vector3d r = up.cross(f).normalized(); // right
  Eigen::Vector3d u = f.cross(r).normalized();  // corrected up

  // World rotation matrix whose columns are the world axes that the TOOL frame
  // will map to. We want tool_forward_in_tool to map to 'f'. Build the rotation
  // that sends tool basis -> world basis. Start with tool basis = I
  // (X_tool,Y_tool,Z_tool). Then assign which world axes they should align to.
  // Here we assume tool_forward_in_tool is exactly one of +/-X, +/-Y, +/-Z.
  Eigen::Matrix3d R;
  // Default: tool X->r, Y->u, Z->f (tool +Z forward). Change mapping if your
  // forward is -Z, +X, etc.
  R.col(0) = r;
  R.col(1) = u;
  R.col(2) = f;

  // If your forward is -Z, flip the Z column (and recompute a consistent
  // basis):
  if (tool_forward_in_tool == Eigen::Vector3d(0, 0, -1)) {
    R.col(2) = -f;
    // keep a right-handed frame: choose r the same, recompute u
    R.col(1) =
        R.col(2).cross(R.col(0)).normalized(); // u = z x x
                                               // (R already orthonormal enough;
                                               // this keeps roll stable)
  } else if (tool_forward_in_tool == Eigen::Vector3d::UnitX()) {
    R.col(0) = f;
    R.col(1) = up; // keep up close to world_up
    R.col(2) = R.col(0).cross(R.col(1)).normalized();
  } else if (tool_forward_in_tool == -Eigen::Vector3d::UnitX()) {
    R.col(0) = -f;
    R.col(1) = up;
    R.col(2) = R.col(0).cross(R.col(1)).normalized();
  } else if (tool_forward_in_tool == Eigen::Vector3d::UnitY()) {
    R.col(1) = f;
    R.col(0) = r;
    R.col(2) = R.col(0).cross(R.col(1)).normalized();
  } else if (tool_forward_in_tool == -Eigen::Vector3d::UnitY()) {
    R.col(1) = -f;
    R.col(0) = r;
    R.col(2) = R.col(0).cross(R.col(1)).normalized();
  }

  Eigen::Quaterniond q2(R);

  q.x = q2.x();
  q.y = q2.y();
  q.z = q2.z();
  q.w = q2.w();
  return q;
}

geometry_msgs::msg::Pose
CrackleManipulation::construct_reach_pose(geometry_msgs::msg::Pose object_pose,
                                          Vector3 tool_offset) {
  geometry_msgs::msg::Pose target_pose;
  target_pose.position = object_pose.position;
  target_pose.position.x += tool_offset.x; // offset approach
  target_pose.position.y += tool_offset.y;
  target_pose.position.z += tool_offset.z;

  Eigen::Vector3d obj_position(object_pose.position.x, object_pose.position.y,
                               object_pose.position.z);
  Eigen::Vector3d target_position_eig(
      target_pose.position.x, target_pose.position.y, target_pose.position.z);
  Eigen::Vector3d to_dir_world = (obj_position - target_position_eig)
                                     .normalized(); // from target to object

  target_pose.orientation =
      lookAtQuat(to_dir_world, // to_dir_world (pointing from gripper to object)
                 Eigen::Vector3d::UnitZ(), // world_up
                 kToolForwardInTool);      // tool_forward_in_tool

  RCLCPP_INFO(node_->get_logger(), "reach_for_object: trying pose [%f, %f, %f]",
              target_pose.position.x, target_pose.position.y,
              target_pose.position.z);
  return target_pose;
}

std::vector<std::vector<float>> CrackleManipulation::cuboid_handler(std::vector<geometry_msgs::msg::Point> verts)
{

    // Copy the vector of points
    std::vector<geometry_msgs::msg::Point> verts2 (verts);
    RCLCPP_INFO(node_->get_logger(), "Copies the verts vector");

    for (geometry_msgs::msg::Point vert : verts2){
      RCLCPP_INFO(node_->get_logger(), "Vert: [%f %f %f]", vert.x, vert.y, vert.z);
    }

    // Find vertex point with smallest z.
    std::vector<geometry_msgs::msg::Point>::iterator min_z_point_it = std::min_element(verts2.begin(), verts2.end(),
                                                                        [](const geometry_msgs::msg::Point &p1, const geometry_msgs::msg::Point &p2){
                                                                            return p1.z < p2.z;
                                                                        });

    RCLCPP_INFO(node_->get_logger(), "Found the pointer to the minimum z point");
    geometry_msgs::msg::Point min_z_point = *min_z_point_it;
    RCLCPP_INFO(node_->get_logger(), "Dereferenced to obtain the minimum z point");

    // Remove the min-z-point
    verts2.erase(min_z_point_it);
    RCLCPP_INFO(node_->get_logger(), "Removed the minimum z point");

    // Make the smallest z point the origin.
    for (int i = 0; i < verts2.size(); i++){
        geometry_msgs::msg::Point shifted_point = geometry_msgs::msg::Point();
        shifted_point.x = verts2[i].x - min_z_point.x;
        shifted_point.y = verts2[i].y - min_z_point.y;
        shifted_point.z = verts2[i].z - min_z_point.z;
        verts2.at(i) = shifted_point;
    }

    RCLCPP_INFO(node_->get_logger(), "Made the smallest z point the origin");

    bool foundFlag = false;
    std::vector<geometry_msgs::msg::Point> basisVecs;

    RCLCPP_INFO(node_->get_logger(), "Declared basisVecs");

    // Loop through remaining vertices until we find a orthogonal set of three.
    for (int i = 0; i < verts2.size(); i++){
        for (int j = i+1; j < verts2.size(); j++){
            for (int k = j+1; k < verts2.size(); k++){
                double dot_ij = (verts2[i].x * verts2[j].x) + (verts2[i].y * verts2[j].y) + (verts2[i].z * verts2[j].z);
                double dot_jk = (verts2[j].x * verts2[k].x) + (verts2[j].y * verts2[k].y) + (verts2[j].z * verts2[k].z);
                double dot_ki = (verts2[k].x * verts2[i].x) + (verts2[k].y * verts2[i].y) + (verts2[k].z * verts2[i].z);
                if ((abs(dot_ij) < 1e-5) && (abs(dot_jk) < 1e-5) && (abs(dot_ki) < 1e-5)){
                    basisVecs.push_back(verts2[i]);
                    basisVecs.push_back(verts2[j]);
                    basisVecs.push_back(verts2[k]);
                    foundFlag = true;
                    break;
                }
            }
            if (foundFlag){
                break;
            }
        }
        if (foundFlag){
            break;
        }    
    }

    RCLCPP_INFO(node_->get_logger(), "Populated basisVecs");

    // sort the basis vectors in descending order of magnitude.
    std::stable_sort(basisVecs.begin(), basisVecs.end(), [](const geometry_msgs::msg::Point &p1, const geometry_msgs::msg::Point &p2){
                                                                return sqrt(pow(p1.x, 2) + pow(p1.y, 2) + pow(p1.z, 2)) > sqrt(pow(p2.x, 2) + pow(p2.y, 2) + pow(p2.z, 2));
                                                            });

    RCLCPP_INFO(node_->get_logger(), "Sorted basisVecs in descending order of magnitude");

    // find the most z-axis aligned basis vector. This is the height vector
    std::vector<geometry_msgs::msg::Point>::iterator best_z_basis_vec_it = std::max_element(basisVecs.begin(), basisVecs.end(),
                                                                                [](const geometry_msgs::msg::Point &p1, const geometry_msgs::msg::Point &p2){
                                                                                    return p1.z < p2.z;
                                                                                });
    RCLCPP_INFO(node_->get_logger(), "Found pointer to most z aligned basis vec");
    
    geometry_msgs::msg::Point best_z_basis_vec = *best_z_basis_vec_it;

    RCLCPP_INFO(node_->get_logger(), "Assigned variable to most z aligned basis vec");

    // remove height vector and re-add it at the end.
    basisVecs.erase(best_z_basis_vec_it);
    basisVecs.push_back(best_z_basis_vec);

    RCLCPP_INFO(node_->get_logger(), "Removed height vec and added it back at the end");
    RCLCPP_INFO(node_->get_logger(), "Size of basisVecs: [%ld]", basisVecs.size());
    RCLCPP_INFO(node_->get_logger(), "Height basisVec: [%f %f %f]", basisVecs.back().x, basisVecs.back().y, basisVecs.back().z);
    
    std::vector<std::vector<float>> basisVecsStdVec = {{basisVecs[0].x, basisVecs[0].y, basisVecs[0].z},
                                                       {basisVecs[1].x, basisVecs[1].y, basisVecs[1].z}, 
                                                       {basisVecs[2].x, basisVecs[2].y, basisVecs[2].z}};

    RCLCPP_INFO(node_->get_logger(), "Made the floats std vector");

    return basisVecsStdVec;
   
}

// Publish an arrow per grasp candidate, colored by planning status so we can
// watch the pipeline evolve in rviz (MTC-style observability).
void CrackleManipulation::publish_grasp_markers(
    const std::vector<geometry_msgs::msg::Pose> &grasps,
    const std::vector<int> &statuses) {
  if (!grasp_markers_publisher_) return;

  visualization_msgs::msg::MarkerArray arr;
  arr.markers.reserve(grasps.size());
  const auto now = node_->now();

  for (size_t i = 0; i < grasps.size(); ++i) {
    const int status = (i < statuses.size()) ? statuses[i] : 0;

    // Color scheme by status
    std_msgs::msg::ColorRGBA color;
    color.a = 0.9f;
    switch (status) {
      case 1: color.r = 1.0f; color.g = 1.0f; color.b = 0.0f; break; // planning (yellow)
      case 2: color.r = 0.0f; color.g = 1.0f; color.b = 0.0f; break; // feasible (green)
      case 3: color.r = 1.0f; color.g = 0.0f; color.b = 0.0f; break; // infeasible (red)
      case 4: color.r = 1.0f; color.g = 0.0f; color.b = 1.0f; break; // selected (magenta)
      case 5: color.r = 0.5f; color.g = 0.5f; color.b = 0.5f; color.a = 0.4f; break; // skipped (gray)
      case 0: default: color.r = 0.0f; color.g = 0.8f; color.b = 1.0f; break; // proposed (cyan)
    }

    // Arrow showing the approach direction. Tail at the grasp pose, head
    // pointing along the tool-forward (+Z) axis into the object.
    visualization_msgs::msg::Marker arrow;
    arrow.header.frame_id = "world";
    arrow.header.stamp = now;
    arrow.ns = "grasp_candidates";
    arrow.id = static_cast<int>(i);
    arrow.type = visualization_msgs::msg::Marker::ARROW;
    arrow.action = visualization_msgs::msg::Marker::ADD;
    arrow.color = color;
    // scale.x = shaft diameter, scale.y = head diameter, scale.z = head length
    const double base_scale = (status == 4) ? 1.6 : 1.0;
    arrow.scale.x = 0.008 * base_scale;
    arrow.scale.y = 0.018 * base_scale;
    arrow.scale.z = 0.020 * base_scale;

    Eigen::Quaterniond q(grasps[i].orientation.w, grasps[i].orientation.x,
                         grasps[i].orientation.y, grasps[i].orientation.z);
    Eigen::Vector3d fwd = q * kToolForwardInTool;
    const double len = 0.07 * base_scale;

    geometry_msgs::msg::Point p0, p1;
    p0.x = grasps[i].position.x;
    p0.y = grasps[i].position.y;
    p0.z = grasps[i].position.z;
    p1.x = p0.x + fwd.x() * len;
    p1.y = p0.y + fwd.y() * len;
    p1.z = p0.z + fwd.z() * len;
    arrow.points = {p0, p1};
    arr.markers.push_back(arrow);

    // Small sphere at the grasp position so the sampling grid is legible even
    // when arrows overlap.
    visualization_msgs::msg::Marker dot;
    dot.header.frame_id = "world";
    dot.header.stamp = now;
    dot.ns = "grasp_candidates_points";
    dot.id = static_cast<int>(i);
    dot.type = visualization_msgs::msg::Marker::SPHERE;
    dot.action = visualization_msgs::msg::Marker::ADD;
    dot.color = color;
    dot.scale.x = dot.scale.y = dot.scale.z = 0.012 * base_scale;
    dot.pose.position = p0;
    dot.pose.orientation.w = 1.0;
    arr.markers.push_back(dot);
  }

  grasp_markers_publisher_->publish(arr);
}

void CrackleManipulation::clear_grasp_markers() {
  if (!grasp_markers_publisher_) return;
  visualization_msgs::msg::MarkerArray arr;
  visualization_msgs::msg::Marker del;
  del.header.frame_id = "world";
  del.header.stamp = node_->now();
  del.action = visualization_msgs::msg::Marker::DELETEALL;
  arr.markers.push_back(del);
  grasp_markers_publisher_->publish(arr);
}

// Draw where the gripper actually grips: a sphere at kGripPointOffset along the
// tool-forward (+Z) axis of the end-effector link, plus a line from the EEF
// origin out to it so the offset is legible. Published in the EEF frame, so
// rviz keeps it attached to the tool as the arm moves.
void CrackleManipulation::publish_grip_point_marker() {
  if (!grip_point_publisher_ || !move_group_) return;
  const std::string ik_frame = move_group_->getEndEffectorLink();
  if (ik_frame.empty()) return;

  const rclcpp::Time stamp = node_->now();
  visualization_msgs::msg::MarkerArray arr;

  geometry_msgs::msg::Point eef_origin;   // (0,0,0) in the EEF frame
  geometry_msgs::msg::Point grip_point;   // grip point along +Z
  grip_point.z = kGripPointOffset;

  // Line from the flange out to the grip point.
  visualization_msgs::msg::Marker line;
  line.header.frame_id = ik_frame;
  line.header.stamp = stamp;
  line.ns = "grip_point";
  line.id = 0;
  line.type = visualization_msgs::msg::Marker::LINE_LIST;
  line.action = visualization_msgs::msg::Marker::ADD;
  line.scale.x = 0.004; // line width
  line.color.r = 1.0f;
  line.color.g = 0.6f;
  line.color.a = 1.0f;
  line.pose.orientation.w = 1.0;
  line.points.push_back(eef_origin);
  line.points.push_back(grip_point);
  arr.markers.push_back(line);

  // Sphere at the grip point itself.
  visualization_msgs::msg::Marker dot;
  dot.header.frame_id = ik_frame;
  dot.header.stamp = stamp;
  dot.ns = "grip_point";
  dot.id = 1;
  dot.type = visualization_msgs::msg::Marker::SPHERE;
  dot.action = visualization_msgs::msg::Marker::ADD;
  dot.pose.position = grip_point;
  dot.pose.orientation.w = 1.0;
  dot.scale.x = dot.scale.y = dot.scale.z = 0.02;
  dot.color.r = 1.0f;
  dot.color.g = 0.6f;
  dot.color.a = 1.0f;
  arr.markers.push_back(dot);

  grip_point_publisher_->publish(arr);
}

std::vector<geometry_msgs::msg::Pose>
CrackleManipulation::get_grasp_poses(moveit_msgs::msg::CollisionObject object,
                                     double approach_dist, double tool_width,
                                     std::vector<int> *face_ids_out) {
  // Max object width the jaws can straddle (the gripper's finger opening).
  const double max_grasp_width = tool_width;
  if (face_ids_out) face_ids_out->clear();
  std::vector<geometry_msgs::msg::Pose> grasp_poses;

  // Object centre in the planning frame.
  const Eigen::Vector3d center(object.pose.position.x, object.pose.position.y,
                               object.pose.position.z);

  // Drop a red sphere at the object centre so we can confirm alignment in rviz.
  {
    visualization_msgs::msg::Marker marker;
    marker.header.frame_id = "world";
    marker.header.stamp = node_->now();
    marker.ns = "object_position";
    marker.id = 0;
    marker.type = visualization_msgs::msg::Marker::SPHERE;
    marker.action = visualization_msgs::msg::Marker::ADD;
    marker.pose.position = object.pose.position;
    marker.pose.orientation.w = 1.0;
    marker.scale.x = marker.scale.y = marker.scale.z = 0.05;
    marker.color.r = 1.0f;
    marker.color.a = 1.0f;
    marker_publisher_->publish(marker);
  }

  // Default the primitive to a BOX when the collision object has no type.
  // This lets scene objects published without SolidPrimitive metadata still
  // produce grasp candidates instead of bailing out.
  if (object.primitives.empty()) {
    shape_msgs::msg::SolidPrimitive prim;
    prim.type = shape_msgs::msg::SolidPrimitive::BOX;
    prim.dimensions = {0.05, 0.05, 0.05}; // safe default half-extents * 2
    object.primitives.push_back(prim);
    RCLCPP_INFO(node_->get_logger(),
                "get_grasp_poses: no primitive on '%s', defaulting to BOX",
                object.id.c_str());
  }
  if (object.primitives[0].type == 0) {
    object.primitives[0].type = shape_msgs::msg::SolidPrimitive::BOX;
  }

  const auto &prim = object.primitives[0];

  // Resolve the object's oriented bounding box in the planning frame:
  // (half_extents, R_world whose columns are the box axes, obj_center).
  //
  //   1. Preferred: derive it straight from the MESH corner points. Perception
  //      publishes a minimum-volume oriented bounding-box mesh, so a PCA of its
  //      corners recovers the true box axes exactly. Every grasp we then build
  //      from R_world's columns approaches along a real mesh-face normal — i.e.
  //      perpendicular to the surface — instead of being skewed by a primitive
  //      whose orientation is often left at identity by perception.
  //   2. Fallback (no usable mesh, e.g. spheres): the SolidPrimitive dimensions
  //      combined with object.pose.orientation.
  Eigen::Vector3d half_extents(0.05, 0.05, 0.05);
  Eigen::Matrix3d R_world = Eigen::Matrix3d::Identity();
  Eigen::Vector3d obj_center = center;
  bool used_mesh_basis = false;

  if (prim.type == shape_msgs::msg::SolidPrimitive::BOX &&
      !object.meshes.empty() && object.meshes[0].vertices.size() >= 4) {
    // Corner points into the planning frame: world = T_obj * T_mesh * v.
    Eigen::Quaterniond q_obj(object.pose.orientation.w, object.pose.orientation.x,
                             object.pose.orientation.y, object.pose.orientation.z);
    if (q_obj.norm() < 1e-6) q_obj = Eigen::Quaterniond::Identity();
    Eigen::Isometry3d T_obj = Eigen::Isometry3d::Identity();
    T_obj.linear() = q_obj.normalized().toRotationMatrix();
    T_obj.translation() = Eigen::Vector3d(
        object.pose.position.x, object.pose.position.y, object.pose.position.z);

    Eigen::Isometry3d T_mesh = Eigen::Isometry3d::Identity();
    if (!object.mesh_poses.empty()) {
      Eigen::Quaterniond q_m(object.mesh_poses[0].orientation.w,
                             object.mesh_poses[0].orientation.x,
                             object.mesh_poses[0].orientation.y,
                             object.mesh_poses[0].orientation.z);
      if (q_m.norm() < 1e-6) q_m = Eigen::Quaterniond::Identity();
      T_mesh.linear() = q_m.normalized().toRotationMatrix();
      T_mesh.translation() = Eigen::Vector3d(object.mesh_poses[0].position.x,
                                             object.mesh_poses[0].position.y,
                                             object.mesh_poses[0].position.z);
    }
    const Eigen::Isometry3d T = T_obj * T_mesh;

    std::vector<Eigen::Vector3d> pts;
    pts.reserve(object.meshes[0].vertices.size());
    Eigen::Vector3d mean = Eigen::Vector3d::Zero();
    for (const auto &vtx : object.meshes[0].vertices) {
      const Eigen::Vector3d wv = T * Eigen::Vector3d(vtx.x, vtx.y, vtx.z);
      pts.push_back(wv);
      mean += wv;
    }
    mean /= static_cast<double>(pts.size());

    // PCA: eigenvectors of the corner covariance are the (orthonormal) box axes.
    Eigen::Matrix3d cov = Eigen::Matrix3d::Zero();
    for (const auto &p : pts) {
      const Eigen::Vector3d d = p - mean;
      cov += d * d.transpose();
    }
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> es(cov);
    if (es.info() == Eigen::Success) {
      Eigen::Matrix3d axes = es.eigenvectors();
      if (axes.determinant() < 0) axes.col(0) *= -1.0; // keep right-handed
      Eigen::Vector3d he = Eigen::Vector3d::Zero();
      for (const auto &p : pts) {
        const Eigen::Vector3d d = p - mean;
        for (int a = 0; a < 3; ++a)
          he[a] = std::max(he[a], std::abs(d.dot(axes.col(a))));
      }
      R_world = axes;
      half_extents = he;
      obj_center = mean;
      used_mesh_basis = true;
    }
  }

  if (!used_mesh_basis) {
    using P = shape_msgs::msg::SolidPrimitive;
    switch (prim.type) {
    case P::BOX:
      if (prim.dimensions.size() >= 3) {
        half_extents = Eigen::Vector3d(prim.dimensions[P::BOX_X] / 2.0,
                                       prim.dimensions[P::BOX_Y] / 2.0,
                                       prim.dimensions[P::BOX_Z] / 2.0);
      }
      break;
    case P::CYLINDER:
      if (prim.dimensions.size() >= 2) {
        const double r = prim.dimensions[P::CYLINDER_RADIUS];
        half_extents = Eigen::Vector3d(
            r, r, prim.dimensions[P::CYLINDER_HEIGHT] / 2.0);
      }
      break;
    case P::SPHERE:
      if (!prim.dimensions.empty()) {
        const double r = prim.dimensions[P::SPHERE_RADIUS];
        half_extents = Eigen::Vector3d(r, r, r);
      }
      break;
    default:
      break;
    }
    Eigen::Quaterniond q(object.pose.orientation.w, object.pose.orientation.x,
                         object.pose.orientation.y, object.pose.orientation.z);
    if (q.norm() < 1e-6) q = Eigen::Quaterniond::Identity();
    R_world = q.normalized().toRotationMatrix();
  }

  RCLCPP_INFO(node_->get_logger(),
              "get_grasp_poses: '%s' half_extents=[%.3f %.3f %.3f] (%s basis)",
              object.id.c_str(), half_extents.x(), half_extents.y(),
              half_extents.z(), used_mesh_basis ? "mesh" : "primitive");

  // A face is described by the body axis it is normal to (±X, ±Y, ±Z) and the
  // two in-plane axes we tile across. Axis indices: 0=X, 1=Y, 2=Z.
  struct Face {
    int normal_axis;
    int sign; // +1 or -1
    int u_axis;
    int v_axis;
  };
  const std::vector<Face> faces = {
      {2, +1, 0, 1}, // +Z top
      {2, -1, 0, 1}, // -Z bottom (usually skipped)
      {0, +1, 1, 2}, // +X
      {0, -1, 1, 2}, // -X
      {1, +1, 0, 2}, // +Y
      {1, -1, 0, 2}, // -Y
  };

  // Decide which face is the "bottom" in world frame so we can skip it: the
  // face whose outward world-normal points most strongly along -Z.
  int skip_index = -1;
  double most_down = 1.0; // anything < 0 beats the sentinel
  for (size_t i = 0; i < faces.size(); ++i) {
    const Eigen::Vector3d n_world =
        faces[i].sign * R_world.col(faces[i].normal_axis);
    if (n_world.z() < most_down) {
      most_down = n_world.z();
      skip_index = static_cast<int>(i);
    }
  }

  // Also skip the face whose outward normal points most directly away from the
  // arm's base (i.e. the face on the far side of the object). Grasping that
  // face would require reaching around the object and almost always fails IK.
  Eigen::Vector3d arm_base_world = Eigen::Vector3d::Zero();
  bool have_arm_base = false;
  try {
    moveit::core::RobotStatePtr state = move_group_->getCurrentState(1.0);
    if (state) {
      const auto *jmg = move_group_->getRobotModel()->getJointModelGroup(
          move_group_->getName());
      if (jmg) {
        const auto &link_names = jmg->getLinkModelNames();
        if (!link_names.empty()) {
          const auto &tf = state->getGlobalLinkTransform(link_names.front());
          arm_base_world = tf.translation();
          have_arm_base = true;
          RCLCPP_INFO(node_->get_logger(),
                      "get_grasp_poses: arm base '%s' at [%.3f %.3f %.3f]",
                      link_names.front().c_str(), arm_base_world.x(),
                      arm_base_world.y(), arm_base_world.z());
        }
      }
    }
  } catch (const std::exception &e) {
    RCLCPP_WARN(node_->get_logger(),
                "get_grasp_poses: could not resolve arm base (%s)", e.what());
  }

  int skip_far_index = -1;
  if (have_arm_base) {
    Eigen::Vector3d base_to_obj = obj_center - arm_base_world;
    if (base_to_obj.norm() > 1e-6) {
      base_to_obj.normalize();
      double most_away = 0.0;
      for (size_t i = 0; i < faces.size(); ++i) {
        const Eigen::Vector3d n =
            faces[i].sign * R_world.col(faces[i].normal_axis);
        // + dot product means the face's outward normal is aligned with the
        // base->object direction, i.e. the face sits on the far side of the
        // object relative to the arm.
        const double d = n.dot(base_to_obj);
        if (d > most_away) {
          most_away = d;
          skip_far_index = static_cast<int>(i);
        }
      }
    }
  }

  // Grid density per face. 3x3 on the broad top face, 2x2 on sides keeps the
  // candidate count manageable while still probing each flat surface.
  auto grid_size = [](const Face &f) -> std::pair<int, int> {
    if (f.normal_axis == 2) return {3, 3}; // top / bottom
    return {2, 2};                          // sides
  };

  // How far grasp points may sit from the face centre, as a fraction of the
  // face half-extent. On the side faces we allow most of the face (0.8). On the
  // TOP face (top-down grasps) we pull the candidates in hard toward the middle
  // (kTopCentralInset): a top-down grasp near an edge easily tips the object out
  // of the jaws, whereas a central one is far more likely to actually capture
  // it. Combined with the distance-to-centre sort below, this both eliminates
  // the near-edge top-down points and tries the most-central ones first.
  constexpr double kEdgeInset = 0.8;
  constexpr double kTopCentralInset = 0.4;

  for (size_t fi = 0; fi < faces.size(); ++fi) {
    if (static_cast<int>(fi) == skip_index ||
        static_cast<int>(fi) == skip_far_index)
      continue;
    const Face &f = faces[fi];

    const Eigen::Vector3d n_world = f.sign * R_world.col(f.normal_axis);
    const Eigen::Vector3d u_world = R_world.col(f.u_axis);
    const Eigen::Vector3d v_world = R_world.col(f.v_axis);

    // Top face (outward normal along +Z) uses the tighter central inset so its
    // top-down grasps cluster near the middle of the surface.
    const bool is_top_face = (f.normal_axis == 2 && f.sign > 0);
    const double face_inset = is_top_face ? kTopCentralInset : kEdgeInset;

    const double h_face = half_extents[f.normal_axis];
    const double h_u = half_extents[f.u_axis] * face_inset;
    const double h_v = half_extents[f.v_axis] * face_inset;

    // Finger-closing axis (tool Y): the NARROWER of the face's two in-plane
    // axes, so the fixed ~100 mm jaw opening only has to span the object's
    // smaller cross-section. lookAtQuat maps tool-Y to the in-plane component of
    // up_hint, so passing the closing axis as up_hint rolls the gripper to close
    // along it. Skip the whole face if even that narrower span exceeds the jaw
    // opening — the gripper physically cannot close around it.
    const double w_u = 2.0 * half_extents[f.u_axis];
    const double w_v = 2.0 * half_extents[f.v_axis];
    const bool close_along_u = (w_u <= w_v);
    const double grasp_width = close_along_u ? w_u : w_v;
    if (grasp_width > max_grasp_width) {
      RCLCPP_INFO(node_->get_logger(),
                  "get_grasp_poses: skipping face %zu — narrowest graspable span "
                  "%.0f mm exceeds the %.0f mm jaw opening",
                  fi, grasp_width * 1000.0, max_grasp_width * 1000.0);
      continue;
    }
    const Eigen::Vector3d up_hint =
        (close_along_u ? u_world : v_world).normalized();

    auto [nu, nv] = grid_size(f);

    // Order grid points by distance to the face centre so that the first
    // candidate the pickup loop plans on this face is the one most likely to
    // succeed — this pairs with the per-face early-exit heuristic.
    std::vector<std::pair<double, double>> grid;
    grid.reserve(static_cast<size_t>(nu * nv));
    for (int iu = 0; iu < nu; ++iu) {
      for (int iv = 0; iv < nv; ++iv) {
        const double tu = (nu == 1) ? 0.0
                                    : -1.0 + 2.0 * static_cast<double>(iu) /
                                                 static_cast<double>(nu - 1);
        const double tv = (nv == 1) ? 0.0
                                    : -1.0 + 2.0 * static_cast<double>(iv) /
                                                 static_cast<double>(nv - 1);
        grid.emplace_back(tu, tv);
      }
    }
    std::sort(grid.begin(), grid.end(),
              [](const auto &a, const auto &b) {
                return a.first * a.first + a.second * a.second <
                       b.first * b.first + b.second * b.second;
              });

    for (const auto &[tu, tv] : grid) {
      const Eigen::Vector3d surface_point =
          obj_center + n_world * h_face + u_world * (tu * h_u) +
          v_world * (tv * h_v);
      const Eigen::Vector3d grasp_pt =
          surface_point + n_world * approach_dist;

      geometry_msgs::msg::Pose grasp;
      grasp.position.x = grasp_pt.x();
      grasp.position.y = grasp_pt.y();
      grasp.position.z = grasp_pt.z();
      grasp.orientation = lookAtQuat(-n_world, up_hint, kToolForwardInTool);
      grasp_poses.push_back(grasp);
      if (face_ids_out) face_ids_out->push_back(static_cast<int>(fi));
    }
  }

  const size_t kept_faces = faces.size() -
                            (skip_index >= 0 ? 1u : 0u) -
                            (skip_far_index >= 0 ? 1u : 0u);
  RCLCPP_INFO(node_->get_logger(),
              "get_grasp_poses: generated %zu candidates across %zu faces "
              "(skipped bottom=%d, far=%d)",
              grasp_poses.size(), kept_faces, skip_index, skip_far_index);

  return grasp_poses;
}

// ---------------------------------------------------------------------------
// Pickup pipeline helpers
// ---------------------------------------------------------------------------

double CrackleManipulation::score_trajectory_displacement(
    const moveit_msgs::msg::RobotTrajectory &traj) {
  const auto &jt = traj.joint_trajectory;
  if (jt.points.size() < 2)
    return 0.0;
  double total = 0.0;
  for (size_t j = 0; j < jt.joint_names.size(); ++j)
    for (size_t i = 1; i < jt.points.size(); ++i)
      total += std::abs(jt.points[i].positions[j] - jt.points[i - 1].positions[j]);
  return total;
}

/**
 * @brief Plan the three pickup phases with chained robot states.
 *
 * Phase A – joint-space plan from the current robot state to `pregrasp`.
 * Phase B – Cartesian path from `pregrasp` to `grasp`, seeded from the
 *           final state of phase A.
 * Phase C – Cartesian lift from `grasp` to `lift`, seeded from the final
 *           state of phase B.
 *
 * Chaining the start states means each phase benefits from the IK seed of
 * the previous one, avoiding large configuration jumps between phases.
 * The move_group_ start state is reset to the current robot state on return.
 *
 * @return true only if all three phases plan successfully (Cartesian coverage
 *         >= 99 %) and time-parameterisation succeeds for both Cartesian legs.
 */
bool CrackleManipulation::plan_pickup_phases(
    const geometry_msgs::msg::Pose &pregrasp,
    const geometry_msgs::msg::Pose &grasp,
    const geometry_msgs::msg::Pose &lift,
    PickPhases &out) {

  trajectory_processing::IterativeParabolicTimeParameterization iptp;

  // ---- Phase A: joint-space plan to pregrasp ----------------------------------
  move_group_->setStartStateToCurrentState();
  move_group_->setPoseTarget(pregrasp);
  moveit::planning_interface::MoveGroupInterface::Plan pregrasp_plan;
  if (move_group_->plan(pregrasp_plan) != moveit::core::MoveItErrorCode::SUCCESS) {
    move_group_->setStartStateToCurrentState();
    return false;
  }

  // ---- Phase B: Cartesian approach from pregrasp → grasp ---------------------
  moveit::core::RobotState approach_start =
      state_from_trajectory_end(pregrasp_plan.trajectory_,
                                move_group_->getRobotModel());
  move_group_->setStartState(approach_start);

  moveit_msgs::msg::RobotTrajectory approach_traj;
  double frac = move_group_->computeCartesianPath(
      {grasp}, eef_step, jump_threshold, approach_traj);
  if (frac < 0.99) {
    RCLCPP_DEBUG(node_->get_logger(),
                 "plan_pickup_phases: approach Cartesian coverage %.1f%%",
                 frac * 100.0);
    move_group_->setStartStateToCurrentState();
    return false;
  }
  {
    robot_trajectory::RobotTrajectory rt(move_group_->getRobotModel(),
                                        move_group_->getName());
    rt.setRobotTrajectoryMsg(approach_start, approach_traj);
    if (!iptp.computeTimeStamps(rt, max_velocity_scaling_factor,
                                max_acceleration_scaling_factor)) {
      move_group_->setStartStateToCurrentState();
      return false;
    }
    rt.getRobotTrajectoryMsg(approach_traj);
  }

  // ---- Phase C: Cartesian lift from grasp → lift ------------------------------
  moveit::core::RobotState lift_start =
      state_from_trajectory_end(approach_traj, move_group_->getRobotModel());
  move_group_->setStartState(lift_start);

  moveit_msgs::msg::RobotTrajectory lift_traj;
  frac = move_group_->computeCartesianPath(
      {lift}, eef_step, jump_threshold, lift_traj);
  if (frac < 0.99) {
    RCLCPP_DEBUG(node_->get_logger(),
                 "plan_pickup_phases: lift Cartesian coverage %.1f%%",
                 frac * 100.0);
    move_group_->setStartStateToCurrentState();
    return false;
  }
  {
    robot_trajectory::RobotTrajectory rt(move_group_->getRobotModel(),
                                        move_group_->getName());
    rt.setRobotTrajectoryMsg(lift_start, lift_traj);
    if (!iptp.computeTimeStamps(rt, max_velocity_scaling_factor,
                                max_acceleration_scaling_factor)) {
      move_group_->setStartStateToCurrentState();
      return false;
    }
    rt.getRobotTrajectoryMsg(lift_traj);
  }

  // Reset so subsequent planning calls start from current state
  move_group_->setStartStateToCurrentState();

  out.pregrasp_plan = pregrasp_plan;
  out.approach_traj = approach_traj;
  out.lift_traj = lift_traj;
  out.score = score_trajectory_displacement(pregrasp_plan.trajectory_) +
              score_trajectory_displacement(approach_traj) +
              score_trajectory_displacement(lift_traj);
  return true;
}

bool CrackleManipulation::reach_for_object(const std::string &object_name) {

  // Find object in the planning scene
  std::map<std::string, moveit_msgs::msg::CollisionObject> scene_objects;
  scene_objects = planning_scene_->getObjects();
  if (scene_objects.find(object_name) == scene_objects.end()) {
    RCLCPP_ERROR(node_->get_logger(),
                 "reach_for_object: object '%s' not found in planning scene",
                 object_name.c_str());
    return false;
  } else {
    RCLCPP_INFO(node_->get_logger(),
                "reach_for_object: object '%s' found in planning scene",
                object_name.c_str());
    // print the collision object pose
    const moveit_msgs::msg::CollisionObject &obj = scene_objects[object_name];
    RCLCPP_INFO(node_->get_logger(), "Object ID: %s", obj.id.c_str());
    RCLCPP_INFO(node_->get_logger(), "Object Frame ID: %s",
                obj.header.frame_id.c_str());
    RCLCPP_INFO(node_->get_logger(), "Object Pose: [%f, %f, %f]",
                obj.pose.position.x, obj.pose.position.y, obj.pose.position.z);
    RCLCPP_INFO(node_->get_logger(), "Object Primitive Pose: [%f, %f, %f]",
                obj.primitive_poses[0].position.x,
                obj.primitive_poses[0].position.y,
                obj.primitive_poses[0].position.z);
    RCLCPP_INFO(node_->get_logger(), "Object Orientation: [%f, %f, %f, %f]",
                obj.primitive_poses[0].orientation.x,
                obj.primitive_poses[0].orientation.y,
                obj.primitive_poses[0].orientation.z,
                obj.primitive_poses[0].orientation.w);
    RCLCPP_INFO(node_->get_logger(), "Object Dimensions: [%f, %f, %f]",
                obj.primitives[0].dimensions[0],
                obj.primitives[0].dimensions[1],
                obj.primitives[0].dimensions[2]);

    // Add possible reach poses to this vector from lowest costing to highest
    // costing
    std::vector<geometry_msgs::msg::Pose> target_reach_poses;
    std::vector<bool> reach_success;

    target_reach_poses.push_back(
        construct_reach_pose(obj.pose, {0.05, 0.05, 0.2})); // above object
    target_reach_poses.push_back(
        construct_reach_pose(obj.pose, {-0.05, -0.05, 0.2})); // above object

    bool success = false;
    for (const auto &pose : target_reach_poses) {
      std::vector<geometry_msgs::msg::Pose> pose_vector = {pose};
      geometry_msgs::msg::Pose target_approach_pose = construct_reach_pose(
          obj.pose, {(obj.pose.position.x - pose.position.x) / 2,
                     (obj.pose.position.y - pose.position.y) / 2, 0.2});

      pose_vector.push_back(
          target_approach_pose); // add approach pose to the end of the vector
      // success = plan_to_pose(pose);
      // success = plan_cartesian_path(pose_vector);
      success = plan_to_pose(pose);
      reach_success.push_back(success);
    }

    // Try to execute the first successful plan
    for (size_t i = 0; i < reach_success.size(); ++i) {
      if (reach_success[i]) {
        RCLCPP_INFO(node_->get_logger(),
                    "reach_for_object: planned to object '%s' successfully",
                    object_name.c_str());
        {
          // Build a pose 0.05 m closer to the object along the vector from the
          // approach pose to the object
          const geometry_msgs::msg::Pose approach_pose = target_reach_poses[i];
          Eigen::Vector3d approach_pt(approach_pose.position.x,
                                      approach_pose.position.y,
                                      approach_pose.position.z);
          Eigen::Vector3d object_pt(obj.pose.position.x, obj.pose.position.y,
                                    obj.pose.position.z);

          Eigen::Vector3d dir = object_pt - approach_pt;
          double norm = dir.norm();
          if (norm < 1e-6) {
            RCLCPP_WARN(node_->get_logger(),
                        "reach_for_object: zero distance between approach and "
                        "object, skipping close-in step");
          } else {
            dir.normalize();
            const double closer_step = 0.05; // meters toward the object

            geometry_msgs::msg::Pose closer_pose = approach_pose;
            closer_pose.position.x += dir.x() * closer_step;
            closer_pose.position.y += dir.y() * closer_step;
            closer_pose.position.z += dir.z() * closer_step;
            // keep same orientation as approach_pose

            // Plan a Cartesian path that goes through the approach pose then
            // the closer pose
            std::vector<geometry_msgs::msg::Pose> cartesian_waypoints;
            cartesian_waypoints.push_back(approach_pose);
            cartesian_waypoints.push_back(closer_pose);

            bool cart_ok = plan_cartesian_path(cartesian_waypoints);
            if (cart_ok) {
              RCLCPP_INFO(node_->get_logger(),
                          "reach_for_object: planned cartesian close-in path");
              // execution will follow the existing execute_plan(true) call
              // below, which will run the recently planned trajectory_
              // (is_trajectory_ == true)
            } else {
              RCLCPP_WARN(node_->get_logger(),
                          "reach_for_object: failed to plan cartesian close-in "
                          "path, will execute nominal plan");
              // leave plan_ intact so the subsequent execute_plan(true)
              // executes the earlier plan_
            }
          }
        }

        success = execute_plan(true);
        if (success) {
          RCLCPP_INFO(node_->get_logger(),
                      "reach_for_object: executed to object '%s' successfully",
                      object_name.c_str());
          set_gripper_blocking(false);
          return true;
        }
      }
    }

    // close in on the object

    RCLCPP_ERROR(node_->get_logger(),
                 "reach_for_object: failed to reach for object '%s'",
                 object_name.c_str());
  }

  return false;
}

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);

  auto manipulation = std::make_shared<CrackleManipulation>("lite6");

  rclcpp::executors::MultiThreadedExecutor executor(rclcpp::ExecutorOptions(),
                                                    2);
  executor.add_node(manipulation->node_);
  executor.spin();

  rclcpp::shutdown();
  return 0;
}
