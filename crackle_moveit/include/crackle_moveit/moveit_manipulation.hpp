#ifndef __CRACKLE_PLANNER_H__
#define __CRACKLE_PLANNER_H__

#include <memory>
#include <signal.h>
#include <rclcpp/rclcpp.hpp>

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <crackle_interfaces/srv/pickup_object.hpp>

#include <std_srvs/srv/trigger.hpp>

#include <moveit_msgs/msg/display_robot_state.hpp>
#include <moveit_msgs/msg/display_trajectory.hpp>
// #include <moveit_visual_tools/moveit_visual_tools.h>

#include <std_msgs/msg/bool.hpp>
#include <xarm_msgs/srv/plan_pose.hpp>
#include <xarm_msgs/srv/plan_joint.hpp>
#include <xarm_msgs/srv/plan_exec.hpp>
#include <xarm_msgs/srv/plan_single_straight.hpp>

class CrackleManipulation
{
    public:
        rclcpp::Logger logger_;
        CrackleManipulation(const std::string& group_name);
        ~CrackleManipulation() {};
    
        moveit::planning_interface::MoveGroupInterface& getMoveGroup() { return *move_group_; }
        bool move_to_pose(const geometry_msgs::msg::Pose& target_pose);

        // Return logger
        rclcpp::Logger& getLogger() { return logger_; }

    private:
        void initialize(const std::string& group_name);
    
        rclcpp::Node::SharedPtr node_;
        std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_;
        std::shared_ptr<moveit::planning_interface::PlanningSceneInterface> planning_scene_;
        moveit_msgs::msg::RobotTrajectory trajectory_;
        bool is_trajectory_;
};

#endif // __CRACKLE_PLANNER_H__