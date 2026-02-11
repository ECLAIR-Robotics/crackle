
/**
 * @file moveit_manipulation.cpp
 * @brief Implements basic MoveIt! manipulation functionalities for the Crackle robot.
 *
 * This file contains functions to initialize the MoveIt! MoveGroupInterface,
 * plan and execute motions to a target pose, and a main function to run the manipulation node.
 *
 * Functions:
 * - initialize: Sets up the MoveGroupInterface, logs planning frame, end effector link, and available planning groups.
 * - move_to_pose: Plans and executes a motion to a specified target pose using MoveIt!.
 * - main: Initializes ROS 2, creates the node and logger, initializes MoveIt!, defines a target pose,
 *         and commands the robot to move to the target pose.
 *
 * Usage:
 * Compile and run as part of a ROS 2 package. The node will attempt to move the robot's end effector
 * to a hardcoded target pose using MoveIt! planning and execution.
 *
 * @author Tanay Garg
 * @date 09/08/2025
 */
#include <crackle_moveit/moveit_manipulation.hpp>
#include <cmath>

const double jump_threshold = 0.0;
const double eef_step = 0.001;
const double max_velocity_scaling_factor = 0.1;     // [move_group_interface] default is 0.1
const double max_acceleration_scaling_factor = 0.1; // [move_group_interface] default is 0.1

static const Eigen::Vector3d kToolForwardInTool(0.0, 0.0, 1.0); // Z axis

CrackleManipulation::CrackleManipulation(const std::string &group_name)
    : logger_(rclcpp::get_logger("crackle_moveit_manipulation_node"))
{
    node_ = rclcpp::Node::make_shared("crackle_moveit_manipulation_node");

    have_joint_state_ = false;
    last_joint_state_stamp_ = node_->now();
    joint_state_sub_ = node_->create_subscription<sensor_msgs::msg::JointState>(
        "/joint_states",
        10,
        [this](const sensor_msgs::msg::JointState::SharedPtr msg)
        {
            last_joint_state_stamp_ = msg->header.stamp;
            have_joint_state_ = true;
        });

    services_cb_group_ = node_->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);

    pickup_service_ = node_->create_service<crackle_interfaces::srv::PickupObject>(
        "crackle_manipulation/pickup_object",
        std::bind(&CrackleManipulation::pick_up_object, this, std::placeholders::_1, std::placeholders::_2),
        rmw_qos_profile_services_default,
        services_cb_group_);
    look_at_service_ = node_->create_service<crackle_interfaces::srv::LookAt>(
        "crackle_manipulation/look_at",
        std::bind(&CrackleManipulation::look_at, this, std::placeholders::_1, std::placeholders::_2),
        rmw_qos_profile_services_default,
        services_cb_group_);
    plan_pose_service_ = node_->create_service<crackle_interfaces::srv::PlanPose>(
        "crackle_manipulation/plan_pose",
        std::bind(&CrackleManipulation::plan_pose_service, this, std::placeholders::_1, std::placeholders::_2),
        rmw_qos_profile_services_default,
        services_cb_group_);
    execute_plan_service_ = node_->create_service<crackle_interfaces::srv::ExecutePlan>(
        "crackle_manipulation/execute_plan",
        std::bind(&CrackleManipulation::execute_plan_service, this, std::placeholders::_1, std::placeholders::_2),
        rmw_qos_profile_services_default,
        services_cb_group_);
    plan_trajectory_service_ = node_->create_service<crackle_interfaces::srv::PlanTrajectory>(
        "crackle_manipulation/plan_trajectory",
        std::bind(&CrackleManipulation::plan_trajectory_service, this, std::placeholders::_1, std::placeholders::_2),
        rmw_qos_profile_services_default,
        services_cb_group_);
    set_joint_angles_service_ = node_->create_service<crackle_interfaces::srv::SetJointAngles>(
        "crackle_manipulation/set_joint_angles",
        std::bind(&CrackleManipulation::set_joint_angles_service, this, std::placeholders::_1, std::placeholders::_2),
        rmw_qos_profile_services_default,
        services_cb_group_);
    face_direction_relative_service_ = node_->create_service<crackle_interfaces::srv::FaceDirectionRelative>(
        "crackle_manipulation/face_direction_relative",
        std::bind(&CrackleManipulation::face_direction_relative_service, this, std::placeholders::_1, std::placeholders::_2),
        rmw_qos_profile_services_default,
        services_cb_group_);
    move_relative_service_ = node_->create_service<crackle_interfaces::srv::MoveRelative>(
        "crackle_manipulation/move_relative",
        std::bind(&CrackleManipulation::move_relative_service, this, std::placeholders::_1, std::placeholders::_2),
        rmw_qos_profile_services_default,
        services_cb_group_);
    get_end_effector_pose_service_ = node_->create_service<crackle_interfaces::srv::GetEndEffectorPose>(
        "crackle_manipulation/get_end_effector_pose",
        std::bind(&CrackleManipulation::get_end_effector_pose_service, this, std::placeholders::_1, std::placeholders::_2),
        rmw_qos_profile_services_default,
        services_cb_group_);
    demo_trajectory_service_ = node_->create_service<crackle_interfaces::srv::DemoTrajectory>(
        "crackle_manipulation/demo_trajectory",
        std::bind(&CrackleManipulation::demo_trajectory_service, this, std::placeholders::_1, std::placeholders::_2),
        rmw_qos_profile_services_default,
        services_cb_group_);
    dance_service_ = node_->create_service<std_srvs::srv::Trigger>(
        "crackle_manipulation/dance",
        std::bind(&CrackleManipulation::dance_dance, this, std::placeholders::_1, std::placeholders::_2),
        rmw_qos_profile_services_default,
        services_cb_group_);
    gripper_command_publisher_ = node_->create_publisher<std_msgs::msg::Bool>("/claw/command", 10);
    initialize(group_name);
}

