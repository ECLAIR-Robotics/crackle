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
#include <crackle_interfaces/srv/place_object.hpp>
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
#include <std_srvs/srv/set_bool.hpp>
#include <visualization_msgs/msg/marker.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
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
#include <rmw/qos_profiles.h>
#include <tf2/LinearMath/Quaternion.h>
#include <xarm_msgs/srv/plan_pose.hpp>
#include <xarm_msgs/srv/plan_joint.hpp>
#include <xarm_msgs/srv/plan_exec.hpp>
#include <xarm_msgs/srv/plan_single_straight.hpp>
#include <tf2_eigen/tf2_eigen.hpp>
#include <Eigen/Geometry>

// MoveIt Task Constructor (MTC) – used by the pickup pipeline.
#include <moveit/task_constructor/task.h>
#include <moveit/task_constructor/solvers/pipeline_planner.h>
#include <moveit/task_constructor/solvers/cartesian_path.h>

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
    // MTC-based pickup: plans all grasp candidates in a single planning tree
    // (IK + collision prune bad grasps cheaply, RRTConnect connects the best
    // reachable grasp) and executes the lowest-cost solution. Replaces the
    // legacy per-candidate loop in pick_up_object.
    bool pick_up_object_mtc(crackle_interfaces::srv::PickupObject::Request::SharedPtr request,
                            crackle_interfaces::srv::PickupObject::Response::SharedPtr response);
    // MTC-based place: mirrors pick_up_object_mtc. Plans all free table spots in
    // a single planning tree and executes the lowest-cost solution.
    bool place_object_mtc(crackle_interfaces::srv::PlaceObject::Request::SharedPtr request,
                          crackle_interfaces::srv::PlaceObject::Response::SharedPtr response);
    bool place_object(crackle_interfaces::srv::PlaceObject::Request::SharedPtr request,
                      crackle_interfaces::srv::PlaceObject::Response::SharedPtr response);
    bool look_at(crackle_interfaces::srv::LookAt::Request::SharedPtr request,
                 crackle_interfaces::srv::LookAt::Response::SharedPtr response);
    std::vector<std::vector<float>> cuboid_handler(std::vector<geometry_msgs::msg::Point> verts);
    // void cuboid_handler(std::vector<geometry_msgs::msg::Point> verts, std::vector<geometry_msgs::msg::Point> basisVecs);

    rclcpp::Logger &getLogger() { return logger_; }
    rclcpp::Node::SharedPtr node_;
    geometry_msgs::msg::Quaternion lookAtQuat(
        const Eigen::Vector3d &to_dir_world,
        const Eigen::Vector3d &world_up,
        const Eigen::Vector3d &tool_forward_in_tool);
    geometry_msgs::msg::Pose construct_reach_pose(geometry_msgs::msg::Pose object_pose, Vector3 tool_offset);
    // `face_ids_out`, if non-null, is filled with a parallel array giving the
    // face index each returned pose belongs to, so callers can group by face
    // and short-circuit planning once one candidate per face succeeds.
    std::vector<geometry_msgs::msg::Pose> get_grasp_poses(
        moveit_msgs::msg::CollisionObject object, double approach_dist,
        double tool_width, std::vector<int> *face_ids_out = nullptr);
    bool dance_dance(std_srvs::srv::Trigger::Request::SharedPtr request,
                     std_srvs::srv::Trigger::Response::SharedPtr response);

    // Command the gripper and block until the firmware reports completion.
    // close=true closes, false opens. Returns true if the gripper confirmed the
    // requested state within timeout_s; on service unavailability falls back to
    // the legacy topic + a fixed wait. Must not be called from the executor
    // thread that also serves the gripper client's callback group.
    bool set_gripper_blocking(bool close, double timeout_s = 15.0);

