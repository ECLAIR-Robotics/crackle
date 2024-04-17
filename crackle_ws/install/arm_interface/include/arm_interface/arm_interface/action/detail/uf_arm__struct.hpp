// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from arm_interface:action/UfArm.idl
// generated code does not contain a copyright notice

#ifndef ARM_INTERFACE__ACTION__DETAIL__UF_ARM__STRUCT_HPP_
#define ARM_INTERFACE__ACTION__DETAIL__UF_ARM__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'goal_pose'
#include "geometry_msgs/msg/detail/pose__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__arm_interface__action__UfArm_Goal __attribute__((deprecated))
#else
# define DEPRECATED__arm_interface__action__UfArm_Goal __declspec(deprecated)
#endif

namespace arm_interface
{

namespace action
{

// message struct
template<class ContainerAllocator>
struct UfArm_Goal_
{
  using Type = UfArm_Goal_<ContainerAllocator>;

  explicit UfArm_Goal_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : goal_pose(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->pose = "";
    }
  }

  explicit UfArm_Goal_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : goal_pose(_alloc, _init),
    pose(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->pose = "";
    }
  }

  // field types and members
  using _goal_pose_type =
    geometry_msgs::msg::Pose_<ContainerAllocator>;
  _goal_pose_type goal_pose;
  using _pose_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _pose_type pose;

  // setters for named parameter idiom
  Type & set__goal_pose(
    const geometry_msgs::msg::Pose_<ContainerAllocator> & _arg)
  {
    this->goal_pose = _arg;
    return *this;
  }
  Type & set__pose(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->pose = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    arm_interface::action::UfArm_Goal_<ContainerAllocator> *;
  using ConstRawPtr =
    const arm_interface::action::UfArm_Goal_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<arm_interface::action::UfArm_Goal_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<arm_interface::action::UfArm_Goal_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      arm_interface::action::UfArm_Goal_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<arm_interface::action::UfArm_Goal_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      arm_interface::action::UfArm_Goal_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<arm_interface::action::UfArm_Goal_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<arm_interface::action::UfArm_Goal_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<arm_interface::action::UfArm_Goal_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__arm_interface__action__UfArm_Goal
    std::shared_ptr<arm_interface::action::UfArm_Goal_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__arm_interface__action__UfArm_Goal
    std::shared_ptr<arm_interface::action::UfArm_Goal_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const UfArm_Goal_ & other) const
  {
    if (this->goal_pose != other.goal_pose) {
      return false;
    }
    if (this->pose != other.pose) {
      return false;
    }
    return true;
  }
  bool operator!=(const UfArm_Goal_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct UfArm_Goal_

// alias to use template instance with default allocator
using UfArm_Goal =
  arm_interface::action::UfArm_Goal_<std::allocator<void>>;

// constant definitions

}  // namespace action

}  // namespace arm_interface


// Include directives for member types
// Member 'final_pose'
// already included above
// #include "geometry_msgs/msg/detail/pose__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__arm_interface__action__UfArm_Result __attribute__((deprecated))
#else
# define DEPRECATED__arm_interface__action__UfArm_Result __declspec(deprecated)
#endif

namespace arm_interface
{

namespace action
{

// message struct
template<class ContainerAllocator>
struct UfArm_Result_
{
  using Type = UfArm_Result_<ContainerAllocator>;

  explicit UfArm_Result_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : final_pose(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
      this->message = "";
    }
  }

  explicit UfArm_Result_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : final_pose(_alloc, _init),
    message(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
      this->message = "";
    }
  }

  // field types and members
  using _final_pose_type =
    geometry_msgs::msg::Pose_<ContainerAllocator>;
  _final_pose_type final_pose;
  using _success_type =
    bool;
  _success_type success;
  using _message_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _message_type message;

  // setters for named parameter idiom
  Type & set__final_pose(
    const geometry_msgs::msg::Pose_<ContainerAllocator> & _arg)
  {
    this->final_pose = _arg;
    return *this;
  }
  Type & set__success(
    const bool & _arg)
  {
    this->success = _arg;
    return *this;
  }
  Type & set__message(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->message = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    arm_interface::action::UfArm_Result_<ContainerAllocator> *;
  using ConstRawPtr =
    const arm_interface::action::UfArm_Result_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<arm_interface::action::UfArm_Result_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<arm_interface::action::UfArm_Result_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      arm_interface::action::UfArm_Result_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<arm_interface::action::UfArm_Result_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      arm_interface::action::UfArm_Result_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<arm_interface::action::UfArm_Result_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<arm_interface::action::UfArm_Result_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<arm_interface::action::UfArm_Result_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__arm_interface__action__UfArm_Result
    std::shared_ptr<arm_interface::action::UfArm_Result_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__arm_interface__action__UfArm_Result
    std::shared_ptr<arm_interface::action::UfArm_Result_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const UfArm_Result_ & other) const
  {
    if (this->final_pose != other.final_pose) {
      return false;
    }
    if (this->success != other.success) {
      return false;
    }
    if (this->message != other.message) {
      return false;
    }
    return true;
  }
  bool operator!=(const UfArm_Result_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct UfArm_Result_

// alias to use template instance with default allocator
using UfArm_Result =
  arm_interface::action::UfArm_Result_<std::allocator<void>>;

// constant definitions

}  // namespace action

}  // namespace arm_interface