// Assumption: This will be the exact point the arm needs to look at
bool CrackleManipulation::look_at(crackle_interfaces::srv::LookAt::Request::SharedPtr request,
                                  crackle_interfaces::srv::LookAt::Response::SharedPtr response)
{
    const double OFFSET = 0.05; // meters
    geometry_msgs::msg::Vector3Stamped look_point = request->look_direction;
    Eigen::Vector3d offsetted_position = Eigen::Vector3d(look_point.vector.x,
                                                         look_point.vector.y,
                                                         look_point.vector.z);
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
    Eigen::Vector3d forward_dir_world = kToolForwardInTool; // Assuming tool frame aligns with world frame
    geometry_msgs::msg::Pose target_pose;
    target_pose.position.x = offsetted_position.x();
    target_pose.position.y = offsetted_position.y();
    target_pose.position.z = offsetted_position.z();
    target_pose.orientation = lookAtQuat(to_dir_world, Eigen::Vector3d::UnitZ(), kToolForwardInTool);

    bool success = plan_to_pose(target_pose);
    if (!success)
    {
        RCLCPP_ERROR(node_->get_logger(), "look_at: planning to look at point failed");
        response->success = false;
        return false;
    }
    moveit::core::MoveItErrorCode exec_status = move_group_->execute(plan_);
    if (exec_status != moveit::core::MoveItErrorCode::SUCCESS)
    {
        RCLCPP_ERROR(node_->get_logger(), "look_at: execution to look at point failed");
        response->success = false;
        return false;
    }
    response->success = true;
    return true;
}

bool CrackleManipulation::plan_pose_service(crackle_interfaces::srv::PlanPose::Request::SharedPtr request,
                                            crackle_interfaces::srv::PlanPose::Response::SharedPtr response)
{
    bool success = plan_to_pose(request->target_pose);
    response->success = success;
    return true;
}

bool CrackleManipulation::execute_plan_service(crackle_interfaces::srv::ExecutePlan::Request::SharedPtr request,
                                               crackle_interfaces::srv::ExecutePlan::Response::SharedPtr response)
{
    bool success = execute_plan(request->wait);
    response->success = success;
    return true;
}

bool CrackleManipulation::plan_trajectory_service(crackle_interfaces::srv::PlanTrajectory::Request::SharedPtr request,
                                                  crackle_interfaces::srv::PlanTrajectory::Response::SharedPtr response)
{
    bool success = plan_cartesian_path(request->waypoints);
    response->success = success;
    return true;
}

bool CrackleManipulation::set_joint_angles_service(crackle_interfaces::srv::SetJointAngles::Request::SharedPtr request,
                                                   crackle_interfaces::srv::SetJointAngles::Response::SharedPtr response)
{
    const auto &joint_angles = request->joint_angles;
    auto joint_model_group = move_group_->getRobotModel()->getJointModelGroup(move_group_->getName());
    if (!joint_model_group)
    {
        RCLCPP_ERROR(node_->get_logger(), "set_joint_angles: joint model group not found");
        response->success = false;
        return true;
    }

    const size_t expected = joint_model_group->getVariableCount();
    if (joint_angles.size() != expected)
    {
        RCLCPP_ERROR(node_->get_logger(), "set_joint_angles: expected %zu joints, got %zu", expected, joint_angles.size());
        response->success = false;
        return true;
    }

    if (!wait_for_current_state("set_joint_angles"))
    {
        response->success = false;
        return true;
    }
    move_group_->setStartStateToCurrentState();
    bool target_ok = move_group_->setJointValueTarget(joint_angles);
    if (!target_ok)
    {
        RCLCPP_ERROR(node_->get_logger(), "set_joint_angles: failed to set joint targets");
        response->success = false;
        return true;
    }

    bool success = (move_group_->plan(plan_) == moveit::core::MoveItErrorCode::SUCCESS);
    if (!success)
        RCLCPP_ERROR(node_->get_logger(), "set_joint_angles: planning failed");
    is_trajectory_ = false;
    response->success = success;
    return true;
}

