// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from xarm_msgs:msg/CIOState.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__MSG__DETAIL__CIO_STATE__STRUCT_HPP_
#define XARM_MSGS__MSG__DETAIL__CIO_STATE__STRUCT_HPP_

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
# define DEPRECATED__xarm_msgs__msg__CIOState __attribute__((deprecated))
#else
# define DEPRECATED__xarm_msgs__msg__CIOState __declspec(deprecated)
#endif

namespace xarm_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct CIOState_
{
  using Type = CIOState_<ContainerAllocator>;

  explicit CIOState_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->state = 0;
      this->code = 0;
      std::fill<typename std::array<uint16_t, 2>::iterator, uint16_t>(this->input_digitals.begin(), this->input_digitals.end(), 0);
      std::fill<typename std::array<uint16_t, 2>::iterator, uint16_t>(this->output_digitals.begin(), this->output_digitals.end(), 0);
      std::fill<typename std::array<float, 2>::iterator, float>(this->input_analogs.begin(), this->input_analogs.end(), 0.0f);
      std::fill<typename std::array<float, 2>::iterator, float>(this->output_analogs.begin(), this->output_analogs.end(), 0.0f);
      std::fill<typename std::array<uint8_t, 16>::iterator, uint8_t>(this->input_conf.begin(), this->input_conf.end(), 0);
      std::fill<typename std::array<uint8_t, 16>::iterator, uint8_t>(this->output_conf.begin(), this->output_conf.end(), 0);
    }
  }

  explicit CIOState_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init),
    input_digitals(_alloc),
    output_digitals(_alloc),
    input_analogs(_alloc),
    output_analogs(_alloc),
    input_conf(_alloc),
    output_conf(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->state = 0;
      this->code = 0;
      std::fill<typename std::array<uint16_t, 2>::iterator, uint16_t>(this->input_digitals.begin(), this->input_digitals.end(), 0);
      std::fill<typename std::array<uint16_t, 2>::iterator, uint16_t>(this->output_digitals.begin(), this->output_digitals.end(), 0);
      std::fill<typename std::array<float, 2>::iterator, float>(this->input_analogs.begin(), this->input_analogs.end(), 0.0f);
      std::fill<typename std::array<float, 2>::iterator, float>(this->output_analogs.begin(), this->output_analogs.end(), 0.0f);
      std::fill<typename std::array<uint8_t, 16>::iterator, uint8_t>(this->input_conf.begin(), this->input_conf.end(), 0);
      std::fill<typename std::array<uint8_t, 16>::iterator, uint8_t>(this->output_conf.begin(), this->output_conf.end(), 0);
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _state_type =
    int16_t;
  _state_type state;
  using _code_type =
    int16_t;
  _code_type code;
  using _input_digitals_type =
    std::array<uint16_t, 2>;
  _input_digitals_type input_digitals;
  using _output_digitals_type =
    std::array<uint16_t, 2>;
  _output_digitals_type output_digitals;
  using _input_analogs_type =
    std::array<float, 2>;
  _input_analogs_type input_analogs;
  using _output_analogs_type =
    std::array<float, 2>;
  _output_analogs_type output_analogs;
  using _input_conf_type =
    std::array<uint8_t, 16>;
  _input_conf_type input_conf;
  using _output_conf_type =
    std::array<uint8_t, 16>;
  _output_conf_type output_conf;

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
  Type & set__code(
    const int16_t & _arg)
  {
    this->code = _arg;
    return *this;
  }
  Type & set__input_digitals(
    const std::array<uint16_t, 2> & _arg)
  {
    this->input_digitals = _arg;
    return *this;
  }
  Type & set__output_digitals(
    const std::array<uint16_t, 2> & _arg)
  {
    this->output_digitals = _arg;
    return *this;
  }
  Type & set__input_analogs(
    const std::array<float, 2> & _arg)
  {
    this->input_analogs = _arg;
    return *this;
  }
  Type & set__output_analogs(
    const std::array<float, 2> & _arg)
  {
    this->output_analogs = _arg;
    return *this;
  }
  Type & set__input_conf(
    const std::array<uint8_t, 16> & _arg)
  {
    this->input_conf = _arg;
    return *this;
  }
  Type & set__output_conf(
    const std::array<uint8_t, 16> & _arg)
  {
    this->output_conf = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    xarm_msgs::msg::CIOState_<ContainerAllocator> *;
  using ConstRawPtr =
    const xarm_msgs::msg::CIOState_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<xarm_msgs::msg::CIOState_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<xarm_msgs::msg::CIOState_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::msg::CIOState_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::msg::CIOState_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::msg::CIOState_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::msg::CIOState_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<xarm_msgs::msg::CIOState_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<xarm_msgs::msg::CIOState_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__xarm_msgs__msg__CIOState
    std::shared_ptr<xarm_msgs::msg::CIOState_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__xarm_msgs__msg__CIOState
    std::shared_ptr<xarm_msgs::msg::CIOState_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const CIOState_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->state != other.state) {
      return false;
    }
    if (this->code != other.code) {
      return false;
    }
    if (this->input_digitals != other.input_digitals) {
      return false;
    }
    if (this->output_digitals != other.output_digitals) {
      return false;
    }
    if (this->input_analogs != other.input_analogs) {
      return false;
    }
    if (this->output_analogs != other.output_analogs) {
      return false;
    }
    if (this->input_conf != other.input_conf) {
      return false;
    }
    if (this->output_conf != other.output_conf) {
      return false;
    }
    return true;
  }
  bool operator!=(const CIOState_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct CIOState_

// alias to use template instance with default allocator
using CIOState =
  xarm_msgs::msg::CIOState_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace xarm_msgs

#endif  // XARM_MSGS__MSG__DETAIL__CIO_STATE__STRUCT_HPP_
