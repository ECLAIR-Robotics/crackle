#ifndef __CRACKLE_PLANNER_H__
#define __CRACKLE_PLANNER_H__
#include <rclcpp/rclcpp.hpp>

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <moveit_msgs/msg/display_robot_state.hpp>
#include <moveit_msgs/msg/display_trajectory.hpp>
#include <shape_msgs/msg/solid_primitive.hpp>
#include <shape_msgs/msg/mesh.hpp>
#include <moveit_msgs/msg/collision_object.hpp>

class CrackleMoveitSceneUpdater {
    public:
        CrackleMoveitSceneUpdater(const rclcpp::Node::SharedPtr& node);
        CrackleMoveitSceneUpdater();
        ~CrackleMoveitSceneUpdater() {};

        std::string moveStringNameInt(std::string target_string);
        void addCollisionObject();
        void removeCollisionObject();
        void attachObject();
        void detachObject();
        void updateObject();

        rclcpp::Publisher<moveit_msgs::msg::PlanningScene>::SharedPtr planning_scene_diff_publisher_;
        rclcpp::Subscription<moveit_msgs::msg::CollisionObject>::SharedPtr collision_object_subscription_;
    private:
        void init();

        rclcpp::Node::SharedPtr node_;
        std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_;
        std::shared_ptr<moveit::planning_interface::PlanningSceneInterface> planning_scene_;
        bool is_trajectory_;
};
#endif