bool CrackleManipulation::face_direction_relative_service(
    crackle_interfaces::srv::FaceDirectionRelative::Request::SharedPtr request,
    crackle_interfaces::srv::FaceDirectionRelative::Response::SharedPtr response)
{
    geometry_msgs::msg::PoseStamped current_pose = move_group_->getCurrentPose();
    Eigen::Vector3d dir_tool(request->direction_tool.x,
                             request->direction_tool.y,
                             request->direction_tool.z);
    if (dir_tool.norm() < 1e-6)
    {
        RCLCPP_ERROR(node_->get_logger(), "face_direction_relative: zero direction provided");
        response->success = false;
        return true;
    }

    Eigen::Quaterniond q(current_pose.pose.orientation.w,
                         current_pose.pose.orientation.x,
                         current_pose.pose.orientation.y,
                         current_pose.pose.orientation.z);
    Eigen::Vector3d dir_world = (q * dir_tool).normalized();

    geometry_msgs::msg::Pose target_pose = current_pose.pose;
    target_pose.orientation = lookAtQuat(dir_world, Eigen::Vector3d::UnitZ(), kToolForwardInTool);

    bool success = plan_to_pose(target_pose);
    response->success = success;
    return true;
}

bool CrackleManipulation::move_relative_service(crackle_interfaces::srv::MoveRelative::Request::SharedPtr request,
                                                crackle_interfaces::srv::MoveRelative::Response::SharedPtr response)
{
    geometry_msgs::msg::PoseStamped current_pose = move_group_->getCurrentPose();
    Eigen::Vector3d offset_tool(request->offset_tool.x,
                                request->offset_tool.y,
                                request->offset_tool.z);

    Eigen::Quaterniond q(current_pose.pose.orientation.w,
                         current_pose.pose.orientation.x,
                         current_pose.pose.orientation.y,
                         current_pose.pose.orientation.z);
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
    crackle_interfaces::srv::GetEndEffectorPose::Response::SharedPtr response)
{
    (void)request;
    if (!wait_for_current_state("get_end_effector_pose"))
    {
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
    crackle_interfaces::srv::DemoTrajectory::Response::SharedPtr response)
{
    if (!wait_for_current_state("demo_trajectory"))
    {
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

    if (type == "circle_xy")
    {
        for (int i = 0; i <= points; ++i)
        {
            const double t = 2.0 * kPi * static_cast<double>(i) / static_cast<double>(points);
            geometry_msgs::msg::Pose p = base_pose;
            p.position.x += size * std::cos(t);
            p.position.y += size * std::sin(t);
            waypoints.push_back(p);
        }
    }
    else if (type == "square_xy")
    {
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

        for (const auto &c : corners)
        {
            geometry_msgs::msg::Pose p = base_pose;
            p.position = c;
            waypoints.push_back(p);
        }
    }
    else if (type == "figure8_xy")
    {
        for (int i = 0; i <= points; ++i)
        {
            const double t = 2.0 * kPi * static_cast<double>(i) / static_cast<double>(points);
            geometry_msgs::msg::Pose p = base_pose;
            p.position.x += size * std::sin(t);
            p.position.y += size * std::sin(t) * std::cos(t);
            waypoints.push_back(p);
        }
    }
    else if (type == "helix_z")
    {
        for (int i = 0; i <= points; ++i)
        {
            const double t = 2.0 * kPi * static_cast<double>(i) / static_cast<double>(points);
            geometry_msgs::msg::Pose p = base_pose;
            p.position.x += size * std::cos(t);
            p.position.y += size * std::sin(t);
            p.position.z += height * (static_cast<double>(i) / static_cast<double>(points));
            waypoints.push_back(p);
        }
    }
    else
    {
        response->success = false;
        response->message = "Unknown type. Use circle_xy, square_xy, figure8_xy, helix_z";
        return true;
    }

    bool plan_ok = plan_cartesian_path(waypoints);
    if (!plan_ok)
    {
        response->success = false;
        response->message = "Planning failed";
        return true;
    }

    if (request->execute)
    {
        bool exec_ok = execute_plan(true);
        response->success = exec_ok;
        response->message = exec_ok ? "Executed" : "Execution failed";
        return true;
    }

    response->success = true;
    response->message = "Planned";
    return true;
}

bool CrackleManipulation::pick_up_object(crackle_interfaces::srv::PickupObject::Request::SharedPtr request,
                                         crackle_interfaces::srv::PickupObject::Response::SharedPtr response)
{
    std::string object_name = request->object_name;
    RCLCPP_INFO(node_->get_logger(), "pick_up_object: requested to pick up");

    if (!wait_for_current_state("pick_up_object"))
    {
        response->success = false;
        return true;
    }

    // Find object in planning scene
    std::map<std::string, moveit_msgs::msg::CollisionObject> scene_objects = planning_scene_->getObjects({object_name});
    if (scene_objects.find(object_name) == scene_objects.end())
    {
        RCLCPP_ERROR(node_->get_logger(), "pick_up_object: object '%s' not found in planning scene", object_name.c_str());
        response->success = false;
        return true;
    }

    const moveit_msgs::msg::CollisionObject &obj = scene_objects[object_name];
    const double approach_dist = 0.04;
    const double pregrasp_height = 0.12;
    const double lift_dist = 0.08;
    const double tool_width = 0.05;
    std::vector<geometry_msgs::msg::Pose> grasp_poses = get_grasp_poses(obj, approach_dist, tool_width);
    if (grasp_poses.empty())
    {
        RCLCPP_ERROR(node_->get_logger(), "pick_up_object: no valid grasp poses computed for '%s'", object_name.c_str());
        response->success = false;
        return true;
    }

    // Open gripper before approach (best-effort)
    gripper_command_publisher_->publish(std_msgs::msg::Bool().set__data(false));

    const geometry_msgs::msg::Pose obj_pose = obj.pose;
    for (const auto &grasp_pose : grasp_poses)
    {
        geometry_msgs::msg::Pose pregrasp_pose = grasp_pose;
        pregrasp_pose.position.x = obj_pose.position.x;
        pregrasp_pose.position.y = obj_pose.position.y;
        pregrasp_pose.position.z = obj_pose.position.z + pregrasp_height;

        if (!plan_to_pose(pregrasp_pose))
        {
            RCLCPP_WARN(node_->get_logger(), "pick_up_object: plan to pregrasp failed, trying next grasp");
            continue;
        }
        if (!execute_plan(true))
        {
            RCLCPP_WARN(node_->get_logger(), "pick_up_object: execute to pregrasp failed, trying next grasp");
            continue;
        }

        std::vector<geometry_msgs::msg::Pose> approach_waypoints = {pregrasp_pose, grasp_pose};
        if (!plan_cartesian_path(approach_waypoints))
        {
            RCLCPP_WARN(node_->get_logger(), "pick_up_object: cartesian approach failed, trying next grasp");
            continue;
        }
        if (!execute_plan(true))
        {
            RCLCPP_WARN(node_->get_logger(), "pick_up_object: execute approach failed, trying next grasp");
            continue;
        }

        // Close gripper
        gripper_command_publisher_->publish(std_msgs::msg::Bool().set__data(true));

        // Attach object to gripper link after closing
        moveit_msgs::msg::AttachedCollisionObject attached;
        attached.link_name = "gripper_base";
        attached.object = obj;
        attached.object.operation = moveit_msgs::msg::CollisionObject::ADD;
        attached.touch_links = {"gripper_base"};
        planning_scene_->applyAttachedCollisionObject(attached);

        geometry_msgs::msg::Pose lift_pose = grasp_pose;
        lift_pose.position.z += lift_dist;
        std::vector<geometry_msgs::msg::Pose> lift_waypoints = {grasp_pose, lift_pose};
        if (!plan_cartesian_path(lift_waypoints))
        {
            RCLCPP_WARN(node_->get_logger(), "pick_up_object: cartesian lift failed, trying next grasp");
            continue;
        }
        if (!execute_plan(true))
        {
            RCLCPP_WARN(node_->get_logger(), "pick_up_object: execute lift failed, trying next grasp");
            continue;
        }

        response->success = true;
        return true;
    }

    RCLCPP_ERROR(node_->get_logger(), "pick_up_object: failed to pick '%s' with all grasps", object_name.c_str());
    response->success = false;
    return true;
}

void CrackleManipulation::initialize(const std::string &group_name)
{

    move_group_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(node_, group_name);
    planning_scene_ = std::make_shared<moveit::planning_interface::PlanningSceneInterface>();
    RCLCPP_INFO(logger_, "Planning frame: %s", move_group_->getPlanningFrame().c_str());
    RCLCPP_INFO(logger_, "End effector link: %s", move_group_->getEndEffectorLink().c_str());
    RCLCPP_INFO(logger_, "Available Planning Groups:");
    std::copy(move_group_->getJointModelGroupNames().begin(),
              move_group_->getJointModelGroupNames().end(), std::ostream_iterator<std::string>(std::cout, ", "));
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

bool CrackleManipulation::wait_for_current_state(const std::string &caller, double timeout_sec)
{
    auto state = move_group_->getCurrentState(timeout_sec);
    if (!state)
    {
        if (have_joint_state_)
        {
            const auto now = node_->now();
            const double age = (now - last_joint_state_stamp_).seconds();
            RCLCPP_ERROR(node_->get_logger(),
                         "%s: last /joint_states age=%.3fs (now=%.3f, stamp=%.3f)",
                         caller.c_str(),
                         age,
                         now.seconds(),
                         last_joint_state_stamp_.seconds());
        }
        RCLCPP_ERROR(node_->get_logger(),
                     "%s: failed to fetch current robot state (timeout=%.1fs). Check /joint_states and clock sync.",
                     caller.c_str(), timeout_sec);
        return false;
    }
    return true;
}

/**
 * @brief Plans and executes a motion to the specified target pose using MoveIt.
 *
 * This function sets the given target pose for the MoveGroupInterface, attempts to plan a trajectory,
 * and if successful, executes the planned trajectory. It logs the outcome of the planning process.
 *
 * @param target_pose The desired end-effector pose to move to.
 * @param move_group_interface Reference to the MoveGroupInterface used for planning and execution.
 * @param logger Logger for outputting informational and error messages.
 * @return true if planning (and execution) succeeded, false otherwise.
 */
bool CrackleManipulation::plan_to_pose(const geometry_msgs::msg::Pose &target_pose)
{
    if (!wait_for_current_state("plan_to_pose"))
        return false;
    move_group_->setStartStateToCurrentState();
    bool success = move_group_->setPoseTarget(target_pose);
    if (!success)
        RCLCPP_WARN(node_->get_logger(), "setPoseTarget: out of bounds");
    success = (move_group_->plan(plan_) == moveit::core::MoveItErrorCode::SUCCESS);
    if (!success)
        RCLCPP_ERROR(node_->get_logger(), "planPoseTarget: plan failed");
    is_trajectory_ = false;
    return success;
}

/**
 * @brief Plan a Cartesian path through a sequence of end-effector poses.
 *
 * This function attempts to compute a Cartesian trajectory that follows the
 * ordered poses in pose_target_vector using the MoveIt computeCartesianPath API.
 * It performs basic validation, invokes the planner with the class-configured
 * step size and jump threshold, stores the resulting trajectory, and records
 * whether a valid trajectory was produced.
 *
 * Behavior:
 * - If pose_target_vector is empty, a warning is logged and the function returns false.
 * - Calls move_group_->computeCartesianPath(pose_target_vector, eef_step, jump_threshold, trajectory_).
 * - If the returned fraction of the path planned is less than 0.99 (i.e. < 99%),
 *   an error is logged and the function returns false.
 * - On successful planning (fraction >= 0.99), sets is_trajectory_ to true and returns true.
 *
 * Side effects / class members used:
 * - Reads/writes: trajectory_ (stores computed trajectory), is_trajectory_ (flag set on success).
 * - Reads: move_group_, eef_step, jump_threshold, node_ (for logging).
 * - Logs warnings/errors to node_->get_logger().
 *
 * Parameters:
 * @param pose_target_vector Vector of target end-effector poses (in order) to follow.
 *
 * Returns:
 * @return true if a valid Cartesian trajectory covering >= 99% of the requested path
 *         was computed and stored in trajectory_; false otherwise.
 */
bool CrackleManipulation::plan_cartesian_path(
    const std::vector<geometry_msgs::msg::Pose> &pose_target_vector)
{
    if (pose_target_vector.empty())
    {
        RCLCPP_WARN(node_->get_logger(), "planCartesianPath: empty target vector");
        return false;
    }

    if (!wait_for_current_state("plan_cartesian_path"))
        return false;
    move_group_->setStartStateToCurrentState();
    moveit_msgs::msg::RobotTrajectory cart_traj_msg;
    double fraction = move_group_->computeCartesianPath(
        pose_target_vector,
        eef_step,
        jump_threshold,
        cart_traj_msg);

    if (fraction < 0.99)
    {
        RCLCPP_ERROR(node_->get_logger(),
                     "planCartesianPath: only %.2f%% of path planned",
                     fraction * 100.0);
        return false;
    }

    // 🚫 No getCurrentState() here – avoids the timeout problem
    robot_trajectory::RobotTrajectory cart_traj(
        move_group_->getRobotModel(),
        move_group_->getName());

    // Make a default state just to satisfy the API
    moveit::core::RobotState dummy_state(move_group_->getRobotModel());
    dummy_state.setToDefaultValues();
    cart_traj.setRobotTrajectoryMsg(dummy_state, cart_traj_msg);

    trajectory_processing::IterativeParabolicTimeParameterization iptp;
    bool timing_ok = iptp.computeTimeStamps(
        cart_traj,
        max_velocity_scaling_factor,
        max_acceleration_scaling_factor);

    if (!timing_ok)
    {
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
bool CrackleManipulation::dance_dance(std_srvs::srv::Trigger::Request::SharedPtr request,
                                      std_srvs::srv::Trigger::Response::SharedPtr response)
{
    // Placeholder for dance logic
    RCLCPP_INFO(rclcpp::get_logger("crackle_moveit_manipulation_node"), "Crackle is dancing!");

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
    pose3.orientation.x = -0.0;;
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
    pose4.orientation.x = -0.0;;
    pose4.orientation.y = -0.0;
    pose4.orientation.z = 0.0;
    pose4.orientation.w = 1.0;

    for (int i = 0; i < 2; i++)
    {
        dance_poses.push_back(pose2);
        dance_poses.push_back(pose3);
        dance_poses.push_back(pose4);
        dance_poses.push_back(pose3);
    }
    bool success = plan_cartesian_path(dance_poses);
    if (!success)
    {
        RCLCPP_ERROR(node_->get_logger(), "dance_dance: planning dance path failed");
        response->success = false;
        return false;
    }
    bool exec_success = this->execute_plan(true);
    if (!exec_success)
    {
        RCLCPP_ERROR(node_->get_logger(), "dance_dance: execution of dance path failed");
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
 * @param wait When true perform a blocking execution; when false start async execution.
 * @return true if execution succeeded (MoveItErrorCode::SUCCESS), false otherwise.
 */
bool CrackleManipulation::execute_plan(bool wait)
{
    moveit::core::MoveItErrorCode code;
    if (wait)
        code = is_trajectory_ ? move_group_->execute(trajectory_) : move_group_->execute(plan_);
    else
        code = is_trajectory_ ? move_group_->asyncExecute(trajectory_) : move_group_->asyncExecute(plan_);
    bool success = (code == moveit::core::MoveItErrorCode::SUCCESS);
    if (!success)
        RCLCPP_ERROR(node_->get_logger(), "executePath: execute failed, wait=%d, MoveItErrorCode=%d", wait, code.val);
    return success;
}

geometry_msgs::msg::Quaternion CrackleManipulation::lookAtQuat(
    const Eigen::Vector3d &to_dir_world,
    const Eigen::Vector3d &world_up,
    const Eigen::Vector3d &tool_forward_in_tool)
{
    // Convert quaternion to roll, pitch, yaw
    geometry_msgs::msg::Quaternion q = geometry_msgs::msg::Quaternion();

    Eigen::Vector3d f = to_dir_world.normalized();

    // Keep 'up' not parallel to forward
    Eigen::Vector3d up = world_up;
    if (std::abs(f.dot(up)) > 0.95)
        up = Eigen::Vector3d::UnitY();

    // Build an orthonormal world basis where 'f' is the forward (tool forward) axis
    Eigen::Vector3d r = up.cross(f).normalized(); // right
    Eigen::Vector3d u = f.cross(r).normalized();  // corrected up

    // World rotation matrix whose columns are the world axes that the TOOL frame will map to.
    // We want tool_forward_in_tool to map to 'f'. Build the rotation that sends tool basis -> world basis.
    // Start with tool basis = I (X_tool,Y_tool,Z_tool). Then assign which world axes they should align to.
    // Here we assume tool_forward_in_tool is exactly one of +/-X, +/-Y, +/-Z.
    Eigen::Matrix3d R;
    // Default: tool X->r, Y->u, Z->f (tool +Z forward). Change mapping if your forward is -Z, +X, etc.
    R.col(0) = r;
    R.col(1) = u;
    R.col(2) = f;

    // If your forward is -Z, flip the Z column (and recompute a consistent basis):
    if (tool_forward_in_tool == Eigen::Vector3d(0, 0, -1))
    {
        R.col(2) = -f;
        // keep a right-handed frame: choose r the same, recompute u
        R.col(1) = R.col(2).cross(R.col(0)).normalized(); // u = z x x
                                                          // (R already orthonormal enough; this keeps roll stable)
    }
    else if (tool_forward_in_tool == Eigen::Vector3d::UnitX())
    {
        R.col(0) = f;
        R.col(1) = up; // keep up close to world_up
        R.col(2) = R.col(0).cross(R.col(1)).normalized();
    }
    else if (tool_forward_in_tool == -Eigen::Vector3d::UnitX())
    {
        R.col(0) = -f;
        R.col(1) = up;
        R.col(2) = R.col(0).cross(R.col(1)).normalized();
    }
    else if (tool_forward_in_tool == Eigen::Vector3d::UnitY())
    {
        R.col(1) = f;
        R.col(0) = r;
        R.col(2) = R.col(0).cross(R.col(1)).normalized();
    }
    else if (tool_forward_in_tool == -Eigen::Vector3d::UnitY())
    {
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

geometry_msgs::msg::Pose CrackleManipulation::construct_reach_pose(geometry_msgs::msg::Pose object_pose, Vector3 tool_offset)
{
    geometry_msgs::msg::Pose target_pose;
    target_pose.position = object_pose.position;
    target_pose.position.x += tool_offset.x; // offset approach
    target_pose.position.y += tool_offset.y;
    target_pose.position.z += tool_offset.z;

    Eigen::Vector3d obj_position(object_pose.position.x, object_pose.position.y, object_pose.position.z);
    Eigen::Vector3d target_position_eig(target_pose.position.x, target_pose.position.y, target_pose.position.z);
    Eigen::Vector3d to_dir_world = (obj_position - target_position_eig).normalized(); // from target to object

    target_pose.orientation = lookAtQuat(
        to_dir_world,             // to_dir_world (pointing from gripper to object)
        Eigen::Vector3d::UnitZ(), // world_up
        kToolForwardInTool);      // tool_forward_in_tool

    RCLCPP_INFO(node_->get_logger(), "reach_for_object: trying pose [%f, %f, %f]", target_pose.position.x, target_pose.position.y, target_pose.position.z);
    return target_pose;
}

std::vector<geometry_msgs::msg::Pose> CrackleManipulation::get_grasp_poses(moveit_msgs::msg::CollisionObject object, double approach_dist, double tool_width)
{
    std::vector<geometry_msgs::msg::Pose> grasp_poses;

    // Assuming the object has a single primitive shape and pose for simplicity
    if (object.primitives.empty() || object.primitive_poses.empty())
    {
        RCLCPP_WARN(rclcpp::get_logger("crackle_moveit_manipulation_node"), "get_grasp_poses: object has no primitives or poses");
        return grasp_poses;
    }

    const shape_msgs::msg::SolidPrimitive &primitive = object.primitives[0];
    const geometry_msgs::msg::Pose &obj_pose = object.primitive_poses[0];

    // Calculate grasp poses based on the object's dimensions and pose
    if (primitive.type == shape_msgs::msg::SolidPrimitive::BOX)
    {
        double length = primitive.dimensions[shape_msgs::msg::SolidPrimitive::BOX_X];
        double width = primitive.dimensions[shape_msgs::msg::SolidPrimitive::BOX_Y];
        double height = primitive.dimensions[shape_msgs::msg::SolidPrimitive::BOX_Z];

        // Basic validation
        if (length <= 0.0 || width <= 0.0 || height <= 0.0)
        {
            RCLCPP_WARN(rclcpp::get_logger("crackle_moveit_manipulation_node"), "get_grasp_poses: invalid box dimension");
            return grasp_poses;
        }

        std::vector<Eigen::Vector3d> offsets;
        // Force top-down grasp only for now
        offsets.push_back({0.0, 0.0, approach_dist});

        for (const auto &offset : offsets)
        {
            geometry_msgs::msg::Pose grasp_pose;
            grasp_pose.position.x = obj_pose.position.x + offset.x();
            grasp_pose.position.y = obj_pose.position.y + offset.y();
            grasp_pose.position.z = obj_pose.position.z + height / 2.0 + offset.z();

            // Force top-down orientation: tool forward points toward -Z in world
            grasp_pose.orientation = lookAtQuat(
                -Eigen::Vector3d::UnitZ(),
                Eigen::Vector3d::UnitY(),
                kToolForwardInTool);

            grasp_poses.push_back(grasp_pose);
        }
    }

    return grasp_poses;
}

bool CrackleManipulation::reach_for_object(const std::string &object_name)
{

    // Find object in the planning scene
    std::map<std::string, moveit_msgs::msg::CollisionObject> scene_objects;
    scene_objects = planning_scene_->getObjects();
    if (scene_objects.find(object_name) == scene_objects.end())
    {
        RCLCPP_ERROR(node_->get_logger(), "reach_for_object: object '%s' not found in planning scene", object_name.c_str());
        return false;
    }
    else
    {
        RCLCPP_INFO(node_->get_logger(), "reach_for_object: object '%s' found in planning scene", object_name.c_str());
        // print the collision object pose
        const moveit_msgs::msg::CollisionObject &obj = scene_objects[object_name];
        RCLCPP_INFO(node_->get_logger(), "Object ID: %s", obj.id.c_str());
        RCLCPP_INFO(node_->get_logger(), "Object Frame ID: %s", obj.header.frame_id.c_str());
        RCLCPP_INFO(node_->get_logger(), "Object Pose: [%f, %f, %f]", obj.pose.position.x, obj.pose.position.y, obj.pose.position.z);
        RCLCPP_INFO(node_->get_logger(), "Object Primitive Pose: [%f, %f, %f]", obj.primitive_poses[0].position.x, obj.primitive_poses[0].position.y, obj.primitive_poses[0].position.z);
        RCLCPP_INFO(node_->get_logger(), "Object Orientation: [%f, %f, %f, %f]", obj.primitive_poses[0].orientation.x, obj.primitive_poses[0].orientation.y, obj.primitive_poses[0].orientation.z, obj.primitive_poses[0].orientation.w);
        RCLCPP_INFO(node_->get_logger(), "Object Dimensions: [%f, %f, %f]", obj.primitives[0].dimensions[0], obj.primitives[0].dimensions[1], obj.primitives[0].dimensions[2]);

        // Add possible reach poses to this vector from lowest costing to highest costing
        std::vector<geometry_msgs::msg::Pose> target_reach_poses;
        std::vector<bool> reach_success;

        target_reach_poses.push_back(construct_reach_pose(obj.pose, {0.05, 0.05, 0.2}));   // above object
        target_reach_poses.push_back(construct_reach_pose(obj.pose, {-0.05, -0.05, 0.2})); // above object

        bool success = false;
        for (const auto &pose : target_reach_poses)
        {
            std::vector<geometry_msgs::msg::Pose> pose_vector = {pose};
            geometry_msgs::msg::Pose target_approach_pose = construct_reach_pose(obj.pose, {(obj.pose.position.x - pose.position.x) / 2, (obj.pose.position.y - pose.position.y) / 2, 0.2});

            pose_vector.push_back(target_approach_pose); // add approach pose to the end of the vector
            // success = plan_to_pose(pose);
            // success = plan_cartesian_path(pose_vector);
            success = plan_to_pose(pose);
            reach_success.push_back(success);
        }

        // Try to execute the first successful plan
        for (size_t i = 0; i < reach_success.size(); ++i)
        {
            if (reach_success[i])
            {
                RCLCPP_INFO(node_->get_logger(), "reach_for_object: planned to object '%s' successfully", object_name.c_str());
                {
                    // Build a pose 0.05 m closer to the object along the vector from the approach pose to the object
                    const geometry_msgs::msg::Pose approach_pose = target_reach_poses[i];
                    Eigen::Vector3d approach_pt(approach_pose.position.x,
                                                approach_pose.position.y,
                                                approach_pose.position.z);
                    Eigen::Vector3d object_pt(obj.pose.position.x,
                                              obj.pose.position.y,
                                              obj.pose.position.z);

                    Eigen::Vector3d dir = object_pt - approach_pt;
                    double norm = dir.norm();
                    if (norm < 1e-6)
                    {
                        RCLCPP_WARN(node_->get_logger(), "reach_for_object: zero distance between approach and object, skipping close-in step");
                    }
                    else
                    {
                        dir.normalize();
                        const double closer_step = 0.05; // meters toward the object

                        geometry_msgs::msg::Pose closer_pose = approach_pose;
                        closer_pose.position.x += dir.x() * closer_step;
                        closer_pose.position.y += dir.y() * closer_step;
                        closer_pose.position.z += dir.z() * closer_step;
                        // keep same orientation as approach_pose

                        // Plan a Cartesian path that goes through the approach pose then the closer pose
                        std::vector<geometry_msgs::msg::Pose> cartesian_waypoints;
                        cartesian_waypoints.push_back(approach_pose);
                        cartesian_waypoints.push_back(closer_pose);

                        bool cart_ok = plan_cartesian_path(cartesian_waypoints);
                        if (cart_ok)
                        {
                            RCLCPP_INFO(node_->get_logger(), "reach_for_object: planned cartesian close-in path");
                            // execution will follow the existing execute_plan(true) call below,
                            // which will run the recently planned trajectory_ (is_trajectory_ == true)
                        }
                        else
                        {
                            RCLCPP_WARN(node_->get_logger(), "reach_for_object: failed to plan cartesian close-in path, will execute nominal plan");
                            // leave plan_ intact so the subsequent execute_plan(true) executes the earlier plan_
                        }
                    }
                }

                success = execute_plan(true);
                if (success)
                {
                    RCLCPP_INFO(node_->get_logger(), "reach_for_object: executed to object '%s' successfully", object_name.c_str());
                    std_msgs::msg::Bool msg;
                    msg.data = false;
                    gripper_command_publisher_->publish(msg);
                    return true;
                }
            }
        }

        // close in on the object

        
        RCLCPP_ERROR(node_->get_logger(), "reach_for_object: failed to reach for object '%s'", object_name.c_str());
    }

    return false;
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    auto manipulation = std::make_shared<CrackleManipulation>("lite6");

    rclcpp::executors::MultiThreadedExecutor executor(rclcpp::ExecutorOptions(), 2);
    executor.add_node(manipulation->node_);
    executor.spin();

    rclcpp::shutdown();
    return 0;
}
