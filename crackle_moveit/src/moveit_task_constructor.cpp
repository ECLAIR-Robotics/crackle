#include "rclcpp/rclcpp.hpp"
#include <crackle_moveit/moveit_task_constructor.hpp>

const double jump_threshold = 0.0;
const double eef_step = 0.005;
const double max_velocity_scaling_factor = 0.3;  // [move_group_interface] default is 0.1
const double max_acceleration_scaling_factor = 0.1;  // [move_group_interface] default is 0.1

CrackleMoveitTaskConstructorNode::CrackleMoveitTaskConstructorNode(const rclcpp::Node::SharedPtr& node, const std::string& group_name)
    : node_(node)
{
    init(group_name);
}

void CrackleMoveitTaskConstructorNode::init(const std::string& group_name) {
    move_group_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(node_, group_name);
    RCLCPP_INFO(node_->get_logger(), "Planning frame: %s", move_group_->getPlanningFrame().c_str());
    RCLCPP_INFO(node_->get_logger(), "End effector link: %s", move_group_->getEndEffectorLink().c_str());
    RCLCPP_INFO(node_->get_logger(), "Available Planning Groups:");
    std::copy(move_group_->getJointModelGroupNames().begin(), move_group_->getJointModelGroupNames().end(), std::ostream_iterator<std::string>(std::cout, ", "));
    move_group_->setMaxVelocityScalingFactor(max_velocity_scaling_factor);
    move_group_->setMaxAccelerationScalingFactor(max_acceleration_scaling_factor);
}

void exit_sig_handler(int signum)
{
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Received signal %d", signum);
    rclcpp::shutdown();
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::NodeOptions node_options;
    node_options.automatically_declare_parameters_from_overrides(true);
    std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("xarm_planner_move_group_node", node_options);
    RCLCPP_INFO(node->get_logger(), "crackle_moveit_task_constructor_node start");
    signal(SIGINT, exit_sig_handler);

    RCLCPP_INFO(node->get_logger(), "namespace: %s, group_name: %s", node->get_namespace(), "lite6");
    std::shared_ptr<CrackleMoveitTaskConstructorNode> crackle_moveit_task_constructor_node = std::make_shared<CrackleMoveitTaskConstructorNode>(node, "lite6");

    rclcpp::shutdown();
    return 0;
}