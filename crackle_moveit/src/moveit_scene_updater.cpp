#include <crackle_moveit/moveit_scene_updater.hpp>

CrackleMoveitSceneUpdater::CrackleMoveitSceneUpdater(const rclcpp::Node::SharedPtr& node) : node_(node) {
    init();
}

void CrackleMoveitSceneUpdater::init() {
    static const std::string PLANNING_GROUP = "lite6";
    move_group_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(node_, PLANNING_GROUP);
    planning_scene_ = std::make_shared<moveit::planning_interface::PlanningSceneInterface>();
    planning_scene_diff_publisher_ = node_->create_publisher<moveit_msgs::msg::PlanningScene>("planning_scene", 1);
    collision_object_subscription_ = node_->create_subscription<moveit_msgs::msg::CollisionObject>(
        "crackle_moveit/publish_collision_object", 10, 
        [this](const moveit_msgs::msg::CollisionObject::SharedPtr msg) -> void {
            RCLCPP_INFO(node_->get_logger(), "Received collision object message with id: %s", msg->id.c_str());
            std::vector<moveit_msgs::msg::CollisionObject> collision_objects;
            collision_objects.push_back(*msg);
            planning_scene_->applyCollisionObjects(collision_objects);
            RCLCPP_INFO(node_->get_logger(), "Applied collision object with id: %s to the planning scene", msg->id.c_str());
        }
    );
    while(planning_scene_diff_publisher_->get_subscription_count() < 1) {
        RCLCPP_INFO(node_->get_logger(), "Waiting for subscriber to connect");
        rclcpp::sleep_for(std::chrono::seconds(1));
    }

    is_trajectory_ = false;
}

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::NodeOptions node_options;
    node_options.automatically_declare_parameters_from_overrides(true);
    std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("scene_updater", node_options);
    RCLCPP_INFO(node->get_logger(), "crackle_moveit_scene_updater_node start");

    RCLCPP_INFO(node->get_logger(), "namespace: %s", node->get_namespace());
    std::shared_ptr<CrackleMoveitSceneUpdater> crackle_moveit_scene_updater = std::make_shared<CrackleMoveitSceneUpdater>(node);

    shape_msgs::msg::SolidPrimitive box;
    box.type = box.BOX;
    box.dimensions.resize(3);
    box.dimensions[0] = 1;
    box.dimensions[1] = 1;
    box.dimensions[2] = 0.1;

    moveit_msgs::msg::CollisionObject collision_object;
    collision_object.header.frame_id = "world";
    collision_object.id = "table";
    collision_object.primitives.push_back(box);
    
    geometry_msgs::msg::Pose box_pose;
    box_pose.orientation.w = 1.0;    
    box_pose.position.x = 0.0;
    box_pose.position.y = 0.0;
    box_pose.position.z = -0.05;
    collision_object.primitive_poses.push_back(box_pose);
    collision_object.operation = collision_object.ADD;

    // Adds the table on which the arm is placed as a collision object
    RCLCPP_INFO(node->get_logger(), "Add collision object: table1");
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface; 
    planning_scene_interface.applyCollisionObject(collision_object);

    rclcpp::shutdown();
    return 0;    
}