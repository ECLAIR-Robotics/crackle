// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/GetAnalogIO.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__GET_ANALOG_IO__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__GET_ANALOG_IO__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/get_analog_io__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_GetAnalogIO_Request_ionum
{
public:
  Init_GetAnalogIO_Request_ionum()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::xarm_msgs::srv::GetAnalogIO_Request ionum(::xarm_msgs::srv::GetAnalogIO_Request::_ionum_type arg)
  {
    msg_.ionum = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::GetAnalogIO_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::GetAnalogIO_Request>()
{
  return xarm_msgs::srv::builder::Init_GetAnalogIO_Request_ionum();
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_GetAnalogIO_Response_data
{
public:
  explicit Init_GetAnalogIO_Response_data(::xarm_msgs::srv::GetAnalogIO_Response & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::GetAnalogIO_Response data(::xarm_msgs::srv::GetAnalogIO_Response::_data_type arg)
  {
    msg_.data = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::GetAnalogIO_Response msg_;
};

class Init_GetAnalogIO_Response_message
{
public:
  explicit Init_GetAnalogIO_Response_message(::xarm_msgs::srv::GetAnalogIO_Response & msg)
  : msg_(msg)
  {}
  Init_GetAnalogIO_Response_data message(::xarm_msgs::srv::GetAnalogIO_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return Init_GetAnalogIO_Response_data(msg_);
  }

private:
  ::xarm_msgs::srv::GetAnalogIO_Response msg_;
};

class Init_GetAnalogIO_Response_ret
{
public:
  Init_GetAnalogIO_Response_ret()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_GetAnalogIO_Response_message ret(::xarm_msgs::srv::GetAnalogIO_Response::_ret_type arg)
  {
    msg_.ret = std::move(arg);
    return Init_GetAnalogIO_Response_message(msg_);
  }

private:
  ::xarm_msgs::srv::GetAnalogIO_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::GetAnalogIO_Response>()
{
  return xarm_msgs::srv::builder::Init_GetAnalogIO_Response_ret();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__GET_ANALOG_IO__BUILDER_HPP_