// Include directives for member types
// Member 'current_pose'
// already included above
// #include "geometry_msgs/msg/detail/pose__struct.hpp"
// Member 'status'
#include "actionlib_msgs/msg/detail/goal_status__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__arm_interface__action__UfArm_Feedback __attribute__((deprecated))
#else
# define DEPRECATED__arm_interface__action__UfArm_Feedback __declspec(deprecated)
#endif

namespace arm_interface
{

namespace action
{

// message struct
template<class ContainerAllocator>
struct UfArm_Feedback_
{
  using Type = UfArm_Feedback_<ContainerAllocator>;

  explicit UfArm_Feedback_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : current_pose(_init),
    status(_init)
  {
    (void)_init;
  }

  explicit UfArm_Feedback_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : current_pose(_alloc, _init),
    status(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _current_pose_type =
    geometry_msgs::msg::Pose_<ContainerAllocator>;
  _current_pose_type current_pose;
  using _status_type =
    actionlib_msgs::msg::GoalStatus_<ContainerAllocator>;
  _status_type status;

  // setters for named parameter idiom
  Type & set__current_pose(
    const geometry_msgs::msg::Pose_<ContainerAllocator> & _arg)
  {
    this->current_pose = _arg;
    return *this;
  }
  Type & set__status(
    const actionlib_msgs::msg::GoalStatus_<ContainerAllocator> & _arg)
  {
    this->status = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    arm_interface::action::UfArm_Feedback_<ContainerAllocator> *;
  using ConstRawPtr =
    const arm_interface::action::UfArm_Feedback_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<arm_interface::action::UfArm_Feedback_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<arm_interface::action::UfArm_Feedback_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      arm_interface::action::UfArm_Feedback_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<arm_interface::action::UfArm_Feedback_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      arm_interface::action::UfArm_Feedback_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<arm_interface::action::UfArm_Feedback_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<arm_interface::action::UfArm_Feedback_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<arm_interface::action::UfArm_Feedback_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__arm_interface__action__UfArm_Feedback
    std::shared_ptr<arm_interface::action::UfArm_Feedback_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__arm_interface__action__UfArm_Feedback
    std::shared_ptr<arm_interface::action::UfArm_Feedback_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const UfArm_Feedback_ & other) const
  {
    if (this->current_pose != other.current_pose) {
      return false;
    }
    if (this->status != other.status) {
      return false;
    }
    return true;
  }
  bool operator!=(const UfArm_Feedback_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct UfArm_Feedback_

// alias to use template instance with default allocator
using UfArm_Feedback =
  arm_interface::action::UfArm_Feedback_<std::allocator<void>>;

// constant definitions

}  // namespace action

}  // namespace arm_interface


// Include directives for member types
// Member 'goal_id'
#include "unique_identifier_msgs/msg/detail/uuid__struct.hpp"
// Member 'goal'
#include "arm_interface/action/detail/uf_arm__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__arm_interface__action__UfArm_SendGoal_Request __attribute__((deprecated))
#else
# define DEPRECATED__arm_interface__action__UfArm_SendGoal_Request __declspec(deprecated)
#endif

namespace arm_interface
{

namespace action
{

// message struct
template<class ContainerAllocator>
struct UfArm_SendGoal_Request_
{
  using Type = UfArm_SendGoal_Request_<ContainerAllocator>;

  explicit UfArm_SendGoal_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : goal_id(_init),
    goal(_init)
  {
    (void)_init;
  }

