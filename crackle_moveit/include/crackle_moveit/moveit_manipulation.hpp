#ifndef __CRACKLE_PLANNER_H__
#define __CRACKLE_PLANNER_H__

#include <memory>
#include <signal.h>
#include <rclcpp/rclcpp.hpp>

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene/planning_scene.h>
#include <moveit/trajectory_processing/iterative_time_parameterization.h>
#include <moveit/robot_trajectory/robot_trajectory.h>
#include <moveit/robot_state/robot_state.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <crackle_interfaces/srv/pickup_object.hpp>
#include <crackle_interfaces/srv/look_at.hpp>
#include <crackle_interfaces/srv/plan_pose.hpp>
#include <crackle_interfaces/srv/execute_plan.hpp>
#include <crackle_interfaces/srv/plan_trajectory.hpp>
#include <crackle_interfaces/srv/set_joint_angles.hpp>
#include <crackle_interfaces/srv/face_direction_relative.hpp>
#include <crackle_interfaces/srv/move_relative.hpp>
#include <crackle_interfaces/srv/get_end_effector_pose.hpp>
#include <crackle_interfaces/srv/demo_trajectory.hpp>
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
#include <geometry_msgs/msg/vector3_stamped.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <std_msgs/msg/bool.hpp>
#include <rmw/qos_profiles.h>
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
    CrackleManipulation(const std::string &group_name);
    ~CrackleManipulation() {};

    moveit::planning_interface::MoveGroupInterface &getMoveGroup() { return *move_group_; }
    bool plan_to_pose(const geometry_msgs::msg::Pose &target_pose);
    bool execute_plan(bool wait);
    bool plan_cartesian_path(const std::vector<geometry_msgs::msg::Pose> &pose_target_vector);
    bool reach_for_object(const std::string &object_name);
    bool plan_pose_service(crackle_interfaces::srv::PlanPose::Request::SharedPtr request,
                           crackle_interfaces::srv::PlanPose::Response::SharedPtr response);
    bool execute_plan_service(crackle_interfaces::srv::ExecutePlan::Request::SharedPtr request,
                              crackle_interfaces::srv::ExecutePlan::Response::SharedPtr response);
    bool plan_trajectory_service(crackle_interfaces::srv::PlanTrajectory::Request::SharedPtr request,
                                 crackle_interfaces::srv::PlanTrajectory::Response::SharedPtr response);
    bool set_joint_angles_service(crackle_interfaces::srv::SetJointAngles::Request::SharedPtr request,
                                  crackle_interfaces::srv::SetJointAngles::Response::SharedPtr response);
    bool face_direction_relative_service(crackle_interfaces::srv::FaceDirectionRelative::Request::SharedPtr request,
                                         crackle_interfaces::srv::FaceDirectionRelative::Response::SharedPtr response);
    bool move_relative_service(crackle_interfaces::srv::MoveRelative::Request::SharedPtr request,
                               crackle_interfaces::srv::MoveRelative::Response::SharedPtr response);
    bool get_end_effector_pose_service(crackle_interfaces::srv::GetEndEffectorPose::Request::SharedPtr request,
                                       crackle_interfaces::srv::GetEndEffectorPose::Response::SharedPtr response);
    bool demo_trajectory_service(crackle_interfaces::srv::DemoTrajectory::Request::SharedPtr request,
                                 crackle_interfaces::srv::DemoTrajectory::Response::SharedPtr response);
    bool pick_up_object(crackle_interfaces::srv::PickupObject::Request::SharedPtr request,
                        crackle_interfaces::srv::PickupObject::Response::SharedPtr response);
    bool look_at(crackle_interfaces::srv::LookAt::Request::SharedPtr request,
                 crackle_interfaces::srv::LookAt::Response::SharedPtr response);
    rclcpp::Logger &getLogger() { return logger_; }
    rclcpp::Node::SharedPtr node_;
    geometry_msgs::msg::Quaternion lookAtQuat(
        const Eigen::Vector3d &to_dir_world,
        const Eigen::Vector3d &world_up,
        const Eigen::Vector3d &tool_forward_in_tool);
    geometry_msgs::msg::Pose construct_reach_pose(geometry_msgs::msg::Pose object_pose, Vector3 tool_offset);
    std::vector<geometry_msgs::msg::Pose> get_grasp_poses(moveit_msgs::msg::CollisionObject object, double approach_dist, double tool_width);
    bool dance_dance(std_srvs::srv::Trigger::Request::SharedPtr request,
                     std_srvs::srv::Trigger::Response::SharedPtr response);

private:
    void initialize(const std::string &group_name);
    bool wait_for_current_state(const std::string &caller, double timeout_sec = 5.0);
    rclcpp::Logger logger_;
    rclcpp::Service<crackle_interfaces::srv::PickupObject>::SharedPtr pickup_service_;
    rclcpp::Service<crackle_interfaces::srv::LookAt>::SharedPtr look_at_service_;
    rclcpp::Service<crackle_interfaces::srv::PlanPose>::SharedPtr plan_pose_service_;
    rclcpp::Service<crackle_interfaces::srv::ExecutePlan>::SharedPtr execute_plan_service_;
    rclcpp::Service<crackle_interfaces::srv::PlanTrajectory>::SharedPtr plan_trajectory_service_;
    rclcpp::Service<crackle_interfaces::srv::SetJointAngles>::SharedPtr set_joint_angles_service_;
    rclcpp::Service<crackle_interfaces::srv::FaceDirectionRelative>::SharedPtr face_direction_relative_service_;
    rclcpp::Service<crackle_interfaces::srv::MoveRelative>::SharedPtr move_relative_service_;
    rclcpp::Service<crackle_interfaces::srv::GetEndEffectorPose>::SharedPtr get_end_effector_pose_service_;
    rclcpp::Service<crackle_interfaces::srv::DemoTrajectory>::SharedPtr demo_trajectory_service_;
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr dance_service_;
    rclcpp::CallbackGroup::SharedPtr services_cb_group_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr gripper_command_publisher_;
    rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_state_sub_;
    rclcpp::Time last_joint_state_stamp_;
    bool have_joint_state_;
    std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_;
    std::shared_ptr<moveit::planning_interface::PlanningSceneInterface> planning_scene_;
    moveit::planning_interface::MoveGroupInterface::Plan plan_;
    moveit_msgs::msg::RobotTrajectory trajectory_;
    bool is_trajectory_;
};

#endif // __CRACKLE_PLANNER_H__