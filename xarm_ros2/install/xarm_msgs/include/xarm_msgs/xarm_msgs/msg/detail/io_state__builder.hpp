// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:msg/IOState.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__MSG__DETAIL__IO_STATE__BUILDER_HPP_
#define XARM_MSGS__MSG__DETAIL__IO_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/msg/detail/io_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace msg
{

namespace builder
{

class Init_IOState_analog_2
{
public:
  explicit Init_IOState_analog_2(::xarm_msgs::msg::IOState & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::msg::IOState analog_2(::xarm_msgs::msg::IOState::_analog_2_type arg)
  {
    msg_.analog_2 = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::msg::IOState msg_;
};

class Init_IOState_analog_1
{
public:
  explicit Init_IOState_analog_1(::xarm_msgs::msg::IOState & msg)
  : msg_(msg)
  {}
  Init_IOState_analog_2 analog_1(::xarm_msgs::msg::IOState::_analog_1_type arg)
  {
    msg_.analog_1 = std::move(arg);
    return Init_IOState_analog_2(msg_);
  }

private:
  ::xarm_msgs::msg::IOState msg_;
};

class Init_IOState_digital_2
{
public:
  explicit Init_IOState_digital_2(::xarm_msgs::msg::IOState & msg)
  : msg_(msg)
  {}
  Init_IOState_analog_1 digital_2(::xarm_msgs::msg::IOState::_digital_2_type arg)
  {
    msg_.digital_2 = std::move(arg);
    return Init_IOState_analog_1(msg_);
  }

private:
  ::xarm_msgs::msg::IOState msg_;
};

class Init_IOState_digital_1
{
public:
  Init_IOState_digital_1()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_IOState_digital_2 digital_1(::xarm_msgs::msg::IOState::_digital_1_type arg)
  {
    msg_.digital_1 = std::move(arg);
    return Init_IOState_digital_2(msg_);
  }

private:
  ::xarm_msgs::msg::IOState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::msg::IOState>()
{
  return xarm_msgs::msg::builder::Init_IOState_digital_1();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__MSG__DETAIL__IO_STATE__BUILDER_HPP_
