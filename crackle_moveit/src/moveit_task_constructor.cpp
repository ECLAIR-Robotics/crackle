#include <crackle_moveit/moveit_task_constructor.hpp>
#include "example_interfaces/srv/add_two_ints.hpp"
#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

namespace mtc = moveit::task_constructor;

using namespace std::chrono_literals;

using moveit::planning_interface::MoveGroupInterface;
class MinimalPublisher : public rclcpp::Node
{
  public:
    MinimalPublisher()
    : Node("minimal_publisher"), count_(0)
    {
      publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
      timer_ = this->create_wall_timer(
      500ms, std::bind(&MinimalPublisher::timer_callback, this));
      init("lite6");
    }

    void pickupCallback(const std::shared_ptr<crackle_interfaces::srv::PickupObject::Request> request,
                        std::shared_ptr<crackle_interfaces::srv::PickupObject::Response> response)
    {
      RCLCPP_INFO(this->get_logger(), "Pickup object: %s", request->object_name.c_str());
       
      response->success = true;
    }

    void init(const std::string& group_name)
    {
      const rclcpp::Node::SharedPtr node = this->shared_from_this();
      // Initialize the MoveGroupInterface
      move_group_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(node, group_name);
      planning_scene_ = std::make_shared<moveit::planning_interface::PlanningSceneInterface>();
      
      // Create a service for the pickup object
      pickup_service_ = this->create_service<crackle_interfaces::srv::PickupObject>(
        "pickup_object", std::bind(&MinimalPublisher::pickupCallback, this, _1, _2));
    }

  private:
    void timer_callback()
    {
      auto message = std_msgs::msg::String();
      message.data = "Hello, world! " + std::to_string(count_++);
      RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
      publisher_->publish(message);
    }

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::Service<crackle_interfaces::srv::PickupObject>::SharedPtr pickup_service_;

    std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_;
    std::shared_ptr<moveit::planning_interface::PlanningSceneInterface> planning_scene_;

    moveit_msgs::msg::RobotTrajectory trajectory_;

    size_t count_;
};

void add(const std::shared_ptr<example_interfaces::srv::AddTwoInts::Request> request,
          std::shared_ptr<example_interfaces::srv::AddTwoInts::Response>      response)
{
  response->sum = request->a + request->b;
  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Incoming request\na: %ld" " b: %ld",
                request->a, request->b);
  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "sending back response: [%ld]", (long int)response->sum);

}


int main(int argc, char** argv)
{
    // Initialize ROS and create the Node
  rclcpp::init(argc, argv);
  auto const node = std::make_shared<rclcpp::Node>(
    "crackle_moveit_task_constructor_node",
    rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true)
  );

  auto minimal_publisher = std::make_shared<MinimalPublisher>();
  auto logger = rclcpp::get_logger("crackle_moveit_task_constructor_node");
  RCLCPP_INFO(logger, "crackle_moveit_task_constructor_node start");



  rclcpp::shutdown();
  return 0;
  
  

  // Create a ROS logger
  RCLCPP_INFO(logger, "crackle_moveit_task_constructor_node start");

  // Create the MoveIt MoveGroup Interface
  using moveit::planning_interface::MoveGroupInterface;
  auto move_group_interface = MoveGroupInterface(node, "lite6");


  rclcpp::Service<example_interfaces::srv::AddTwoInts>::SharedPtr service =
    node->create_service<example_interfaces::srv::AddTwoInts>("add_two_ints", &add);

  
  // Set a target Pose
  auto const target_pose = []{
    geometry_msgs::msg::Pose msg;
    msg.orientation.w = 1.0;
    msg.position.x = 0.28;
    msg.position.y = -0.2;
    msg.position.z = 0.5;
    return msg;
  }();

  move_group_interface.setPoseTarget(target_pose);

  // Create a plan to that target pose
  auto const [success, plan] = [&move_group_interface]{
    moveit::planning_interface::MoveGroupInterface::Plan msg;
    auto const ok = static_cast<bool>(move_group_interface.plan(msg));
    return std::make_pair(ok, msg);
  }();

  // Execute the plan
  if(success) {
    move_group_interface.execute(plan);
    RCLCPP_INFO(logger, "Planning succeeded!");
  } else {
    RCLCPP_ERROR(logger, "Planning failed!");
  }

  mtc::Task task;
  task.stages()->setName("xarm_planner_task");
  task.setRobotModel(move_group_interface.getRobotModel());

  const auto& arm_group_name = "lite6";
  
  const auto& hand_frame = "gripper_base";

  // Set task properties
  task.setProperty("group", arm_group_name);
  task.setProperty("ik_frame", hand_frame);

// Disable warnings for this line, as it's a variable that's set but not used in this example
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    mtc::Stage* current_state_ptr = nullptr;  // Forward current_state on to grasp pose generator
  #pragma GCC diagnostic pop

  auto stage_state_current = std::make_unique<mtc::stages::CurrentState>("current");
  current_state_ptr = stage_state_current.get();
  task.add(std::move(stage_state_current));

  auto sampling_planner = std::make_shared<mtc::solvers::PipelinePlanner>(node);
  auto interpolation_planner = std::make_shared<mtc::solvers::JointInterpolationPlanner>();


  auto cartesian_planner = std::make_shared<mtc::solvers::CartesianPath>();
  cartesian_planner->setMaxVelocityScalingFactor(1.0);
  cartesian_planner->setMaxAccelerationScalingFactor(1.0);
  cartesian_planner->setStepSize(.01);

  const geometry_msgs::msg::PoseStamped target_pose_2 = []{
    geometry_msgs::msg::PoseStamped msg;
    msg.header.frame_id = "gripper_base";
    msg.pose.orientation.w = 1.0;
    msg.pose.position.x = 0.28;
    msg.pose.position.y = -0.2;
    msg.pose.position.z = 0.5;
    return msg;
  }();

  auto move_to = std::make_unique<mtc::stages::MoveTo>("move_to");
  move_to->setGroup(arm_group_name);
  move_to->setGoal(target_pose_2);
  move_to->setProperty("group", arm_group_name);
 
  task.add(std::move(move_to));
  
  
  RCLCPP_INFO(logger, "Task created!");
  task.init();

  auto error_code = task.plan();

  rclcpp::spin(node);

  // Shutdown ROS
  rclcpp::shutdown();
  return 0;
}
