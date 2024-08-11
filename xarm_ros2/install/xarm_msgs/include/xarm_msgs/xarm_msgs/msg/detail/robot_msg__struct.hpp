// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from xarm_msgs:msg/RobotMsg.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__MSG__DETAIL__ROBOT_MSG__STRUCT_HPP_
#define XARM_MSGS__MSG__DETAIL__ROBOT_MSG__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__xarm_msgs__msg__RobotMsg __attribute__((deprecated))
#else
# define DEPRECATED__xarm_msgs__msg__RobotMsg __declspec(deprecated)
#endif

namespace xarm_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct RobotMsg_
{
  using Type = RobotMsg_<ContainerAllocator>;

  explicit RobotMsg_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->state = 0;
      this->mode = 0;
      this->cmdnum = 0;
      this->mt_brake = 0;
      this->mt_able = 0;
      this->err = 0;
      this->warn = 0;
      std::fill<typename std::array<float, 6>::iterator, float>(this->pose.begin(), this->pose.end(), 0.0f);
      std::fill<typename std::array<float, 6>::iterator, float>(this->offset.begin(), this->offset.end(), 0.0f);
    }
  }

  explicit RobotMsg_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init),
    pose(_alloc),
    offset(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->state = 0;
      this->mode = 0;
      this->cmdnum = 0;
      this->mt_brake = 0;
      this->mt_able = 0;
      this->err = 0;
      this->warn = 0;
      std::fill<typename std::array<float, 6>::iterator, float>(this->pose.begin(), this->pose.end(), 0.0f);
      std::fill<typename std::array<float, 6>::iterator, float>(this->offset.begin(), this->offset.end(), 0.0f);
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _state_type =
    int16_t;
  _state_type state;
  using _mode_type =
    int16_t;
  _mode_type mode;
  using _cmdnum_type =
    int16_t;
  _cmdnum_type cmdnum;
  using _mt_brake_type =
    int16_t;
  _mt_brake_type mt_brake;
  using _mt_able_type =
    int16_t;
  _mt_able_type mt_able;
  using _err_type =
    int16_t;
  _err_type err;
  using _warn_type =
    int16_t;
  _warn_type warn;
  using _angle_type =
    std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>>;
  _angle_type angle;
  using _pose_type =
    std::array<float, 6>;
  _pose_type pose;
  using _offset_type =
    std::array<float, 6>;
  _offset_type offset;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__state(
    const int16_t & _arg)
  {
    this->state = _arg;
    return *this;
  }
  Type & set__mode(
    const int16_t & _arg)
  {
    this->mode = _arg;
    return *this;
  }
  Type & set__cmdnum(
    const int16_t & _arg)
  {
    this->cmdnum = _arg;
    return *this;
  }
  Type & set__mt_brake(
    const int16_t & _arg)
  {
    this->mt_brake = _arg;
    return *this;
  }
  Type & set__mt_able(
    const int16_t & _arg)
  {
    this->mt_able = _arg;
    return *this;
  }
  Type & set__err(
    const int16_t & _arg)
  {
    this->err = _arg;
    return *this;
  }
  Type & set__warn(
    const int16_t & _arg)
  {
    this->warn = _arg;
    return *this;
  }
  Type & set__angle(
    const std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>> & _arg)
  {
    this->angle = _arg;
    return *this;
  }
  Type & set__pose(
    const std::array<float, 6> & _arg)
  {
    this->pose = _arg;
    return *this;
  }
  Type & set__offset(
    const std::array<float, 6> & _arg)
  {
    this->offset = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    xarm_msgs::msg::RobotMsg_<ContainerAllocator> *;
  using ConstRawPtr =
    const xarm_msgs::msg::RobotMsg_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<xarm_msgs::msg::RobotMsg_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<xarm_msgs::msg::RobotMsg_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::msg::RobotMsg_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::msg::RobotMsg_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::msg::RobotMsg_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::msg::RobotMsg_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<xarm_msgs::msg::RobotMsg_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<xarm_msgs::msg::RobotMsg_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__xarm_msgs__msg__RobotMsg
    std::shared_ptr<xarm_msgs::msg::RobotMsg_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__xarm_msgs__msg__RobotMsg
    std::shared_ptr<xarm_msgs::msg::RobotMsg_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const RobotMsg_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->state != other.state) {
      return false;
    }
    if (this->mode != other.mode) {
      return false;
    }
    if (this->cmdnum != other.cmdnum) {
      return false;
    }
    if (this->mt_brake != other.mt_brake) {
      return false;
    }
    if (this->mt_able != other.mt_able) {
      return false;
    }
    if (this->err != other.err) {
      return false;
    }
    if (this->warn != other.warn) {
      return false;
    }
    if (this->angle != other.angle) {
      return false;
    }
    if (this->pose != other.pose) {
      return false;
    }
    if (this->offset != other.offset) {
      return false;
    }
    return true;
  }
  bool operator!=(const RobotMsg_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct RobotMsg_

// alias to use template instance with default allocator
using RobotMsg =
  xarm_msgs::msg::RobotMsg_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace xarm_msgs

#endif  // XARM_MSGS__MSG__DETAIL__ROBOT_MSG__STRUCT_HPP_
