// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from xarm_msgs:srv/PlanSingleStraight.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__PLAN_SINGLE_STRAIGHT__STRUCT_HPP_
#define XARM_MSGS__SRV__DETAIL__PLAN_SINGLE_STRAIGHT__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'target'
#include "geometry_msgs/msg/detail/pose__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__xarm_msgs__srv__PlanSingleStraight_Request __attribute__((deprecated))
#else
# define DEPRECATED__xarm_msgs__srv__PlanSingleStraight_Request __declspec(deprecated)
#endif

namespace xarm_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct PlanSingleStraight_Request_
{
  using Type = PlanSingleStraight_Request_<ContainerAllocator>;

  explicit PlanSingleStraight_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : target(_init)
  {
    (void)_init;
  }

  explicit PlanSingleStraight_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : target(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _target_type =
    geometry_msgs::msg::Pose_<ContainerAllocator>;
  _target_type target;

  // setters for named parameter idiom
  Type & set__target(
    const geometry_msgs::msg::Pose_<ContainerAllocator> & _arg)
  {
    this->target = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    xarm_msgs::srv::PlanSingleStraight_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const xarm_msgs::srv::PlanSingleStraight_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<xarm_msgs::srv::PlanSingleStraight_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<xarm_msgs::srv::PlanSingleStraight_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::PlanSingleStraight_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::PlanSingleStraight_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::PlanSingleStraight_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::PlanSingleStraight_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<xarm_msgs::srv::PlanSingleStraight_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<xarm_msgs::srv::PlanSingleStraight_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__xarm_msgs__srv__PlanSingleStraight_Request
    std::shared_ptr<xarm_msgs::srv::PlanSingleStraight_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__xarm_msgs__srv__PlanSingleStraight_Request
    std::shared_ptr<xarm_msgs::srv::PlanSingleStraight_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const PlanSingleStraight_Request_ & other) const
  {
    if (this->target != other.target) {
      return false;
    }
    return true;
  }
  bool operator!=(const PlanSingleStraight_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct PlanSingleStraight_Request_

// alias to use template instance with default allocator
using PlanSingleStraight_Request =
  xarm_msgs::srv::PlanSingleStraight_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace xarm_msgs


#ifndef _WIN32
# define DEPRECATED__xarm_msgs__srv__PlanSingleStraight_Response __attribute__((deprecated))
#else
# define DEPRECATED__xarm_msgs__srv__PlanSingleStraight_Response __declspec(deprecated)
#endif

namespace xarm_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct PlanSingleStraight_Response_
{
  using Type = PlanSingleStraight_Response_<ContainerAllocator>;

  explicit PlanSingleStraight_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
    }
  }

  explicit PlanSingleStraight_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
    }
  }

  // field types and members
  using _success_type =
    bool;
  _success_type success;

  // setters for named parameter idiom
  Type & set__success(
    const bool & _arg)
  {
    this->success = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    xarm_msgs::srv::PlanSingleStraight_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const xarm_msgs::srv::PlanSingleStraight_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<xarm_msgs::srv::PlanSingleStraight_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<xarm_msgs::srv::PlanSingleStraight_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::PlanSingleStraight_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::PlanSingleStraight_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::PlanSingleStraight_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::PlanSingleStraight_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<xarm_msgs::srv::PlanSingleStraight_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<xarm_msgs::srv::PlanSingleStraight_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__xarm_msgs__srv__PlanSingleStraight_Response
    std::shared_ptr<xarm_msgs::srv::PlanSingleStraight_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__xarm_msgs__srv__PlanSingleStraight_Response
    std::shared_ptr<xarm_msgs::srv::PlanSingleStraight_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const PlanSingleStraight_Response_ & other) const
  {
    if (this->success != other.success) {
      return false;
    }
    return true;
  }
  bool operator!=(const PlanSingleStraight_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct PlanSingleStraight_Response_

// alias to use template instance with default allocator
using PlanSingleStraight_Response =
  xarm_msgs::srv::PlanSingleStraight_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace xarm_msgs

namespace xarm_msgs
{

namespace srv
{

struct PlanSingleStraight
{
  using Request = xarm_msgs::srv::PlanSingleStraight_Request;
  using Response = xarm_msgs::srv::PlanSingleStraight_Response;
};

}  // namespace srv

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__PLAN_SINGLE_STRAIGHT__STRUCT_HPP_
