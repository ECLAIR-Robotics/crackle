// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from xarm_msgs:srv/BioGripperCtrl.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__BIO_GRIPPER_CTRL__STRUCT_HPP_
#define XARM_MSGS__SRV__DETAIL__BIO_GRIPPER_CTRL__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__xarm_msgs__srv__BioGripperCtrl_Request __attribute__((deprecated))
#else
# define DEPRECATED__xarm_msgs__srv__BioGripperCtrl_Request __declspec(deprecated)
#endif

namespace xarm_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct BioGripperCtrl_Request_
{
  using Type = BioGripperCtrl_Request_<ContainerAllocator>;

  explicit BioGripperCtrl_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->speed = 0;
      this->wait = true;
      this->timeout = 5.0f;
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->speed = 0;
      this->wait = false;
      this->timeout = 0.0f;
    }
  }

  explicit BioGripperCtrl_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->speed = 0;
      this->wait = true;
      this->timeout = 5.0f;
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->speed = 0;
      this->wait = false;
      this->timeout = 0.0f;
    }
  }

  // field types and members
  using _speed_type =
    int16_t;
  _speed_type speed;
  using _wait_type =
    bool;
  _wait_type wait;
  using _timeout_type =
    float;
  _timeout_type timeout;

  // setters for named parameter idiom
  Type & set__speed(
    const int16_t & _arg)
  {
    this->speed = _arg;
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

  // constant declarations

  // pointer types
  using RawPtr =
    xarm_msgs::srv::BioGripperCtrl_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const xarm_msgs::srv::BioGripperCtrl_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<xarm_msgs::srv::BioGripperCtrl_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<xarm_msgs::srv::BioGripperCtrl_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::BioGripperCtrl_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::BioGripperCtrl_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::BioGripperCtrl_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::BioGripperCtrl_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<xarm_msgs::srv::BioGripperCtrl_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<xarm_msgs::srv::BioGripperCtrl_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__xarm_msgs__srv__BioGripperCtrl_Request
    std::shared_ptr<xarm_msgs::srv::BioGripperCtrl_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__xarm_msgs__srv__BioGripperCtrl_Request
    std::shared_ptr<xarm_msgs::srv::BioGripperCtrl_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const BioGripperCtrl_Request_ & other) const
  {
    if (this->speed != other.speed) {
      return false;
    }
    if (this->wait != other.wait) {
      return false;
    }
    if (this->timeout != other.timeout) {
      return false;
    }
    return true;
  }
  bool operator!=(const BioGripperCtrl_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct BioGripperCtrl_Request_

// alias to use template instance with default allocator
using BioGripperCtrl_Request =
  xarm_msgs::srv::BioGripperCtrl_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace xarm_msgs


#ifndef _WIN32
# define DEPRECATED__xarm_msgs__srv__BioGripperCtrl_Response __attribute__((deprecated))
#else
# define DEPRECATED__xarm_msgs__srv__BioGripperCtrl_Response __declspec(deprecated)
#endif

namespace xarm_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct BioGripperCtrl_Response_
{
  using Type = BioGripperCtrl_Response_<ContainerAllocator>;

  explicit BioGripperCtrl_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->ret = 0;
      this->message = "";
    }
  }

  explicit BioGripperCtrl_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
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
    xarm_msgs::srv::BioGripperCtrl_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const xarm_msgs::srv::BioGripperCtrl_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<xarm_msgs::srv::BioGripperCtrl_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<xarm_msgs::srv::BioGripperCtrl_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::BioGripperCtrl_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::BioGripperCtrl_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::BioGripperCtrl_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::BioGripperCtrl_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<xarm_msgs::srv::BioGripperCtrl_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<xarm_msgs::srv::BioGripperCtrl_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__xarm_msgs__srv__BioGripperCtrl_Response
    std::shared_ptr<xarm_msgs::srv::BioGripperCtrl_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__xarm_msgs__srv__BioGripperCtrl_Response
    std::shared_ptr<xarm_msgs::srv::BioGripperCtrl_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const BioGripperCtrl_Response_ & other) const
  {
    if (this->ret != other.ret) {
      return false;
    }
    if (this->message != other.message) {
      return false;
    }
    return true;
  }
  bool operator!=(const BioGripperCtrl_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct BioGripperCtrl_Response_

// alias to use template instance with default allocator
using BioGripperCtrl_Response =
  xarm_msgs::srv::BioGripperCtrl_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace xarm_msgs

namespace xarm_msgs
{

namespace srv
{

struct BioGripperCtrl
{
  using Request = xarm_msgs::srv::BioGripperCtrl_Request;
  using Response = xarm_msgs::srv::BioGripperCtrl_Response;
};

}  // namespace srv

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__BIO_GRIPPER_CTRL__STRUCT_HPP_