  explicit UfArm_SendGoal_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : goal_id(_alloc, _init),
    goal(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _goal_id_type =
    unique_identifier_msgs::msg::UUID_<ContainerAllocator>;
  _goal_id_type goal_id;
  using _goal_type =
    arm_interface::action::UfArm_Goal_<ContainerAllocator>;
  _goal_type goal;

  // setters for named parameter idiom
  Type & set__goal_id(
    const unique_identifier_msgs::msg::UUID_<ContainerAllocator> & _arg)
  {
    this->goal_id = _arg;
    return *this;
  }
  Type & set__goal(
    const arm_interface::action::UfArm_Goal_<ContainerAllocator> & _arg)
  {
    this->goal = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    arm_interface::action::UfArm_SendGoal_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const arm_interface::action::UfArm_SendGoal_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<arm_interface::action::UfArm_SendGoal_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<arm_interface::action::UfArm_SendGoal_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      arm_interface::action::UfArm_SendGoal_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<arm_interface::action::UfArm_SendGoal_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      arm_interface::action::UfArm_SendGoal_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<arm_interface::action::UfArm_SendGoal_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<arm_interface::action::UfArm_SendGoal_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<arm_interface::action::UfArm_SendGoal_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__arm_interface__action__UfArm_SendGoal_Request
    std::shared_ptr<arm_interface::action::UfArm_SendGoal_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__arm_interface__action__UfArm_SendGoal_Request
    std::shared_ptr<arm_interface::action::UfArm_SendGoal_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const UfArm_SendGoal_Request_ & other) const
  {
    if (this->goal_id != other.goal_id) {
      return false;
    }
    if (this->goal != other.goal) {
      return false;
    }
    return true;
  }
  bool operator!=(const UfArm_SendGoal_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct UfArm_SendGoal_Request_

// alias to use template instance with default allocator
using UfArm_SendGoal_Request =
  arm_interface::action::UfArm_SendGoal_Request_<std::allocator<void>>;

// constant definitions

}  // namespace action

}  // namespace arm_interface


// Include directives for member types
// Member 'stamp'
#include "builtin_interfaces/msg/detail/time__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__arm_interface__action__UfArm_SendGoal_Response __attribute__((deprecated))
#else
# define DEPRECATED__arm_interface__action__UfArm_SendGoal_Response __declspec(deprecated)
#endif

namespace arm_interface
{

namespace action
{

// message struct
template<class ContainerAllocator>
struct UfArm_SendGoal_Response_
{
  using Type = UfArm_SendGoal_Response_<ContainerAllocator>;

  explicit UfArm_SendGoal_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : stamp(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->accepted = false;
    }
  }

  explicit UfArm_SendGoal_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : stamp(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->accepted = false;
    }
  }

  // field types and members
  using _accepted_type =
    bool;
  _accepted_type accepted;
  using _stamp_type =
    builtin_interfaces::msg::Time_<ContainerAllocator>;
  _stamp_type stamp;

  // setters for named parameter idiom
  Type & set__accepted(
    const bool & _arg)
  {
    this->accepted = _arg;
    return *this;
  }
  Type & set__stamp(
    const builtin_interfaces::msg::Time_<ContainerAllocator> & _arg)
  {
    this->stamp = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    arm_interface::action::UfArm_SendGoal_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const arm_interface::action::UfArm_SendGoal_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<arm_interface::action::UfArm_SendGoal_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<arm_interface::action::UfArm_SendGoal_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      arm_interface::action::UfArm_SendGoal_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<arm_interface::action::UfArm_SendGoal_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      arm_interface::action::UfArm_SendGoal_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<arm_interface::action::UfArm_SendGoal_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<arm_interface::action::UfArm_SendGoal_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<arm_interface::action::UfArm_SendGoal_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__arm_interface__action__UfArm_SendGoal_Response
    std::shared_ptr<arm_interface::action::UfArm_SendGoal_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__arm_interface__action__UfArm_SendGoal_Response
    std::shared_ptr<arm_interface::action::UfArm_SendGoal_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const UfArm_SendGoal_Response_ & other) const
  {
    if (this->accepted != other.accepted) {
      return false;
    }
    if (this->stamp != other.stamp) {
      return false;
    }
    return true;
  }
  bool operator!=(const UfArm_SendGoal_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct UfArm_SendGoal_Response_

// alias to use template instance with default allocator
using UfArm_SendGoal_Response =
  arm_interface::action::UfArm_SendGoal_Response_<std::allocator<void>>;

// constant definitions

}  // namespace action

}  // namespace arm_interface

namespace arm_interface
{

namespace action
{

struct UfArm_SendGoal
{
  using Request = arm_interface::action::UfArm_SendGoal_Request;
  using Response = arm_interface::action::UfArm_SendGoal_Response;
};

}  // namespace action

}  // namespace arm_interface


// Include directives for member types
// Member 'goal_id'
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__arm_interface__action__UfArm_GetResult_Request __attribute__((deprecated))
#else
# define DEPRECATED__arm_interface__action__UfArm_GetResult_Request __declspec(deprecated)
#endif

namespace arm_interface
{

namespace action
{

// message struct
template<class ContainerAllocator>
struct UfArm_GetResult_Request_
{
  using Type = UfArm_GetResult_Request_<ContainerAllocator>;

