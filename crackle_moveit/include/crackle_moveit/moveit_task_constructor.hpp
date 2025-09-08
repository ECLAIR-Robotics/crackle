#ifndef __CRACKLE_PLANNER_H__
#define __CRACKLE_PLANNER_H__

#include <memory>
#include <signal.h>
#include <rclcpp/rclcpp.hpp>

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit/task_constructor/task.h>
#include <moveit/task_constructor/solvers.h>
#include <moveit/task_constructor/stages.h>
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

static const rclcpp::Logger LOGGER = rclcpp::get_logger("mtc_tutorial");
namespace mtc = moveit::task_constructor;

using std::placeholders::_1;
using std::placeholders::_2;

class CrackleManipulation {
public:
    CrackleManipulation(const rclcpp::Node::SharedPtr& node);
    ~CrackleManipulation();
private:
    // Add member variables and methods as needed
    void initialize(const rclcpp::Node::SharedPtr& node_, const rclcpp::Logger& logger);
};

#endif // __XARM_PLANNER_H__