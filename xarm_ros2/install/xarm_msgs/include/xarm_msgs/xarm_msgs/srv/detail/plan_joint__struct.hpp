// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from xarm_msgs:srv/PlanJoint.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__PLAN_JOINT__STRUCT_HPP_
#define XARM_MSGS__SRV__DETAIL__PLAN_JOINT__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__xarm_msgs__srv__PlanJoint_Request __attribute__((deprecated))
#else
# define DEPRECATED__xarm_msgs__srv__PlanJoint_Request __declspec(deprecated)
#endif

namespace xarm_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct PlanJoint_Request_
{
  using Type = PlanJoint_Request_<ContainerAllocator>;

  explicit PlanJoint_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
  }

  explicit PlanJoint_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
    (void)_alloc;
  }

  // field types and members
  using _target_type =
    std::vector<double, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<double>>;
  _target_type target;

  // setters for named parameter idiom
  Type & set__target(
    const std::vector<double, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<double>> & _arg)
  {
    this->target = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    xarm_msgs::srv::PlanJoint_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const xarm_msgs::srv::PlanJoint_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<xarm_msgs::srv::PlanJoint_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<xarm_msgs::srv::PlanJoint_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::PlanJoint_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::PlanJoint_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::PlanJoint_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::PlanJoint_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<xarm_msgs::srv::PlanJoint_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<xarm_msgs::srv::PlanJoint_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__xarm_msgs__srv__PlanJoint_Request
    std::shared_ptr<xarm_msgs::srv::PlanJoint_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__xarm_msgs__srv__PlanJoint_Request
    std::shared_ptr<xarm_msgs::srv::PlanJoint_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const PlanJoint_Request_ & other) const
  {
    if (this->target != other.target) {
      return false;
    }
    return true;
  }
  bool operator!=(const PlanJoint_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct PlanJoint_Request_

// alias to use template instance with default allocator
using PlanJoint_Request =
  xarm_msgs::srv::PlanJoint_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace xarm_msgs


#ifndef _WIN32
# define DEPRECATED__xarm_msgs__srv__PlanJoint_Response __attribute__((deprecated))
#else
# define DEPRECATED__xarm_msgs__srv__PlanJoint_Response __declspec(deprecated)
#endif

namespace xarm_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct PlanJoint_Response_
{
  using Type = PlanJoint_Response_<ContainerAllocator>;

  explicit PlanJoint_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
    }
  }

  explicit PlanJoint_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
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
    xarm_msgs::srv::PlanJoint_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const xarm_msgs::srv::PlanJoint_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<xarm_msgs::srv::PlanJoint_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<xarm_msgs::srv::PlanJoint_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::PlanJoint_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::PlanJoint_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::PlanJoint_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::PlanJoint_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<xarm_msgs::srv::PlanJoint_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<xarm_msgs::srv::PlanJoint_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__xarm_msgs__srv__PlanJoint_Response
    std::shared_ptr<xarm_msgs::srv::PlanJoint_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__xarm_msgs__srv__PlanJoint_Response
    std::shared_ptr<xarm_msgs::srv::PlanJoint_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const PlanJoint_Response_ & other) const
  {
    if (this->success != other.success) {
      return false;
    }
    return true;
  }
  bool operator!=(const PlanJoint_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct PlanJoint_Response_

// alias to use template instance with default allocator
using PlanJoint_Response =
  xarm_msgs::srv::PlanJoint_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace xarm_msgs

namespace xarm_msgs
{

namespace srv
{

struct PlanJoint
{
  using Request = xarm_msgs::srv::PlanJoint_Request;
  using Response = xarm_msgs::srv::PlanJoint_Response;
};

}  // namespace srv

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__PLAN_JOINT__STRUCT_HPP_
