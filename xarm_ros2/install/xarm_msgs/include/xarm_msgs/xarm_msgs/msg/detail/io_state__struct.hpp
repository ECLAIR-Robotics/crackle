// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from xarm_msgs:msg/IOState.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__MSG__DETAIL__IO_STATE__STRUCT_HPP_
#define XARM_MSGS__MSG__DETAIL__IO_STATE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__xarm_msgs__msg__IOState __attribute__((deprecated))
#else
# define DEPRECATED__xarm_msgs__msg__IOState __declspec(deprecated)
#endif

namespace xarm_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct IOState_
{
  using Type = IOState_<ContainerAllocator>;

  explicit IOState_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->digital_1 = 0l;
      this->digital_2 = 0l;
      this->analog_1 = 0.0f;
      this->analog_2 = 0.0f;
    }
  }

  explicit IOState_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->digital_1 = 0l;
      this->digital_2 = 0l;
      this->analog_1 = 0.0f;
      this->analog_2 = 0.0f;
    }
  }

  // field types and members
  using _digital_1_type =
    int32_t;
  _digital_1_type digital_1;
  using _digital_2_type =
    int32_t;
  _digital_2_type digital_2;
  using _analog_1_type =
    float;
  _analog_1_type analog_1;
  using _analog_2_type =
    float;
  _analog_2_type analog_2;

  // setters for named parameter idiom
  Type & set__digital_1(
    const int32_t & _arg)
  {
    this->digital_1 = _arg;
    return *this;
  }
  Type & set__digital_2(
    const int32_t & _arg)
  {
    this->digital_2 = _arg;
    return *this;
  }
  Type & set__analog_1(
    const float & _arg)
  {
    this->analog_1 = _arg;
    return *this;
  }
  Type & set__analog_2(
    const float & _arg)
  {
    this->analog_2 = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    xarm_msgs::msg::IOState_<ContainerAllocator> *;
  using ConstRawPtr =
    const xarm_msgs::msg::IOState_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<xarm_msgs::msg::IOState_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<xarm_msgs::msg::IOState_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::msg::IOState_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::msg::IOState_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::msg::IOState_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::msg::IOState_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<xarm_msgs::msg::IOState_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<xarm_msgs::msg::IOState_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__xarm_msgs__msg__IOState
    std::shared_ptr<xarm_msgs::msg::IOState_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__xarm_msgs__msg__IOState
    std::shared_ptr<xarm_msgs::msg::IOState_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const IOState_ & other) const
  {
    if (this->digital_1 != other.digital_1) {
      return false;
    }
    if (this->digital_2 != other.digital_2) {
      return false;
    }
    if (this->analog_1 != other.analog_1) {
      return false;
    }
    if (this->analog_2 != other.analog_2) {
      return false;
    }
    return true;
  }
  bool operator!=(const IOState_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct IOState_

// alias to use template instance with default allocator
using IOState =
  xarm_msgs::msg::IOState_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace xarm_msgs

#endif  // XARM_MSGS__MSG__DETAIL__IO_STATE__STRUCT_HPP_
