#ifndef __CRACKLE_PLANNER_H__
#define __CRACKLE_PLANNER_H__

#include <signal.h>
#include <rclcpp/rclcpp.hpp>

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit/task_constructor/task.h>
#include <moveit/task_constructor/solvers.h>
#include <moveit/task_constructor/stages.h>

#include <moveit_msgs/msg/display_robot_state.hpp>
#include <moveit_msgs/msg/display_trajectory.hpp>
// #include <moveit_visual_tools/moveit_visual_tools.h>

#include <std_msgs/msg/bool.hpp>
#include <xarm_msgs/srv/plan_pose.hpp>
#include <xarm_msgs/srv/plan_joint.hpp>
#include <xarm_msgs/srv/plan_exec.hpp>
#include <xarm_msgs/srv/plan_single_straight.hpp>

static const rclcpp::Logger LOGGER = rclcpp::get_logger("mtc_tutorial");
namespace mtc = moveit::task_constructor;

class CrackleMoveitTaskConstructorNode{
    public:
        CrackleMoveitTaskConstructorNode(const rclcpp::Node::SharedPtr& node, const std::string& group_name);
        CrackleMoveitTaskConstructorNode(const std::string& group_name);
        ~CrackleMoveitTaskConstructorNode() {};
        mtc::Task createTask(const std::string& group_name);
        // bool planJointTarget(const std::vector<double>& joint_target);
        bool planPoseTarget(const geometry_msgs::msg::Pose& pose_target);
        // bool planPoseTargets(const std::vector<geometry_msgs::msg::Pose>& pose_target_vector);
        bool planCartesianPath(const std::vector<geometry_msgs::msg::Pose>& pose_target_vector);

        bool executePath(bool wait = true);
    
    private:
        void init(const std::string& group_name);

        rclcpp::Node::SharedPtr node_;
        std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_;
        moveit::planning_interface::MoveGroupInterface::Plan xarm_plan_;
        moveit_msgs::msg::RobotTrajectory trajectory_;
        bool is_trajectory_;
};
#endif // __XARM_PLANNER_H__