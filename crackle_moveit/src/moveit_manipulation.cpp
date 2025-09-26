
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
const double eef_step = 0.005;
const double max_velocity_scaling_factor = 0.3;     // [move_group_interface] default is 0.1
const double max_acceleration_scaling_factor = 0.1; // [move_group_interface] default is 0.1

static const Eigen::Vector3d kToolForwardInTool(0.0, 0.0, 1.0); // Z axis

CrackleManipulation::CrackleManipulation(const std::string &group_name)
    : logger_(rclcpp::get_logger("crackle_moveit_manipulation_node"))
{
    node_ = rclcpp::Node::make_shared("crackle_moveit_manipulation_node");
    pickup_service_ = node_->create_service<crackle_interfaces::srv::PickupObject>(
        "crackle_manipulation/pickup_object",
        std::bind(&CrackleManipulation::pick_up_object, this, std::placeholders::_1, std::placeholders::_2));
    initialize(group_name);
}

bool CrackleManipulation::pick_up_object(crackle_interfaces::srv::PickupObject::Request::SharedPtr request,
                                         crackle_interfaces::srv::PickupObject::Response::SharedPtr response)
{
    std::string object_name = request->object_name;
    RCLCPP_INFO(node_->get_logger(), "pick_up_object: requested to pick up");

    // First, reach for the object
    bool success = reach_for_object(object_name);
    if (!success)
    {
        RCLCPP_ERROR(node_->get_logger(), "pick_up_object: failed to reach for object '%s'", object_name.c_str());
        return false;
    }

    // Next, close the gripper (this is a placeholder, actual gripper control code needed)
    RCLCPP_INFO(node_->get_logger(), "pick_up_object: closing gripper to pick up object '%s'", object_name.c_str());

    // TODO: Implement actual gripper control logic here
    response->success = true;
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
bool CrackleManipulation::plan_cartesian_path(const std::vector<geometry_msgs::msg::Pose> &pose_target_vector)
{
    if (pose_target_vector.empty())
    {
        RCLCPP_WARN(node_->get_logger(), "planCartesianPath: empty target vector");
        return false;
    }
    double fraction = move_group_->computeCartesianPath(pose_target_vector, eef_step, jump_threshold, trajectory_);
    if (fraction < 0.99)
    {
        RCLCPP_ERROR(node_->get_logger(), "planCartesianPath: only %.2f%% of path planned", fraction * 100.0);
        return false;
    }
    is_trajectory_ = true;
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

std::vector<Vector3> CrackleManipulation::compute_grip_points_for_box(const std::vector<double>& dimensions, const geometry_msgs::msg::Pose& object_pose)
{
    std::vector<Vector3> grip_points;
    
    // Box dimensions: [width (x), depth (y), height (z)]
    double width = dimensions[0];
    double depth = dimensions[1]; 
    double height = dimensions[2];
    
    // Define safe approach distances based on gripper size (assuming ~5cm gripper width)
    const double safe_distance = 0.08; // 8cm approach distance
    const double min_grip_size = 0.04; // 4cm minimum grip dimension
    
    // Priority order: top -> sides (prefer larger faces)
    // Top approach (highest priority for pick-up operations)
    grip_points.push_back({0.0, 0.0, height/2 + safe_distance});
    
    // Side approaches - only if object dimensions allow gripper to fit
    if (depth > min_grip_size) {
        // Approach from +X side
        grip_points.push_back({width/2 + safe_distance, 0.0, height/4});
        // Approach from -X side  
        grip_points.push_back({-(width/2 + safe_distance), 0.0, height/4});
    }
    
    if (width > min_grip_size) {
        // Approach from +Y side
        grip_points.push_back({0.0, depth/2 + safe_distance, height/4});
        // Approach from -Y side
        grip_points.push_back({0.0, -(depth/2 + safe_distance), height/4});
    }
    
    // If object is very flat, add angled approaches
    if (height < min_grip_size) {
        grip_points.push_back({width/3, depth/3, safe_distance});
        grip_points.push_back({-width/3, -depth/3, safe_distance});
    }
    
    RCLCPP_INFO(node_->get_logger(), "Computed %zu grip points for box [%.3f x %.3f x %.3f]", 
                grip_points.size(), width, depth, height);
    
    return grip_points;
}

std::vector<Vector3> CrackleManipulation::compute_grip_points_for_sphere(double radius, const geometry_msgs::msg::Pose& object_pose)
{
    std::vector<Vector3> grip_points;
    
    const double safe_distance = 0.08; // 8cm approach distance
    const double approach_radius = radius + safe_distance;
    
    // For spheres, approach from multiple angles around the equator and from top
    // Top approach (highest priority)
    grip_points.push_back({0.0, 0.0, approach_radius});
    
    // Equatorial approaches at 45-degree intervals for good coverage
    const int num_approaches = 8;
    for (int i = 0; i < num_approaches; ++i) {
        double angle = 2.0 * M_PI * i / num_approaches;
        double x = approach_radius * cos(angle);
        double y = approach_radius * sin(angle);
        double z = radius * 0.3; // Slightly above center for better grip
        grip_points.push_back({x, y, z});
    }
    
    // If sphere is small enough, add angled top approaches
    if (radius < 0.05) { // 5cm radius threshold
        grip_points.push_back({approach_radius * 0.5, 0.0, approach_radius * 0.866}); // 60 degrees
        grip_points.push_back({0.0, approach_radius * 0.5, approach_radius * 0.866});
        grip_points.push_back({-approach_radius * 0.5, 0.0, approach_radius * 0.866});
        grip_points.push_back({0.0, -approach_radius * 0.5, approach_radius * 0.866});
    }
    
    RCLCPP_INFO(node_->get_logger(), "Computed %zu grip points for sphere with radius %.3f", 
                grip_points.size(), radius);
    
    return grip_points;
}

std::vector<Vector3> CrackleManipulation::compute_shape_aware_grip_points(const moveit_msgs::msg::CollisionObject& obj)
{
    std::vector<Vector3> grip_points;
    
    if (obj.primitives.empty()) {
        RCLCPP_WARN(node_->get_logger(), "Object has no primitive shapes, using default grip points");
        // Fallback to simple default points
        grip_points.push_back({0.0, 0.0, 0.1});
        grip_points.push_back({0.1, 0.1, 0.1});
        return grip_points;
    }
    
    const auto& primitive = obj.primitives[0];
    
    switch (primitive.type) {
        case shape_msgs::msg::SolidPrimitive::BOX: {
            if (primitive.dimensions.size() >= 3) {
                grip_points = compute_grip_points_for_box(primitive.dimensions, obj.primitive_poses[0]);
                RCLCPP_INFO(node_->get_logger(), "Using box-specific grip computation");
            } else {
                RCLCPP_WARN(node_->get_logger(), "Invalid box dimensions, using default grip points");
                grip_points.push_back({0.0, 0.0, 0.1});
            }
            break;
        }
        case shape_msgs::msg::SolidPrimitive::SPHERE: {
            if (!primitive.dimensions.empty()) {
                double radius = primitive.dimensions[0]; // For sphere, dimensions[0] is radius
                grip_points = compute_grip_points_for_sphere(radius, obj.primitive_poses[0]);
                RCLCPP_INFO(node_->get_logger(), "Using sphere-specific grip computation");
            } else {
                RCLCPP_WARN(node_->get_logger(), "Invalid sphere dimensions, using default grip points");
                grip_points.push_back({0.0, 0.0, 0.1});
            }
            break;
        }
        case shape_msgs::msg::SolidPrimitive::CYLINDER: {
            // Treat cylinder similar to box but with circular cross-section
            if (primitive.dimensions.size() >= 2) {
                double radius = primitive.dimensions[0];
                double height = primitive.dimensions[1];
                // Create box-like dimensions for cylinder
                std::vector<double> cylinder_dims = {radius * 2, radius * 2, height};
                grip_points = compute_grip_points_for_box(cylinder_dims, obj.primitive_poses[0]); 
                RCLCPP_INFO(node_->get_logger(), "Using cylinder-specific grip computation (as box)");
            } else {
                RCLCPP_WARN(node_->get_logger(), "Invalid cylinder dimensions, using default grip points");
                grip_points.push_back({0.0, 0.0, 0.1});
            }
            break;
        }
        default: {
            RCLCPP_WARN(node_->get_logger(), "Unknown primitive type %d, using default grip points", primitive.type);
            grip_points.push_back({0.0, 0.0, 0.1});
            grip_points.push_back({0.1, 0.1, 0.1});
            break;
        }
    }
    
    return grip_points;
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
        
        // Check if object has valid dimensions before printing
        if (!obj.primitives.empty() && obj.primitives[0].dimensions.size() >= 3) {
            RCLCPP_INFO(node_->get_logger(), "Object Dimensions: [%f, %f, %f]", obj.primitives[0].dimensions[0], obj.primitives[0].dimensions[1], obj.primitives[0].dimensions[2]);
        } else if (!obj.primitives.empty() && obj.primitives[0].dimensions.size() >= 1) {
            RCLCPP_INFO(node_->get_logger(), "Object has %zu dimension(s): [%f]", obj.primitives[0].dimensions.size(), obj.primitives[0].dimensions[0]);
        } else {
            RCLCPP_WARN(node_->get_logger(), "Object has no valid dimensions");
        }

        // Compute shape-aware grip points instead of using hardcoded offsets
        std::vector<Vector3> shape_aware_grip_points = compute_shape_aware_grip_points(obj);
        
        RCLCPP_INFO(node_->get_logger(), "Generated %zu shape-aware grip points for object '%s'", 
                    shape_aware_grip_points.size(), object_name.c_str());
        
        // Add possible reach poses to this vector from lowest costing to highest costing
        std::vector<geometry_msgs::msg::Pose> target_reach_poses;
        std::vector<bool> reach_success;

        // Convert shape-aware grip points to reach poses
        for (const auto& grip_point : shape_aware_grip_points) {
            target_reach_poses.push_back(construct_reach_pose(obj.pose, grip_point));
        }
        
        // If no shape-aware points generated, fall back to default approach
        if (target_reach_poses.empty()) {
            RCLCPP_WARN(node_->get_logger(), "No shape-aware grip points generated, using fallback approach");
            target_reach_poses.push_back(construct_reach_pose(obj.pose, {0.0, 0.0, 0.1}));   // above object
            target_reach_poses.push_back(construct_reach_pose(obj.pose, {0.1, 0.0, 0.1}));   // side approach
        }

        bool success = false;
        for (const auto &pose : target_reach_poses)
        {
            std::vector<geometry_msgs::msg::Pose> pose_vector = {pose};
            geometry_msgs::msg::Pose target_approach_pose = construct_reach_pose(obj.pose, {(obj.pose.position.x - pose.position.x)/2, (obj.pose.position.y - pose.position.y)/2, 0.05});

            pose_vector.push_back(target_approach_pose); // add approach pose to the end of the vector
            // success = plan_to_pose(pose);
            success = plan_cartesian_path(pose_vector);
            reach_success.push_back(success);
        }

        // Try to execute the first successful plan
        for (size_t i = 0; i < reach_success.size(); ++i)
        {
            if (reach_success[i])
            {
                RCLCPP_INFO(node_->get_logger(), "reach_for_object: planned to object '%s' successfully", object_name.c_str());
                success = execute_plan(true);
                if (success)
                {
                    RCLCPP_INFO(node_->get_logger(), "reach_for_object: executed to object '%s' successfully", object_name.c_str());
                    return true;
                }
            }
        }
        RCLCPP_ERROR(node_->get_logger(), "reach_for_object: failed to reach for object '%s'", object_name.c_str());
    }

    return false;
}

int main(int argc, char *argv[])
{
    // Initialize ROS and create the Node
    rclcpp::init(argc, argv);

    CrackleManipulation manipulation = CrackleManipulation("lite6");
    // Next step goes here
    RCLCPP_INFO(manipulation.getLogger(), "Moveit Manipulation is running...");

    // Shutdown ROS
    rclcpp::spin(manipulation.node_);
    rclcpp::shutdown();
    return 0;
}
