
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

const double jump_threshold = 0.0;
const double eef_step = 0.005;
const double max_velocity_scaling_factor = 0.3;  // [move_group_interface] default is 0.1
const double max_acceleration_scaling_factor = 0.1;  // [move_group_interface] default is 0.1


CrackleManipulation::CrackleManipulation(const std::string& group_name)
: logger_(rclcpp::get_logger("crackle_moveit_manipulation_node"))
{
    node_ = rclcpp::Node::make_shared("crackle_moveit_manipulation_node");

    initialize(group_name);
}

void CrackleManipulation::initialize(const std::string& group_name) {
    move_group_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(node_, group_name);
    RCLCPP_INFO(logger_, "Planning frame: %s", move_group_->getPlanningFrame().c_str());
    RCLCPP_INFO(logger_, "End effector link: %s", move_group_->getEndEffectorLink().c_str());
    RCLCPP_INFO(logger_, "Available Planning Groups:");
    std::copy(move_group_->getJointModelGroupNames().begin(), move_group_->getJointModelGroupNames().end(), std::ostream_iterator<std::string>(std::cout, ", "));
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
bool CrackleManipulation::move_to_pose(const geometry_msgs::msg::Pose& target_pose) {
    move_group_->setPoseTarget(target_pose);

    moveit::planning_interface::MoveGroupInterface::Plan plan;
    bool success = static_cast<bool>(move_group_->plan(plan));

    if (success) {
        auto exec_result = move_group_->execute(plan);
        if (exec_result == moveit::core::MoveItErrorCode::SUCCESS) {
            RCLCPP_INFO(logger_, "Planning and execution succeeded!");
        } else {
            RCLCPP_ERROR(logger_, "Execution failed with code %d", static_cast<int>(exec_result.val));
            success = false;
        }
    } else {
        RCLCPP_ERROR(logger_, "Planning failed!");
    }

    move_group_->clearPoseTargets();
    return success;
}

int main(int argc, char * argv[])
{
    // Initialize ROS and create the Node
    rclcpp::init(argc, argv);

    CrackleManipulation manipulation = CrackleManipulation("lite6");
    // Next step goes here
    RCLCPP_INFO(manipulation.getLogger(), "Moveit Manipulation is running...");

    // Define a target pose
    auto const target_pose = []{
        geometry_msgs::msg::Pose msg;
        msg.orientation.w = 1.0;
        msg.position.x = 0.28;
        msg.position.y = -0.2;
        msg.position.z = 0.5;
        return msg;
    }();

    // Move to the target pose
    manipulation.move_to_pose(target_pose);


    // Shutdown ROS
    rclcpp::shutdown();
    return 0;
}