private:
    void initialize(const std::string &group_name);
    bool wait_for_current_state(const std::string &caller, double timeout_sec = 5.0);

    // Pickup pipeline helpers
    struct PickPhases
    {
        moveit::planning_interface::MoveGroupInterface::Plan pregrasp_plan;
        moveit_msgs::msg::RobotTrajectory approach_traj;
        moveit_msgs::msg::RobotTrajectory lift_traj;
        double score; // total joint displacement – lower is safer
    };
    // Sum of absolute joint-angle deltas across all joints and waypoints.
    double score_trajectory_displacement(const moveit_msgs::msg::RobotTrajectory &traj);
    // Plan pregrasp (joint-space) → approach (Cartesian) → lift (Cartesian)
    // with properly chained start states. Returns false if any phase fails.
    bool plan_pickup_phases(const geometry_msgs::msg::Pose &pregrasp,
                            const geometry_msgs::msg::Pose &grasp,
                            const geometry_msgs::msg::Pose &lift,
                            PickPhases &out);

    // --- MTC pick/place helpers ---------------------------------------------
    // Build an MTC pick or place task for `object_id` over the supplied target
    // end-effector poses (in the planning frame). Structure:
    //   CurrentState -> Connect(RRTConnect)
    //                -> [approach (Cartesian) | target-IK | attach/detach | retreat (Cartesian)]
    // `attach == true` attaches the object (pick); `attach == false` detaches it
    // (place). `approach_move` / `retreat_move` are the Cartesian leg distances.
    bool build_manipulation_task(const std::string &object_id,
                                 const std::vector<geometry_msgs::msg::Pose> &target_poses,
                                 double approach_move, double retreat_move, bool attach,
                                 moveit::task_constructor::Task &task,
                                 const std::string &support_surface = "");
    // Execute a planned MTC pick/place solution. Between the approach and the
    // retreat it actuates the gripper and (de)attaches `obj` in the real
    // planning scene: pick closes+attaches, place opens+detaches. Returns false
    // (and sets `err`) on any execution failure.
    bool execute_manipulation_solution(const moveit::task_constructor::SolutionBase &solution,
                                       const moveit_msgs::msg::CollisionObject &obj,
                                       bool is_pick, std::string &err);
    // Lazily-constructed MTC solvers (need node_ / robot model).
    std::shared_ptr<moveit::task_constructor::solvers::PipelinePlanner> mtc_sampling_planner_;
    std::shared_ptr<moveit::task_constructor::solvers::CartesianPath> mtc_cartesian_planner_;
    // Sample free placement spots on a named table collision object.
    // Filters out locations that would overlap any other scene object.
    std::vector<geometry_msgs::msg::Pose>
    find_place_poses_on_table(const std::string &object_name,
                              const std::string &table_name);

    // Grasp-candidate visualization.
    // status codes: 0 = proposed (cyan), 1 = planning (yellow),
    //               2 = feasible (green), 3 = infeasible (red),
    //               4 = selected (magenta, enlarged),
    //               5 = skipped (gray)
    void publish_grasp_markers(const std::vector<geometry_msgs::msg::Pose> &grasps,
                               const std::vector<int> &statuses);
    void clear_grasp_markers();

    rclcpp::Logger logger_;
    rclcpp::Service<crackle_interfaces::srv::PickupObject>::SharedPtr pickup_service_;
    rclcpp::Service<crackle_interfaces::srv::PlaceObject>::SharedPtr place_service_;
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
    // Blocking gripper interface: waits until the claw firmware reports the
    // command finished. Uses its own (reentrant) callback group so the response
    // can be processed on a second executor thread while a service handler
    // (e.g. pick_up_object) blocks on the result.
    rclcpp::Client<std_srvs::srv::SetBool>::SharedPtr gripper_client_;
    rclcpp::CallbackGroup::SharedPtr gripper_cb_group_;
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_publisher_;
    rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr grasp_markers_publisher_;
    rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_state_sub_;
    rclcpp::Time last_joint_state_stamp_;
    bool have_joint_state_;
    std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_;
    std::shared_ptr<moveit::planning_interface::PlanningSceneInterface> planning_scene_;
    moveit::planning_interface::MoveGroupInterface::Plan plan_;
    moveit_msgs::msg::RobotTrajectory trajectory_;
    bool is_trajectory_;

    // Whether the arm is currently holding an object, and which one. Set on a
    // successful MTC pick and cleared on a successful MTC place. place_object
    // uses this instead of requiring the object name in the request.
    bool holding_object_ = false;
    moveit_msgs::msg::CollisionObject held_object_;
};

#endif // __CRACKLE_PLANNER_H__