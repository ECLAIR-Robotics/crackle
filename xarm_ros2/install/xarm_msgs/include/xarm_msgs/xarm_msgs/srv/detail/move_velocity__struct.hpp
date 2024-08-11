// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from xarm_msgs:srv/MoveVelocity.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__MOVE_VELOCITY__STRUCT_HPP_
#define XARM_MSGS__SRV__DETAIL__MOVE_VELOCITY__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__xarm_msgs__srv__MoveVelocity_Request __attribute__((deprecated))
#else
# define DEPRECATED__xarm_msgs__srv__MoveVelocity_Request __declspec(deprecated)
#endif

namespace xarm_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct MoveVelocity_Request_
{
  using Type = MoveVelocity_Request_<ContainerAllocator>;

  explicit MoveVelocity_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->is_sync = true;
      this->is_tool_coord = false;
      this->duration = -1.0f;
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->is_sync = false;
      this->is_tool_coord = false;
      this->duration = 0.0f;
    }
  }

  explicit MoveVelocity_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->is_sync = true;
      this->is_tool_coord = false;
      this->duration = -1.0f;
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->is_sync = false;
      this->is_tool_coord = false;
      this->duration = 0.0f;
    }
  }

  // field types and members
  using _speeds_type =
    std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>>;
  _speeds_type speeds;
  using _is_sync_type =
    bool;
  _is_sync_type is_sync;
  using _is_tool_coord_type =
    bool;
  _is_tool_coord_type is_tool_coord;
  using _duration_type =
    float;
  _duration_type duration;

  // setters for named parameter idiom
  Type & set__speeds(
    const std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>> & _arg)
  {
    this->speeds = _arg;
    return *this;
  }
  Type & set__is_sync(
    const bool & _arg)
  {
    this->is_sync = _arg;
    return *this;
  }
  Type & set__is_tool_coord(
    const bool & _arg)
  {
    this->is_tool_coord = _arg;
    return *this;
  }
  Type & set__duration(
    const float & _arg)
  {
    this->duration = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    xarm_msgs::srv::MoveVelocity_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const xarm_msgs::srv::MoveVelocity_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<xarm_msgs::srv::MoveVelocity_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<xarm_msgs::srv::MoveVelocity_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::MoveVelocity_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::MoveVelocity_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::MoveVelocity_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::MoveVelocity_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<xarm_msgs::srv::MoveVelocity_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<xarm_msgs::srv::MoveVelocity_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__xarm_msgs__srv__MoveVelocity_Request
    std::shared_ptr<xarm_msgs::srv::MoveVelocity_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__xarm_msgs__srv__MoveVelocity_Request
    std::shared_ptr<xarm_msgs::srv::MoveVelocity_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const MoveVelocity_Request_ & other) const
  {
    if (this->speeds != other.speeds) {
      return false;
    }
    if (this->is_sync != other.is_sync) {
      return false;
    }
    if (this->is_tool_coord != other.is_tool_coord) {
      return false;
    }
    if (this->duration != other.duration) {
      return false;
    }
    return true;
  }
  bool operator!=(const MoveVelocity_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct MoveVelocity_Request_

// alias to use template instance with default allocator
using MoveVelocity_Request =
  xarm_msgs::srv::MoveVelocity_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace xarm_msgs


#ifndef _WIN32
# define DEPRECATED__xarm_msgs__srv__MoveVelocity_Response __attribute__((deprecated))
#else
# define DEPRECATED__xarm_msgs__srv__MoveVelocity_Response __declspec(deprecated)
#endif

namespace xarm_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct MoveVelocity_Response_
{
  using Type = MoveVelocity_Response_<ContainerAllocator>;

  explicit MoveVelocity_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->ret = 0;
      this->message = "";
    }
  }

  explicit MoveVelocity_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
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
    xarm_msgs::srv::MoveVelocity_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const xarm_msgs::srv::MoveVelocity_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<xarm_msgs::srv::MoveVelocity_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<xarm_msgs::srv::MoveVelocity_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::MoveVelocity_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::MoveVelocity_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::MoveVelocity_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::MoveVelocity_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<xarm_msgs::srv::MoveVelocity_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<xarm_msgs::srv::MoveVelocity_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__xarm_msgs__srv__MoveVelocity_Response
    std::shared_ptr<xarm_msgs::srv::MoveVelocity_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__xarm_msgs__srv__MoveVelocity_Response
    std::shared_ptr<xarm_msgs::srv::MoveVelocity_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const MoveVelocity_Response_ & other) const
  {
    if (this->ret != other.ret) {
      return false;
    }
    if (this->message != other.message) {
      return false;
    }
    return true;
  }
  bool operator!=(const MoveVelocity_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct MoveVelocity_Response_

// alias to use template instance with default allocator
using MoveVelocity_Response =
  xarm_msgs::srv::MoveVelocity_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace xarm_msgs

namespace xarm_msgs
{

namespace srv
{

struct MoveVelocity
{
  using Request = xarm_msgs::srv::MoveVelocity_Request;
  using Response = xarm_msgs::srv::MoveVelocity_Response;
};

}  // namespace srv

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__MOVE_VELOCITY__STRUCT_HPP_
