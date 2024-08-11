// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from xarm_msgs:srv/MoveCartesian.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__MOVE_CARTESIAN__STRUCT_HPP_
#define XARM_MSGS__SRV__DETAIL__MOVE_CARTESIAN__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__xarm_msgs__srv__MoveCartesian_Request __attribute__((deprecated))
#else
# define DEPRECATED__xarm_msgs__srv__MoveCartesian_Request __declspec(deprecated)
#endif

namespace xarm_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct MoveCartesian_Request_
{
  using Type = MoveCartesian_Request_<ContainerAllocator>;

  explicit MoveCartesian_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->speed = 0.0f;
      this->acc = 0.0f;
      this->mvtime = 0.0f;
      this->wait = false;
      this->timeout = -1.0f;
      this->radius = -1.0f;
      this->is_tool_coord = false;
      this->relative = false;
      this->motion_type = 0;
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->speed = 0.0f;
      this->acc = 0.0f;
      this->mvtime = 0.0f;
      this->wait = false;
      this->timeout = 0.0f;
      this->radius = 0.0f;
      this->is_tool_coord = false;
      this->relative = false;
      this->motion_type = 0;
    }
  }

  explicit MoveCartesian_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->speed = 0.0f;
      this->acc = 0.0f;
      this->mvtime = 0.0f;
      this->wait = false;
      this->timeout = -1.0f;
      this->radius = -1.0f;
      this->is_tool_coord = false;
      this->relative = false;
      this->motion_type = 0;
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->speed = 0.0f;
      this->acc = 0.0f;
      this->mvtime = 0.0f;
      this->wait = false;
      this->timeout = 0.0f;
      this->radius = 0.0f;
      this->is_tool_coord = false;
      this->relative = false;
      this->motion_type = 0;
    }
  }

  // field types and members
  using _pose_type =
    std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>>;
  _pose_type pose;
  using _speed_type =
    float;
  _speed_type speed;
  using _acc_type =
    float;
  _acc_type acc;
  using _mvtime_type =
    float;
  _mvtime_type mvtime;
  using _wait_type =
    bool;
  _wait_type wait;
  using _timeout_type =
    float;
  _timeout_type timeout;
  using _radius_type =
    float;
  _radius_type radius;
  using _is_tool_coord_type =
    bool;
  _is_tool_coord_type is_tool_coord;
  using _relative_type =
    bool;
  _relative_type relative;
  using _motion_type_type =
    uint8_t;
  _motion_type_type motion_type;

  // setters for named parameter idiom
  Type & set__pose(
    const std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>> & _arg)
  {
    this->pose = _arg;
    return *this;
  }
  Type & set__speed(
    const float & _arg)
  {
    this->speed = _arg;
    return *this;
  }
  Type & set__acc(
    const float & _arg)
  {
    this->acc = _arg;
    return *this;
  }
  Type & set__mvtime(
    const float & _arg)
  {
    this->mvtime = _arg;
    return *this;
  }
  Type & set__wait(
    const bool & _arg)
  {
    this->wait = _arg;
    return *this;
  }
  Type & set__timeout(
    const float & _arg)
  {
    this->timeout = _arg;
    return *this;
  }
  Type & set__radius(
    const float & _arg)
  {
    this->radius = _arg;
    return *this;
  }
  Type & set__is_tool_coord(
    const bool & _arg)
  {
    this->is_tool_coord = _arg;
    return *this;
  }
  Type & set__relative(
    const bool & _arg)
  {
    this->relative = _arg;
    return *this;
  }
  Type & set__motion_type(
    const uint8_t & _arg)
  {
    this->motion_type = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    xarm_msgs::srv::MoveCartesian_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const xarm_msgs::srv::MoveCartesian_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<xarm_msgs::srv::MoveCartesian_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<xarm_msgs::srv::MoveCartesian_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::MoveCartesian_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::MoveCartesian_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::MoveCartesian_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::MoveCartesian_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<xarm_msgs::srv::MoveCartesian_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<xarm_msgs::srv::MoveCartesian_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__xarm_msgs__srv__MoveCartesian_Request
    std::shared_ptr<xarm_msgs::srv::MoveCartesian_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__xarm_msgs__srv__MoveCartesian_Request
    std::shared_ptr<xarm_msgs::srv::MoveCartesian_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const MoveCartesian_Request_ & other) const
  {
    if (this->pose != other.pose) {
      return false;
    }
    if (this->speed != other.speed) {
      return false;
    }
    if (this->acc != other.acc) {
      return false;
    }
    if (this->mvtime != other.mvtime) {
      return false;
    }
    if (this->wait != other.wait) {
      return false;
    }
    if (this->timeout != other.timeout) {
      return false;
    }
    if (this->radius != other.radius) {
      return false;
    }
    if (this->is_tool_coord != other.is_tool_coord) {
      return false;
    }
    if (this->relative != other.relative) {
      return false;
    }
    if (this->motion_type != other.motion_type) {
      return false;
    }
    return true;
  }
  bool operator!=(const MoveCartesian_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct MoveCartesian_Request_

// alias to use template instance with default allocator
using MoveCartesian_Request =
  xarm_msgs::srv::MoveCartesian_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace xarm_msgs


#ifndef _WIN32
# define DEPRECATED__xarm_msgs__srv__MoveCartesian_Response __attribute__((deprecated))
#else
# define DEPRECATED__xarm_msgs__srv__MoveCartesian_Response __declspec(deprecated)
#endif

namespace xarm_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct MoveCartesian_Response_
{
  using Type = MoveCartesian_Response_<ContainerAllocator>;

  explicit MoveCartesian_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->ret = 0;
      this->message = "";
    }
  }

  explicit MoveCartesian_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : message(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->ret = 0;
      this->message = "";
    }
  }

  // field types and members
  using _ret_type =
    int16_t;
  _ret_type ret;
  using _message_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _message_type message;

  // setters for named parameter idiom
  Type & set__ret(
    const int16_t & _arg)
  {
    this->ret = _arg;
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
    xarm_msgs::srv::MoveCartesian_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const xarm_msgs::srv::MoveCartesian_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<xarm_msgs::srv::MoveCartesian_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<xarm_msgs::srv::MoveCartesian_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::MoveCartesian_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::MoveCartesian_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::MoveCartesian_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::MoveCartesian_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<xarm_msgs::srv::MoveCartesian_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<xarm_msgs::srv::MoveCartesian_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__xarm_msgs__srv__MoveCartesian_Response
    std::shared_ptr<xarm_msgs::srv::MoveCartesian_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__xarm_msgs__srv__MoveCartesian_Response
    std::shared_ptr<xarm_msgs::srv::MoveCartesian_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const MoveCartesian_Response_ & other) const
  {
    if (this->ret != other.ret) {
      return false;
    }
    if (this->message != other.message) {
      return false;
    }
    return true;
  }
  bool operator!=(const MoveCartesian_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct MoveCartesian_Response_

// alias to use template instance with default allocator
using MoveCartesian_Response =
  xarm_msgs::srv::MoveCartesian_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace xarm_msgs

namespace xarm_msgs
{

namespace srv
{

struct MoveCartesian
{
  using Request = xarm_msgs::srv::MoveCartesian_Request;
  using Response = xarm_msgs::srv::MoveCartesian_Response;
};

}  // namespace srv

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__MOVE_CARTESIAN__STRUCT_HPP_
