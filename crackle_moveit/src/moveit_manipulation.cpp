
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
#include <thread>
#include <crackle_moveit/moveit_manipulation.hpp>
#include <vector>

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
  node_ = rclcpp::Node::make_shared("crackle_moveit_manipulation_node");

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
          std::bind(&CrackleManipulation::pick_up_object, this,
                    std::placeholders::_1, std::placeholders::_2),
          rmw_qos_profile_services_default, services_cb_group_);
  place_service_ =
      node_->create_service<crackle_interfaces::srv::PlaceObject>(
          "crackle_manipulation/place_object",
          std::bind(&CrackleManipulation::place_object, this,
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
  gripper_command_publisher_ =
      node_->create_publisher<std_msgs::msg::Bool>("/claw/command", 10);
  initialize(group_name);
}

// Assumption: This will be the exact point the arm needs to look at
bool CrackleManipulation::look_at(
    crackle_interfaces::srv::LookAt::Request::SharedPtr request,
    crackle_interfaces::srv::LookAt::Response::SharedPtr response) {
  const double OFFSET = 0.05; // meters
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
  } else {
    response->success = false;
    response->message =
        "Unknown type. Use circle_xy, square_xy, figure8_xy, helix_z";
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
  constexpr double approach_dist = 0.08; // metres – gripper offset from object
  constexpr double pregrasp_dist = 0.20; // metres – retreat along approach axis
  constexpr double lift_dist = 0.10;     // metres – straight-up post-grasp lift
  constexpr double tool_width = 0.10;

  std::vector<geometry_msgs::msg::Pose> grasp_candidates =
      get_grasp_poses(obj, approach_dist, tool_width);
  if (grasp_candidates.empty()) {
    RCLCPP_ERROR(node_->get_logger(),
                 "pick_up_object: no grasp candidates generated for '%s'",
                 object_name.c_str());
    response->success = false;
    return true;
  }

  // Open gripper before planning so the scene state is correct
  gripper_command_publisher_->publish(std_msgs::msg::Bool().set__data(false));

  // ---- Plan all phases for every candidate, keep the feasible ones ------------
  struct Candidate {
    geometry_msgs::msg::Pose grasp;
    PickPhases phases;
  };
  std::vector<Candidate> valid;
  valid.reserve(grasp_candidates.size());

  for (size_t i = 0; i < grasp_candidates.size(); ++i) {
    const geometry_msgs::msg::Pose &grasp = grasp_candidates[i];
    // Pregrasp: retreat from grasp along negative approach axis
    geometry_msgs::msg::Pose pregrasp = compute_pregrasp(grasp, pregrasp_dist);
    // Lift: straight up in world Z from the grasp point
    geometry_msgs::msg::Pose lift = grasp;
    lift.position.z += lift_dist;

    RCLCPP_INFO(node_->get_logger(),
                "pick_up_object: planning candidate %zu/%zu ...", i + 1,
                grasp_candidates.size());

    PickPhases phases;
    if (plan_pickup_phases(pregrasp, grasp, lift, phases)) {
      RCLCPP_INFO(node_->get_logger(),
                  "pick_up_object: candidate %zu feasible, score=%.4f", i + 1,
                  phases.score);
      valid.push_back({grasp, std::move(phases)});
    } else {
      RCLCPP_DEBUG(node_->get_logger(),
                   "pick_up_object: candidate %zu infeasible", i + 1);
    }
  }

  if (valid.empty()) {
    RCLCPP_ERROR(node_->get_logger(),
                 "pick_up_object: no feasible grasp found for '%s'",
                 object_name.c_str());
    response->success = false;
    return true;
  }

  // ---- Select the safest candidate (lowest total joint displacement) ----------
  std::sort(valid.begin(), valid.end(), [](const Candidate &a, const Candidate &b) {
    return a.phases.score < b.phases.score;
  });
  RCLCPP_INFO(node_->get_logger(),
              "pick_up_object: %zu feasible candidates, executing safest "
              "(score=%.4f)",
              valid.size(), valid.front().phases.score);

  const Candidate &best = valid.front();

  // ---- Phase A: move to pregrasp (joint-space) --------------------------------
  plan_ = best.phases.pregrasp_plan;
  is_trajectory_ = false;
  if (!execute_plan(true)) {
    RCLCPP_ERROR(node_->get_logger(),
                 "pick_up_object: pregrasp execution failed");
    response->success = false;
    return true;
  }

  // ---- Phase B: Cartesian approach --------------------------------------------
  trajectory_ = best.phases.approach_traj;
  is_trajectory_ = true;
  if (!execute_plan(true)) {
    RCLCPP_ERROR(node_->get_logger(),
                 "pick_up_object: approach execution failed");
    response->success = false;
    return true;
  }

  // ---- Close gripper and attach object to the scene ---------------------------
  gripper_command_publisher_->publish(std_msgs::msg::Bool().set__data(true));
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  moveit_msgs::msg::AttachedCollisionObject attached;
  attached.link_name = "gripper_base";
  attached.object = obj;
  attached.object.operation = moveit_msgs::msg::CollisionObject::ADD;
  attached.touch_links = {"gripper_base"};
  planning_scene_->applyAttachedCollisionObject(attached);

  // ---- Phase C: Cartesian lift ------------------------------------------------
  trajectory_ = best.phases.lift_traj;
  is_trajectory_ = true;
  if (!execute_plan(true)) {
    RCLCPP_ERROR(node_->get_logger(), "pick_up_object: lift execution failed");
    response->success = false;
    return true;
  }

  RCLCPP_INFO(node_->get_logger(),
              "pick_up_object: successfully picked up '%s'",
              object_name.c_str());
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
  constexpr double approach_dist_place = 0.08; // EEF offset above object centre
  constexpr double clearance = 0.04;           // extra safety margin (m)
  constexpr double grid_step = 0.12;           // grid spacing (m)
  constexpr double edge_margin = 0.05;         // keep away from table edge (m)
  constexpr size_t MAX_PLACE_CANDIDATES = 8;

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

  // ---- EEF height when placing: table top + object half-height + approach ---
  // table top in world frame is table_pos + table_q * (0,0, table_hz)
  const double table_top_z =
      (table_pos + table_q * Eigen::Vector3d(0, 0, table_hz)).z();
  const double place_z = table_top_z + obj_height / 2.0 + approach_dist_place;

  // Top-down orientation for all place poses
  const geometry_msgs::msg::Quaternion top_down_q =
      lookAtQuat(-Eigen::Vector3d::UnitZ(), Eigen::Vector3d::UnitY(),
                 kToolForwardInTool);

  // ---- Sample grid in table-local XY, filter, convert to world poses --------
  const double x0 = -table_hx + edge_margin + obj_hx;
  const double x1 =  table_hx - edge_margin - obj_hx;
  const double y0 = -table_hy + edge_margin + obj_hy;
  const double y1 =  table_hy - edge_margin - obj_hy;

  std::vector<geometry_msgs::msg::Pose> candidates;
  for (double lx = x0; lx <= x1; lx += grid_step) {
    for (double ly = y0; ly <= y1; ly += grid_step) {
      // World XY of this grid cell (Z from table_top + surface)
      Eigen::Vector3d wpt =
          table_pos + table_q * Eigen::Vector3d(lx, ly, table_hz);

      // 2-D AABB collision check against every occupied region
      bool blocked = false;
      for (const auto &reg : occupied) {
        if (std::abs(wpt.x() - reg.cx) < obj_hx + reg.hx + clearance &&
            std::abs(wpt.y() - reg.cy) < obj_hy + reg.hy + clearance) {
          blocked = true;
          break;
        }
      }
      if (blocked) continue;

      geometry_msgs::msg::Pose pose;
      pose.position.x = wpt.x();
      pose.position.y = wpt.y();
      pose.position.z = place_z;
      pose.orientation = top_down_q;
      candidates.push_back(pose);
    }
  }

  RCLCPP_INFO(node_->get_logger(),
              "find_place_poses_on_table: %zu free spots found on '%s'",
              candidates.size(), table_name.c_str());

  if (candidates.empty())
    return result;

  // ---- Sort by distance from current EEF (prefer nearby spots) --------------
  geometry_msgs::msg::PoseStamped eef = move_group_->getCurrentPose();
  std::sort(candidates.begin(), candidates.end(),
            [&eef](const geometry_msgs::msg::Pose &a,
                   const geometry_msgs::msg::Pose &b) {
              double da = std::hypot(a.position.x - eef.pose.position.x,
                                     a.position.y - eef.pose.position.y);
              double db = std::hypot(b.position.x - eef.pose.position.x,
                                     b.position.y - eef.pose.position.y);
              return da < db;
            });

  // Cap to a reasonable number to bound planning time
  if (candidates.size() > MAX_PLACE_CANDIDATES)
    candidates.resize(MAX_PLACE_CANDIDATES);

  return candidates;
}

bool CrackleManipulation::place_object(
    crackle_interfaces::srv::PlaceObject::Request::SharedPtr request,
    crackle_interfaces::srv::PlaceObject::Response::SharedPtr response) {
  const std::string &object_name = request->object_name;
  const std::string &table_name =
      request->table_name.empty() ? "table" : request->table_name;

  RCLCPP_INFO(node_->get_logger(), "place_object: placing '%s' on '%s'",
              object_name.c_str(), table_name.c_str());

  if (!wait_for_current_state("place_object")) {
    response->success = false;
    return true;
  }

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
  constexpr double preplace_dist = 0.20; // retreat from place along approach axis
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
  gripper_command_publisher_->publish(std_msgs::msg::Bool().set__data(false));
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

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
  bool success = move_group_->setPoseTarget(target_pose);
  if (!success)
    RCLCPP_WARN(node_->get_logger(), "setPoseTarget: out of bounds");
  success =
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

std::vector<geometry_msgs::msg::Pose>
CrackleManipulation::get_grasp_poses(moveit_msgs::msg::CollisionObject object,
                                     double approach_dist, double tool_width) {
  (void)tool_width;
  std::vector<geometry_msgs::msg::Pose> grasp_poses;

  // Object centre in the planning frame. object.pose is the object's world
  // pose; primitive_poses[0] is the primitive offset within the object frame
  // (identity for most objects).
  const geometry_msgs::msg::Point &c = object.pose.position;

    // Add a marker here for object.pose.position
    visualization_msgs::msg::Marker marker;
    marker.header.frame_id = "world";
    marker.header.stamp = rclcpp::Time();
    marker.ns = "object_position";
    marker.id = 0;
    marker.type = visualization_msgs::msg::Marker::SPHERE;
    marker.action = visualization_msgs::msg::Marker::ADD;
    marker.pose.position = object.pose.position;
    marker.scale.x = 0.1;
    marker.scale.y = 0.1;
    marker.scale.z = 0.1;
    marker.color.r = 1.0;
    marker.color.g = 0.0;
    marker.color.b = 0.0;
    marker.color.a = 1.0;

    marker_publisher_->publish(marker);

  // Determine the object's half-height to target side grasps at mid-height.
  double half_h = 0.0;
  double half_l = 0.0;
  double half_w = 0.0;
  std::vector<geometry_msgs::msg::Point> basisVecs;

  // for when we add an .stl
  if (object.primitives.empty()) {
    shape_msgs::msg::SolidPrimitive prim = shape_msgs::msg::SolidPrimitive();
    prim.type = shape_msgs::msg::SolidPrimitive::BOX;
    object.primitives = {prim}; // Does this work??
    RCLCPP_INFO(node_->get_logger(), "Added BOX Solid Primitive");
  }

  if (object.primitives[0].type != shape_msgs::msg::SolidPrimitive::BOX){
    RCLCPP_INFO(node_->get_logger(), "BOX Primitive not successfully added!");
  }
  if (object.meshes.empty()){
    RCLCPP_INFO(node_->get_logger(), "Collision Object has no meshes!");
  }

  if (!object.primitives.empty()) {
    const auto &p = object.primitives[0];
    RCLCPP_INFO(node_->get_logger(), "Accessed first Primitive of CollisionObject");
    switch (p.type) {
    case shape_msgs::msg::SolidPrimitive::BOX:
      if(!object.meshes.empty()){
        std::vector<geometry_msgs::msg::Point> verts ( std::begin(object.meshes[0].vertices), std::end(object.meshes[0].vertices) );
        RCLCPP_INFO(node_->get_logger(), "Initialized mesh vertices vector");
        std::vector<std::vector<float>> cuboidReturn = cuboid_handler(verts);
        for (std::vector<float> basisVecStdVec : cuboidReturn){
          RCLCPP_INFO(node_->get_logger(), "Accessed returned vector");
          geometry_msgs::msg::Point basisVec = geometry_msgs::msg::Point();
          basisVec.x = basisVecStdVec[0];
          basisVec.y = basisVecStdVec[1];
          basisVec.z = basisVecStdVec[2];
          basisVecs.push_back(basisVec);
        } 
        RCLCPP_INFO(node_->get_logger(), "Obtained mesh basis vectors");
        double length = sqrt(pow(basisVecs[0].x, 2) + pow(basisVecs[0].y, 2) + pow(basisVecs[0].z, 2));
        double width  = sqrt(pow(basisVecs[1].x, 2) + pow(basisVecs[1].y, 2) + pow(basisVecs[1].z, 2));
        double height = sqrt(pow(basisVecs[2].x, 2) + pow(basisVecs[2].y, 2) + pow(basisVecs[2].z, 2));
        half_l = length / 2.0;
        half_w = width / 2.0;
        half_h = height / 2.0;
        RCLCPP_INFO(node_->get_logger(), "Box Dimensions: [%f %f %f]", length, width, height);
      }
      else{
        half_h = p.dimensions[shape_msgs::msg::SolidPrimitive::BOX_Z] / 2.0;
      }
      break;
    case shape_msgs::msg::SolidPrimitive::CYLINDER:
      half_h =
          p.dimensions[shape_msgs::msg::SolidPrimitive::CYLINDER_HEIGHT] / 2.0;
      break;
    case shape_msgs::msg::SolidPrimitive::SPHERE:
      half_h = p.dimensions[shape_msgs::msg::SolidPrimitive::SPHERE_RADIUS];
      break;
    default:
      break;
    }
  }

  // Each approach is defined by:
  //   offset  – gripper position relative to object centre (world frame)
  //   to_dir  – direction tool +Z points into the object (world frame)
  //   up      – world-up hint passed to lookAtQuat
  struct Approach {
    Eigen::Vector3d offset;
    Eigen::Vector3d to_dir;
    Eigen::Vector3d up;
  };

  const std::vector<Approach> approaches = {
      // 1. Top-down: tool +Z points toward -Z world (downward)
      {{0.0, 0.0, approach_dist}, {0.0, 0.0, -1.0}, {0.0, 1.0, 0.0}},
      // 2. From +X side: tool +Z points toward -X world
      {{approach_dist, 0.0, half_h}, {-1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
      // 3. From -X side: tool +Z points toward +X world
      {{-approach_dist, 0.0, half_h}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
      // 4. From +Y side: tool +Z points toward -Y world
      {{0.0, approach_dist, half_h}, {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}},
      // 5. From -Y side: tool +Z points toward +Y world
      {{0.0, -approach_dist, half_h}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
  };

  std::vector<Approach> oriented_approaches = {};

  // Normalize the basis / "edge" vectors
  std::vector<geometry_msgs::msg::Point> normalizedBasisVecs;
  for (geometry_msgs::msg::Point basisVec : basisVecs){
    geometry_msgs::msg::Point normalized_vec = geometry_msgs::msg::Point();
    double mag = sqrt(pow(basisVec.x, 2) + pow(basisVec.y, 2) + pow(basisVec.z, 2));
    normalized_vec.x = basisVec.x / mag;
    normalized_vec.y = basisVec.y / mag;
    normalized_vec.z = basisVec.z / mag;
    normalizedBasisVecs.push_back(normalized_vec);
  }

  // 1. Top-down: tool +Z points toward -Height of box ("downward")
  // The grasp_pose position is approach_dist away from the face of the box normal to the height (by adding half_h, we get from the box center to the face normal to height)
  Approach top_down = {{normalizedBasisVecs[2].x*(half_h + approach_dist), 
                      normalizedBasisVecs[2].y*(half_h + approach_dist), 
                      normalizedBasisVecs[2].z*(half_h + approach_dist)}, // tool pos
                     {-normalizedBasisVecs[2].x, -normalizedBasisVecs[2].y, -normalizedBasisVecs[2].z}, // tool to_dir
                     {normalizedBasisVecs[0].x, normalizedBasisVecs[0].y, normalizedBasisVecs[0].z}}; // using the length (longest edge) as the "up" direction. Hopefully this means that the gripper will close in along the shorter edge i.e. transverse rather than longitudinal grip 
  oriented_approaches.push_back(top_down);

  // 2. From +Length side: tool +Z points toward -Length. Use +Height as "up" for tool.
  // The grasp_pose position is approach_dist away from the face of the box normal to the length (by adding half_l, we get from the box center to the face normal to length)
  Approach length_on = {{normalizedBasisVecs[0].x*(half_l + approach_dist), 
                       normalizedBasisVecs[0].y*(half_l + approach_dist), 
                       normalizedBasisVecs[0].z*(half_l + approach_dist)}, // tool pos
                       {-normalizedBasisVecs[0].x, -normalizedBasisVecs[0].y, -normalizedBasisVecs[0].z}, // tool to_dir
                       {normalizedBasisVecs[2].x, normalizedBasisVecs[2].y, normalizedBasisVecs[2].z}}; // tool up
  oriented_approaches.push_back(length_on);

  // 3. From -Length side: tool +Z points toward +Length. Use +Height as "up" for tool.
  Approach length_on_opp = {{-normalizedBasisVecs[0].x*(half_l + approach_dist), 
                           -normalizedBasisVecs[0].y*(half_l + approach_dist), 
                           -normalizedBasisVecs[0].z*(half_l + approach_dist)}, // tool pos
                          {normalizedBasisVecs[0].x, normalizedBasisVecs[0].y, normalizedBasisVecs[0].z}, // tool to_dir
                          {normalizedBasisVecs[2].x, normalizedBasisVecs[2].y, normalizedBasisVecs[2].z}}; // tool up
  oriented_approaches.push_back(length_on_opp);

  // 4. From +Width side: tool +Z points toward -Width. Use +Height as "up" for tool.
  // The grasp_pose position is approach_dist away from the face of the box normal to the width (by adding half_w, we get from the box center to the face normal to width)
  Approach width_on = {{normalizedBasisVecs[1].x*(half_w + approach_dist), 
                      normalizedBasisVecs[1].y*(half_w + approach_dist), 
                      normalizedBasisVecs[1].z*(half_w + approach_dist)}, // tool pos
                     {-normalizedBasisVecs[1].x, -normalizedBasisVecs[1].y, -normalizedBasisVecs[1].z}, // tool to_dir
                     {normalizedBasisVecs[2].x, normalizedBasisVecs[2].y, normalizedBasisVecs[2].z}}; // tool up
  oriented_approaches.push_back(width_on);

  // 5. From -Width side: tool +Z points toward +Width. Use +Height as "up" for tool.
  Approach width_on_opp = {{-normalizedBasisVecs[1].x*(half_w + approach_dist), 
                          -normalizedBasisVecs[1].y*(half_w + approach_dist), 
                          -normalizedBasisVecs[1].z*(half_w + approach_dist)}, // tool pos
                         {normalizedBasisVecs[1].x, normalizedBasisVecs[1].y, normalizedBasisVecs[1].z}, // tool to_dir
                         {normalizedBasisVecs[2].x, normalizedBasisVecs[2].y, normalizedBasisVecs[2].z}}; // tool up
  oriented_approaches.push_back(width_on_opp);

  // for (const auto &a : approaches) {
  for (const auto &a : oriented_approaches) {
    geometry_msgs::msg::Pose grasp;
    // remember: const geometry_msgs::msg::Point &c = object.pose.position;
    grasp.position.x = c.x + a.offset.x();
    grasp.position.y = c.y + a.offset.y();
    grasp.position.z = c.z + a.offset.z();
    grasp.orientation = lookAtQuat(a.to_dir, a.up, kToolForwardInTool);
    grasp_poses.push_back(grasp);
  }

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
          std_msgs::msg::Bool msg;
          msg.data = false;
          gripper_command_publisher_->publish(msg);
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
