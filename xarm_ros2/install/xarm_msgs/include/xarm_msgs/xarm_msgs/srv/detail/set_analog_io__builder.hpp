// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/SetAnalogIO.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__SET_ANALOG_IO__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__SET_ANALOG_IO__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/set_analog_io__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_SetAnalogIO_Request_tol_r
{
public:
  explicit Init_SetAnalogIO_Request_tol_r(::xarm_msgs::srv::SetAnalogIO_Request & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::SetAnalogIO_Request tol_r(::xarm_msgs::srv::SetAnalogIO_Request::_tol_r_type arg)
  {
    msg_.tol_r = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::SetAnalogIO_Request msg_;
};

class Init_SetAnalogIO_Request_xyz
{
public:
  explicit Init_SetAnalogIO_Request_xyz(::xarm_msgs::srv::SetAnalogIO_Request & msg)
  : msg_(msg)
  {}
  Init_SetAnalogIO_Request_tol_r xyz(::xarm_msgs::srv::SetAnalogIO_Request::_xyz_type arg)
  {
    msg_.xyz = std::move(arg);
    return Init_SetAnalogIO_Request_tol_r(msg_);
  }

private:
  ::xarm_msgs::srv::SetAnalogIO_Request msg_;
};

class Init_SetAnalogIO_Request_value
{
public:
  explicit Init_SetAnalogIO_Request_value(::xarm_msgs::srv::SetAnalogIO_Request & msg)
  : msg_(msg)
  {}
  Init_SetAnalogIO_Request_xyz value(::xarm_msgs::srv::SetAnalogIO_Request::_value_type arg)
  {
    msg_.value = std::move(arg);
    return Init_SetAnalogIO_Request_xyz(msg_);
  }

private:
  ::xarm_msgs::srv::SetAnalogIO_Request msg_;
};

class Init_SetAnalogIO_Request_ionum
{
public:
  Init_SetAnalogIO_Request_ionum()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SetAnalogIO_Request_value ionum(::xarm_msgs::srv::SetAnalogIO_Request::_ionum_type arg)
  {
    msg_.ionum = std::move(arg);
    return Init_SetAnalogIO_Request_value(msg_);
  }

private:
  ::xarm_msgs::srv::SetAnalogIO_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::SetAnalogIO_Request>()
{
  return xarm_msgs::srv::builder::Init_SetAnalogIO_Request_ionum();
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_SetAnalogIO_Response_message
{
public:
  explicit Init_SetAnalogIO_Response_message(::xarm_msgs::srv::SetAnalogIO_Response & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::SetAnalogIO_Response message(::xarm_msgs::srv::SetAnalogIO_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::SetAnalogIO_Response msg_;
};

class Init_SetAnalogIO_Response_ret
{
public:
  Init_SetAnalogIO_Response_ret()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SetAnalogIO_Response_message ret(::xarm_msgs::srv::SetAnalogIO_Response::_ret_type arg)
  {
    msg_.ret = std::move(arg);
    return Init_SetAnalogIO_Response_message(msg_);
  }

private:
  ::xarm_msgs::srv::SetAnalogIO_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::SetAnalogIO_Response>()
{
  return xarm_msgs::srv::builder::Init_SetAnalogIO_Response_ret();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__SET_ANALOG_IO__BUILDER_HPP_
