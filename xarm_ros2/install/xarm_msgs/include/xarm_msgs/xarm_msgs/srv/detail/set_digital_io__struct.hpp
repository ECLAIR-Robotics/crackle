// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from xarm_msgs:srv/SetDigitalIO.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__SET_DIGITAL_IO__STRUCT_HPP_
#define XARM_MSGS__SRV__DETAIL__SET_DIGITAL_IO__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__xarm_msgs__srv__SetDigitalIO_Request __attribute__((deprecated))
#else
# define DEPRECATED__xarm_msgs__srv__SetDigitalIO_Request __declspec(deprecated)
#endif

namespace xarm_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct SetDigitalIO_Request_
{
  using Type = SetDigitalIO_Request_<ContainerAllocator>;

  explicit SetDigitalIO_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->delay_sec = 0.0f;
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->ionum = 0;
      this->value = 0;
      this->delay_sec = 0.0f;
      this->tol_r = 0.0f;
    }
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->ionum = 0;
      this->value = 0;
      this->tol_r = 0.0f;
    }
  }

  explicit SetDigitalIO_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->delay_sec = 0.0f;
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->ionum = 0;
      this->value = 0;
      this->delay_sec = 0.0f;
      this->tol_r = 0.0f;
    }
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->ionum = 0;
      this->value = 0;
      this->tol_r = 0.0f;
    }
  }

  // field types and members
  using _ionum_type =
    int16_t;
  _ionum_type ionum;
  using _value_type =
    int16_t;
  _value_type value;
  using _delay_sec_type =
    float;
  _delay_sec_type delay_sec;
  using _xyz_type =
    std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>>;
  _xyz_type xyz;
  using _tol_r_type =
    float;
  _tol_r_type tol_r;

  // setters for named parameter idiom
  Type & set__ionum(
    const int16_t & _arg)
  {
    this->ionum = _arg;
    return *this;
  }
  Type & set__value(
    const int16_t & _arg)
  {
    this->value = _arg;
    return *this;
  }
  Type & set__delay_sec(
    const float & _arg)
  {
    this->delay_sec = _arg;
    return *this;
  }
  Type & set__xyz(
    const std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>> & _arg)
  {
    this->xyz = _arg;
    return *this;
  }
  Type & set__tol_r(
    const float & _arg)
  {
    this->tol_r = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    xarm_msgs::srv::SetDigitalIO_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const xarm_msgs::srv::SetDigitalIO_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<xarm_msgs::srv::SetDigitalIO_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<xarm_msgs::srv::SetDigitalIO_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::SetDigitalIO_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::SetDigitalIO_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::SetDigitalIO_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::SetDigitalIO_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<xarm_msgs::srv::SetDigitalIO_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<xarm_msgs::srv::SetDigitalIO_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__xarm_msgs__srv__SetDigitalIO_Request
    std::shared_ptr<xarm_msgs::srv::SetDigitalIO_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__xarm_msgs__srv__SetDigitalIO_Request
    std::shared_ptr<xarm_msgs::srv::SetDigitalIO_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SetDigitalIO_Request_ & other) const
  {
    if (this->ionum != other.ionum) {
      return false;
    }
    if (this->value != other.value) {
      return false;
    }
    if (this->delay_sec != other.delay_sec) {
      return false;
    }
    if (this->xyz != other.xyz) {
      return false;
    }
    if (this->tol_r != other.tol_r) {
      return false;
    }
    return true;
  }
  bool operator!=(const SetDigitalIO_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SetDigitalIO_Request_

// alias to use template instance with default allocator
using SetDigitalIO_Request =
  xarm_msgs::srv::SetDigitalIO_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace xarm_msgs


#ifndef _WIN32
# define DEPRECATED__xarm_msgs__srv__SetDigitalIO_Response __attribute__((deprecated))
#else
# define DEPRECATED__xarm_msgs__srv__SetDigitalIO_Response __declspec(deprecated)
#endif

namespace xarm_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct SetDigitalIO_Response_
{
  using Type = SetDigitalIO_Response_<ContainerAllocator>;

  explicit SetDigitalIO_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->ret = 0;
      this->message = "";
    }
  }

  explicit SetDigitalIO_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
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
    xarm_msgs::srv::SetDigitalIO_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const xarm_msgs::srv::SetDigitalIO_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<xarm_msgs::srv::SetDigitalIO_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<xarm_msgs::srv::SetDigitalIO_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::SetDigitalIO_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::SetDigitalIO_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::SetDigitalIO_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::SetDigitalIO_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<xarm_msgs::srv::SetDigitalIO_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<xarm_msgs::srv::SetDigitalIO_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__xarm_msgs__srv__SetDigitalIO_Response
    std::shared_ptr<xarm_msgs::srv::SetDigitalIO_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__xarm_msgs__srv__SetDigitalIO_Response
    std::shared_ptr<xarm_msgs::srv::SetDigitalIO_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SetDigitalIO_Response_ & other) const
  {
    if (this->ret != other.ret) {
      return false;
    }
    if (this->message != other.message) {
      return false;
    }
    return true;
  }
  bool operator!=(const SetDigitalIO_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SetDigitalIO_Response_

// alias to use template instance with default allocator
using SetDigitalIO_Response =
  xarm_msgs::srv::SetDigitalIO_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace xarm_msgs

namespace xarm_msgs
{

namespace srv
{

struct SetDigitalIO
{
  using Request = xarm_msgs::srv::SetDigitalIO_Request;
  using Response = xarm_msgs::srv::SetDigitalIO_Response;
};

}  // namespace srv

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__SET_DIGITAL_IO__STRUCT_HPP_
