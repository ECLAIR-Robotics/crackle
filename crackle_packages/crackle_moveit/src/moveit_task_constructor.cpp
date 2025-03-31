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
    move_group_->setMaxVelocityScalingFactor(max_velocity_scaling_factor);
    move_group_->setMaxAccelerationScalingFactor(max_acceleration_scaling_factor);
}

moveit::task_constructor::Task CrackleMoveitTaskConstructorNode::createTask() {
    mtc::Task task;
    task.stages()->setName("demo task");
    task.loadRobotModel(node_);

    const auto& arm_group_name = "lite6";
    const auto& hand_group_name = "uflite_gripper_link";
    const auto& hand_frame = "link6";

    // Set task properties
    task.setProperty("group", arm_group_name);
    task.setProperty("eef", hand_group_name);
    task.setProperty("ik_frame", hand_frame);

    // Disable warnings for this line, as it's a variable that's set but not used in this example
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    mtc::Stage* current_state_ptr = nullptr;  // Forward current_state on to grasp pose generator
    #pragma GCC diagnostic pop

    auto stage_state_current = std::make_unique<mtc::stages::CurrentState>("current");
    current_state_ptr = stage_state_current.get();
    task.add(std::move(stage_state_current));

    auto sampling_planner = std::make_shared<mtc::solvers::PipelinePlanner>(node_);
    auto interpolation_planner = std::make_shared<mtc::solvers::JointInterpolationPlanner>();

    auto cartesian_planner = std::make_shared<mtc::solvers::CartesianPath>();
    cartesian_planner->setMaxVelocityScalingFactor(1.0);
    cartesian_planner->setMaxAccelerationScalingFactor(1.0);
    cartesian_planner->setStepSize(.01);

    auto stage_open_hand =
        std::make_unique<mtc::stages::MoveTo>("open hand", interpolation_planner);
    stage_open_hand->setGroup(hand_group_name);
    stage_open_hand->setGoal("open");
    task.add(std::move(stage_open_hand));

    return task; 
}

void CrackleMoveitTaskConstructorNode::doTask(std::shared_ptr<rclcpp::Node> node) {
    task_ = createTask();

    try
    {
        task_.init();
    }
    catch (mtc::InitStageException& e)
    {
        RCLCPP_ERROR(node->get_logger(), "Initialization failed: %s", e.what());
        return;
    }

    if (!task_.plan(5))
    {
        RCLCPP_ERROR(node->get_logger(), "Planning failed");
        return;
    }

    task_.introspection().publishSolution(*task_.solutions().front());

    auto result = task_.execute(*task_.solutions().front());
    if (result.val != moveit_msgs::msg::MoveItErrorCodes::SUCCESS)
    {
        return;
    }

    return;
}

void exit_sig_handler(int signum)
{
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Received signal %d", signum);
    rclcpp::shutdown();
}

void goToSamplePose(std::shared_ptr<rclcpp::Node> node) {
    auto move_group_interface = MoveGroupInterface(node, "lite6");
    
    RCLCPP_INFO(node->get_logger(), "namespace: %s, group_name: %s", node->get_namespace(), "lite6");
    auto const target_pose = []{
        geometry_msgs::msg::Pose pose;
        pose.orientation.w = 1.0;
        pose.position.x = 0.28;
        pose.position.y = -0.2;
        pose.position.z = 0.5;
        return pose;
    }();

    move_group_interface.setPoseTarget(target_pose);
    
    auto const [success, plan] = [&move_group_interface]{
        moveit::planning_interface::MoveGroupInterface::Plan msg;
        auto const ok = static_cast<bool>(move_group_interface.plan(msg));
        return std::make_pair(ok, msg);
    }();

    if (success) {
        move_group_interface.execute(plan);
    } else {
        RCLCPP_ERROR(node->get_logger(), "Failed to plan");
    }

}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::NodeOptions node_options;
    node_options.automatically_declare_parameters_from_overrides(true);
    std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("xarm_planner_move_group_node", node_options);
    RCLCPP_INFO(node->get_logger(), "crackle_moveit_task_constructor_node start");
    auto const logger = node->get_logger();
    signal(SIGINT, exit_sig_handler);

    std::shared_ptr<CrackleMoveitTaskConstructorNode> crackle_moveit_task_constructor_node = std::make_shared<CrackleMoveitTaskConstructorNode>(node, "lite6");

    crackle_moveit_task_constructor_node->doTask(node);    

    rclcpp::shutdown();
    return 0;
}