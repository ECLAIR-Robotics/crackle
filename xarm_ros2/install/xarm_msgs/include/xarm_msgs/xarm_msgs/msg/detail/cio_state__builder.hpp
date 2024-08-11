// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:msg/CIOState.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__MSG__DETAIL__CIO_STATE__BUILDER_HPP_
#define XARM_MSGS__MSG__DETAIL__CIO_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/msg/detail/cio_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace msg
{

namespace builder
{

class Init_CIOState_output_conf
{
public:
  explicit Init_CIOState_output_conf(::xarm_msgs::msg::CIOState & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::msg::CIOState output_conf(::xarm_msgs::msg::CIOState::_output_conf_type arg)
  {
    msg_.output_conf = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::msg::CIOState msg_;
};

class Init_CIOState_input_conf
{
public:
  explicit Init_CIOState_input_conf(::xarm_msgs::msg::CIOState & msg)
  : msg_(msg)
  {}
  Init_CIOState_output_conf input_conf(::xarm_msgs::msg::CIOState::_input_conf_type arg)
  {
    msg_.input_conf = std::move(arg);
    return Init_CIOState_output_conf(msg_);
  }

private:
  ::xarm_msgs::msg::CIOState msg_;
};

class Init_CIOState_output_analogs
{
public:
  explicit Init_CIOState_output_analogs(::xarm_msgs::msg::CIOState & msg)
  : msg_(msg)
  {}
  Init_CIOState_input_conf output_analogs(::xarm_msgs::msg::CIOState::_output_analogs_type arg)
  {
    msg_.output_analogs = std::move(arg);
    return Init_CIOState_input_conf(msg_);
  }

private:
  ::xarm_msgs::msg::CIOState msg_;
};

class Init_CIOState_input_analogs
{
public:
  explicit Init_CIOState_input_analogs(::xarm_msgs::msg::CIOState & msg)
  : msg_(msg)
  {}
  Init_CIOState_output_analogs input_analogs(::xarm_msgs::msg::CIOState::_input_analogs_type arg)
  {
    msg_.input_analogs = std::move(arg);
    return Init_CIOState_output_analogs(msg_);
  }

private:
  ::xarm_msgs::msg::CIOState msg_;
};

class Init_CIOState_output_digitals
{
public:
  explicit Init_CIOState_output_digitals(::xarm_msgs::msg::CIOState & msg)
  : msg_(msg)
  {}
  Init_CIOState_input_analogs output_digitals(::xarm_msgs::msg::CIOState::_output_digitals_type arg)
  {
    msg_.output_digitals = std::move(arg);
    return Init_CIOState_input_analogs(msg_);
  }

private:
  ::xarm_msgs::msg::CIOState msg_;
};

class Init_CIOState_input_digitals
{
public:
  explicit Init_CIOState_input_digitals(::xarm_msgs::msg::CIOState & msg)
  : msg_(msg)
  {}
  Init_CIOState_output_digitals input_digitals(::xarm_msgs::msg::CIOState::_input_digitals_type arg)
  {
    msg_.input_digitals = std::move(arg);
    return Init_CIOState_output_digitals(msg_);
  }

private:
  ::xarm_msgs::msg::CIOState msg_;
};

class Init_CIOState_code
{
public:
  explicit Init_CIOState_code(::xarm_msgs::msg::CIOState & msg)
  : msg_(msg)
  {}
  Init_CIOState_input_digitals code(::xarm_msgs::msg::CIOState::_code_type arg)
  {
    msg_.code = std::move(arg);
    return Init_CIOState_input_digitals(msg_);
  }

private:
  ::xarm_msgs::msg::CIOState msg_;
};

class Init_CIOState_state
{
public:
  explicit Init_CIOState_state(::xarm_msgs::msg::CIOState & msg)
  : msg_(msg)
  {}
  Init_CIOState_code state(::xarm_msgs::msg::CIOState::_state_type arg)
  {
    msg_.state = std::move(arg);
    return Init_CIOState_code(msg_);
  }

private:
  ::xarm_msgs::msg::CIOState msg_;
};

class Init_CIOState_header
{
public:
  Init_CIOState_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_CIOState_state header(::xarm_msgs::msg::CIOState::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_CIOState_state(msg_);
  }

private:
  ::xarm_msgs::msg::CIOState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::msg::CIOState>()
{
  return xarm_msgs::msg::builder::Init_CIOState_header();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__MSG__DETAIL__CIO_STATE__BUILDER_HPP_