  explicit UfArm_GetResult_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : goal_id(_init)
  {
    (void)_init;
  }

  explicit UfArm_GetResult_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : goal_id(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _goal_id_type =
    unique_identifier_msgs::msg::UUID_<ContainerAllocator>;
  _goal_id_type goal_id;

  // setters for named parameter idiom
  Type & set__goal_id(
    const unique_identifier_msgs::msg::UUID_<ContainerAllocator> & _arg)
  {
    this->goal_id = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    arm_interface::action::UfArm_GetResult_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const arm_interface::action::UfArm_GetResult_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<arm_interface::action::UfArm_GetResult_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<arm_interface::action::UfArm_GetResult_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      arm_interface::action::UfArm_GetResult_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<arm_interface::action::UfArm_GetResult_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      arm_interface::action::UfArm_GetResult_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<arm_interface::action::UfArm_GetResult_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<arm_interface::action::UfArm_GetResult_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<arm_interface::action::UfArm_GetResult_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__arm_interface__action__UfArm_GetResult_Request
    std::shared_ptr<arm_interface::action::UfArm_GetResult_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__arm_interface__action__UfArm_GetResult_Request
    std::shared_ptr<arm_interface::action::UfArm_GetResult_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const UfArm_GetResult_Request_ & other) const
  {
    if (this->goal_id != other.goal_id) {
      return false;
    }
    return true;
  }
  bool operator!=(const UfArm_GetResult_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct UfArm_GetResult_Request_

// alias to use template instance with default allocator
using UfArm_GetResult_Request =
  arm_interface::action::UfArm_GetResult_Request_<std::allocator<void>>;

// constant definitions

}  // namespace action

}  // namespace arm_interface


// Include directives for member types
// Member 'result'
// already included above
// #include "arm_interface/action/detail/uf_arm__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__arm_interface__action__UfArm_GetResult_Response __attribute__((deprecated))
#else
# define DEPRECATED__arm_interface__action__UfArm_GetResult_Response __declspec(deprecated)
#endif

namespace arm_interface
{

namespace action
{

// message struct
template<class ContainerAllocator>
struct UfArm_GetResult_Response_
{
  using Type = UfArm_GetResult_Response_<ContainerAllocator>;

  explicit UfArm_GetResult_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : result(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->status = 0;
    }
  }

  explicit UfArm_GetResult_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : result(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->status = 0;
    }
  }

  // field types and members
  using _status_type =
    int8_t;
  _status_type status;
  using _result_type =
    arm_interface::action::UfArm_Result_<ContainerAllocator>;
  _result_type result;

  // setters for named parameter idiom
  Type & set__status(
    const int8_t & _arg)
  {
    this->status = _arg;
    return *this;
  }
  Type & set__result(
    const arm_interface::action::UfArm_Result_<ContainerAllocator> & _arg)
  {
    this->result = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    arm_interface::action::UfArm_GetResult_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const arm_interface::action::UfArm_GetResult_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<arm_interface::action::UfArm_GetResult_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<arm_interface::action::UfArm_GetResult_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      arm_interface::action::UfArm_GetResult_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<arm_interface::action::UfArm_GetResult_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      arm_interface::action::UfArm_GetResult_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<arm_interface::action::UfArm_GetResult_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<arm_interface::action::UfArm_GetResult_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<arm_interface::action::UfArm_GetResult_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__arm_interface__action__UfArm_GetResult_Response
    std::shared_ptr<arm_interface::action::UfArm_GetResult_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__arm_interface__action__UfArm_GetResult_Response
    std::shared_ptr<arm_interface::action::UfArm_GetResult_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const UfArm_GetResult_Response_ & other) const
  {
    if (this->status != other.status) {
      return false;
    }
    if (this->result != other.result) {
      return false;
    }
    return true;
  }
  bool operator!=(const UfArm_GetResult_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct UfArm_GetResult_Response_

// alias to use template instance with default allocator
using UfArm_GetResult_Response =
  arm_interface::action::UfArm_GetResult_Response_<std::allocator<void>>;

// constant definitions

}  // namespace action

}  // namespace arm_interface

namespace arm_interface
{

namespace action
{

struct UfArm_GetResult
{
  using Request = arm_interface::action::UfArm_GetResult_Request;
  using Response = arm_interface::action::UfArm_GetResult_Response;
};

}  // namespace action

}  // namespace arm_interface


// Include directives for member types
// Member 'goal_id'
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__struct.hpp"
// Member 'feedback'
// already included above
// #include "arm_interface/action/detail/uf_arm__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__arm_interface__action__UfArm_FeedbackMessage __attribute__((deprecated))
#else
# define DEPRECATED__arm_interface__action__UfArm_FeedbackMessage __declspec(deprecated)
#endif

namespace arm_interface
{

namespace action
{

// message struct
template<class ContainerAllocator>
struct UfArm_FeedbackMessage_
{
  using Type = UfArm_FeedbackMessage_<ContainerAllocator>;

