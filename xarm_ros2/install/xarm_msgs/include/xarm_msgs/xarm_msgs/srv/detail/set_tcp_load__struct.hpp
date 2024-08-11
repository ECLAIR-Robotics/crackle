// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from xarm_msgs:srv/SetTcpLoad.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__SET_TCP_LOAD__STRUCT_HPP_
#define XARM_MSGS__SRV__DETAIL__SET_TCP_LOAD__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__xarm_msgs__srv__SetTcpLoad_Request __attribute__((deprecated))
#else
# define DEPRECATED__xarm_msgs__srv__SetTcpLoad_Request __declspec(deprecated)
#endif

namespace xarm_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct SetTcpLoad_Request_
{
  using Type = SetTcpLoad_Request_<ContainerAllocator>;

  explicit SetTcpLoad_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->weight = 0.0f;
    }
  }

  explicit SetTcpLoad_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->weight = 0.0f;
    }
  }

  // field types and members
  using _weight_type =
    float;
  _weight_type weight;
  using _center_of_gravity_type =
    std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>>;
  _center_of_gravity_type center_of_gravity;

  // setters for named parameter idiom
  Type & set__weight(
    const float & _arg)
  {
    this->weight = _arg;
    return *this;
  }
  Type & set__center_of_gravity(
    const std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>> & _arg)
  {
    this->center_of_gravity = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    xarm_msgs::srv::SetTcpLoad_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const xarm_msgs::srv::SetTcpLoad_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<xarm_msgs::srv::SetTcpLoad_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<xarm_msgs::srv::SetTcpLoad_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::SetTcpLoad_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::SetTcpLoad_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::SetTcpLoad_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::SetTcpLoad_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<xarm_msgs::srv::SetTcpLoad_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<xarm_msgs::srv::SetTcpLoad_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__xarm_msgs__srv__SetTcpLoad_Request
    std::shared_ptr<xarm_msgs::srv::SetTcpLoad_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__xarm_msgs__srv__SetTcpLoad_Request
    std::shared_ptr<xarm_msgs::srv::SetTcpLoad_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SetTcpLoad_Request_ & other) const
  {
    if (this->weight != other.weight) {
      return false;
    }
    if (this->center_of_gravity != other.center_of_gravity) {
      return false;
    }
    return true;
  }
  bool operator!=(const SetTcpLoad_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SetTcpLoad_Request_

// alias to use template instance with default allocator
using SetTcpLoad_Request =
  xarm_msgs::srv::SetTcpLoad_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace xarm_msgs


#ifndef _WIN32
# define DEPRECATED__xarm_msgs__srv__SetTcpLoad_Response __attribute__((deprecated))
#else
# define DEPRECATED__xarm_msgs__srv__SetTcpLoad_Response __declspec(deprecated)
#endif

namespace xarm_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct SetTcpLoad_Response_
{
  using Type = SetTcpLoad_Response_<ContainerAllocator>;

  explicit SetTcpLoad_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->ret = 0l;
      this->message = "";
    }
  }

  explicit SetTcpLoad_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : message(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->ret = 0l;
      this->message = "";
    }
  }

  // field types and members
  using _ret_type =
    int32_t;
  _ret_type ret;
  using _message_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _message_type message;

  // setters for named parameter idiom
  Type & set__ret(
    const int32_t & _arg)
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
    xarm_msgs::srv::SetTcpLoad_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const xarm_msgs::srv::SetTcpLoad_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<xarm_msgs::srv::SetTcpLoad_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<xarm_msgs::srv::SetTcpLoad_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::SetTcpLoad_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::SetTcpLoad_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::SetTcpLoad_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::SetTcpLoad_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<xarm_msgs::srv::SetTcpLoad_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<xarm_msgs::srv::SetTcpLoad_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__xarm_msgs__srv__SetTcpLoad_Response
    std::shared_ptr<xarm_msgs::srv::SetTcpLoad_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__xarm_msgs__srv__SetTcpLoad_Response
    std::shared_ptr<xarm_msgs::srv::SetTcpLoad_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SetTcpLoad_Response_ & other) const
  {
    if (this->ret != other.ret) {
      return false;
    }
    if (this->message != other.message) {
      return false;
    }
    return true;
  }
  bool operator!=(const SetTcpLoad_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SetTcpLoad_Response_

// alias to use template instance with default allocator
using SetTcpLoad_Response =
  xarm_msgs::srv::SetTcpLoad_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace xarm_msgs

namespace xarm_msgs
{

namespace srv
{

struct SetTcpLoad
{
  using Request = xarm_msgs::srv::SetTcpLoad_Request;
  using Response = xarm_msgs::srv::SetTcpLoad_Response;
};

}  // namespace srv

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__SET_TCP_LOAD__STRUCT_HPP_
