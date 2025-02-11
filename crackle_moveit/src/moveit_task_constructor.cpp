#include "rclcpp/rclcpp.hpp"


class MoveItTaskConstructor : public rclcpp::Node
{
public:
    MoveItTaskConstructor() : Node("moveit_task_constructor")
    {
        RCLCPP_INFO(get_logger(), "moveit_task_constructor start");

        // create a service client to call /xarm_pose_plan service  
        
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MoveItTaskConstructor>());
    rclcpp::shutdown();
    return 0;
}