  explicit UfArm_FeedbackMessage_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : goal_id(_init),
    feedback(_init)
  {
    (void)_init;
  }

  explicit UfArm_FeedbackMessage_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : goal_id(_alloc, _init),
    feedback(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _goal_id_type =
    unique_identifier_msgs::msg::UUID_<ContainerAllocator>;
  _goal_id_type goal_id;
  using _feedback_type =
    arm_interface::action::UfArm_Feedback_<ContainerAllocator>;
  _feedback_type feedback;

  // setters for named parameter idiom
  Type & set__goal_id(
    const unique_identifier_msgs::msg::UUID_<ContainerAllocator> & _arg)
  {
    this->goal_id = _arg;
    return *this;
  }
  Type & set__feedback(
    const arm_interface::action::UfArm_Feedback_<ContainerAllocator> & _arg)
  {
    this->feedback = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    arm_interface::action::UfArm_FeedbackMessage_<ContainerAllocator> *;
  using ConstRawPtr =
    const arm_interface::action::UfArm_FeedbackMessage_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<arm_interface::action::UfArm_FeedbackMessage_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<arm_interface::action::UfArm_FeedbackMessage_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      arm_interface::action::UfArm_FeedbackMessage_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<arm_interface::action::UfArm_FeedbackMessage_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      arm_interface::action::UfArm_FeedbackMessage_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<arm_interface::action::UfArm_FeedbackMessage_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<arm_interface::action::UfArm_FeedbackMessage_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<arm_interface::action::UfArm_FeedbackMessage_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__arm_interface__action__UfArm_FeedbackMessage
    std::shared_ptr<arm_interface::action::UfArm_FeedbackMessage_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__arm_interface__action__UfArm_FeedbackMessage
    std::shared_ptr<arm_interface::action::UfArm_FeedbackMessage_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const UfArm_FeedbackMessage_ & other) const
  {
    if (this->goal_id != other.goal_id) {
      return false;
    }
    if (this->feedback != other.feedback) {
      return false;
    }
    return true;
  }
  bool operator!=(const UfArm_FeedbackMessage_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct UfArm_FeedbackMessage_

// alias to use template instance with default allocator
using UfArm_FeedbackMessage =
  arm_interface::action::UfArm_FeedbackMessage_<std::allocator<void>>;

// constant definitions

}  // namespace action

}  // namespace arm_interface

#include "action_msgs/srv/cancel_goal.hpp"
#include "action_msgs/msg/goal_info.hpp"
#include "action_msgs/msg/goal_status_array.hpp"

namespace arm_interface
{

namespace action
{

struct UfArm
{
  /// The goal message defined in the action definition.
  using Goal = arm_interface::action::UfArm_Goal;
  /// The result message defined in the action definition.
  using Result = arm_interface::action::UfArm_Result;
  /// The feedback message defined in the action definition.
  using Feedback = arm_interface::action::UfArm_Feedback;

  struct Impl
  {
    /// The send_goal service using a wrapped version of the goal message as a request.
    using SendGoalService = arm_interface::action::UfArm_SendGoal;
    /// The get_result service using a wrapped version of the result message as a response.
    using GetResultService = arm_interface::action::UfArm_GetResult;
    /// The feedback message with generic fields which wraps the feedback message.
    using FeedbackMessage = arm_interface::action::UfArm_FeedbackMessage;

    /// The generic service to cancel a goal.
    using CancelGoalService = action_msgs::srv::CancelGoal;
    /// The generic message for the status of a goal.
    using GoalStatusMessage = action_msgs::msg::GoalStatusArray;
  };
};

typedef struct UfArm UfArm;

}  // namespace action

}  // namespace arm_interface

#endif  // ARM_INTERFACE__ACTION__DETAIL__UF_ARM__STRUCT_HPP_
