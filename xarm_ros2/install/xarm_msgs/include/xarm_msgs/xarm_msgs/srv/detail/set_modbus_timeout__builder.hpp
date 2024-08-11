// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/SetModbusTimeout.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__SET_MODBUS_TIMEOUT__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__SET_MODBUS_TIMEOUT__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/set_modbus_timeout__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_SetModbusTimeout_Request_is_transparent_transmission
{
public:
  explicit Init_SetModbusTimeout_Request_is_transparent_transmission(::xarm_msgs::srv::SetModbusTimeout_Request & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::SetModbusTimeout_Request is_transparent_transmission(::xarm_msgs::srv::SetModbusTimeout_Request::_is_transparent_transmission_type arg)
  {
    msg_.is_transparent_transmission = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::SetModbusTimeout_Request msg_;
};

class Init_SetModbusTimeout_Request_timeout
{
public:
  Init_SetModbusTimeout_Request_timeout()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SetModbusTimeout_Request_is_transparent_transmission timeout(::xarm_msgs::srv::SetModbusTimeout_Request::_timeout_type arg)
  {
    msg_.timeout = std::move(arg);
    return Init_SetModbusTimeout_Request_is_transparent_transmission(msg_);
  }

private:
  ::xarm_msgs::srv::SetModbusTimeout_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::SetModbusTimeout_Request>()
{
  return xarm_msgs::srv::builder::Init_SetModbusTimeout_Request_timeout();
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_SetModbusTimeout_Response_message
{
public:
  explicit Init_SetModbusTimeout_Response_message(::xarm_msgs::srv::SetModbusTimeout_Response & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::SetModbusTimeout_Response message(::xarm_msgs::srv::SetModbusTimeout_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::SetModbusTimeout_Response msg_;
};

class Init_SetModbusTimeout_Response_ret
{
public:
  Init_SetModbusTimeout_Response_ret()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SetModbusTimeout_Response_message ret(::xarm_msgs::srv::SetModbusTimeout_Response::_ret_type arg)
  {
    msg_.ret = std::move(arg);
    return Init_SetModbusTimeout_Response_message(msg_);
  }

private:
  ::xarm_msgs::srv::SetModbusTimeout_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::SetModbusTimeout_Response>()
{
  return xarm_msgs::srv::builder::Init_SetModbusTimeout_Response_ret();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__SET_MODBUS_TIMEOUT__BUILDER_HPP_
