#ifndef __CRACKLE_PLANNER_H__
#define __CRACKLE_PLANNER_H__

#include <memory>
#include <signal.h>
#include <rclcpp/rclcpp.hpp>

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene/planning_scene.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <crackle_interfaces/srv/pickup_object.hpp>
#include <tf2_ros/transform_listener.h>
#include <std_srvs/srv/trigger.hpp>
#include <moveit_msgs/msg/display_robot_state.hpp>
#include <moveit_msgs/msg/display_trajectory.hpp>
#include <shape_msgs/msg/solid_primitive.hpp>
#include <shape_msgs/msg/mesh.hpp>
#include <moveit_msgs/msg/collision_object.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/quaternion.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <std_msgs/msg/bool.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <xarm_msgs/srv/plan_pose.hpp>
#include <xarm_msgs/srv/plan_joint.hpp>
#include <xarm_msgs/srv/plan_exec.hpp>
#include <xarm_msgs/srv/plan_single_straight.hpp>
#include <tf2_eigen/tf2_eigen.hpp>
#include <Eigen/Geometry>

struct Vector3
{
    double x;
    double y;
    double z;
};
class CrackleManipulation
{
public:
    CrackleManipulation(const std::string& group_name);
    ~CrackleManipulation() {};
    
    moveit::planning_interface::MoveGroupInterface& getMoveGroup() { return *move_group_; }
    bool plan_to_pose(const geometry_msgs::msg::Pose& target_pose);
    bool execute_plan(bool wait);
    bool plan_cartesian_path(const std::vector<geometry_msgs::msg::Pose>& pose_target_vector);
    bool reach_for_object(const std::string& object_name);
    bool pick_up_object(crackle_interfaces::srv::PickupObject::Request::SharedPtr request,
                        crackle_interfaces::srv::PickupObject::Response::SharedPtr response);
    rclcpp::Logger& getLogger() { return logger_; }
    rclcpp::Node::SharedPtr node_;
    geometry_msgs::msg::Quaternion lookAtQuat(
        const Eigen::Vector3d& to_dir_world,
        const Eigen::Vector3d& world_up,
        const Eigen::Vector3d& tool_forward_in_tool
    );
    geometry_msgs::msg::Pose construct_reach_pose(geometry_msgs::msg::Pose object_pose, Vector3 tool_offset);
    std::vector<geometry_msgs::msg::Pose> get_grasp_poses(moveit_msgs::msg::CollisionObject object, double approach_dist, double tool_width); 
    
private:

    void initialize(const std::string& group_name);
    rclcpp::Logger logger_;
    rclcpp::Service<crackle_interfaces::srv::PickupObject>::SharedPtr pickup_service_;
    std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_;
    std::shared_ptr<moveit::planning_interface::PlanningSceneInterface> planning_scene_;
    moveit::planning_interface::MoveGroupInterface::Plan plan_;
    moveit_msgs::msg::RobotTrajectory trajectory_;
    bool is_trajectory_;
};

#endif // __CRACKLE_PLANNER_H__