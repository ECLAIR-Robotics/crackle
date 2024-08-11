// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/SetTcpLoad.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__SET_TCP_LOAD__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__SET_TCP_LOAD__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/set_tcp_load__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_SetTcpLoad_Request_center_of_gravity
{
public:
  explicit Init_SetTcpLoad_Request_center_of_gravity(::xarm_msgs::srv::SetTcpLoad_Request & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::SetTcpLoad_Request center_of_gravity(::xarm_msgs::srv::SetTcpLoad_Request::_center_of_gravity_type arg)
  {
    msg_.center_of_gravity = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::SetTcpLoad_Request msg_;
};

class Init_SetTcpLoad_Request_weight
{
public:
  Init_SetTcpLoad_Request_weight()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SetTcpLoad_Request_center_of_gravity weight(::xarm_msgs::srv::SetTcpLoad_Request::_weight_type arg)
  {
    msg_.weight = std::move(arg);
    return Init_SetTcpLoad_Request_center_of_gravity(msg_);
  }

private:
  ::xarm_msgs::srv::SetTcpLoad_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::SetTcpLoad_Request>()
{
  return xarm_msgs::srv::builder::Init_SetTcpLoad_Request_weight();
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_SetTcpLoad_Response_message
{
public:
  explicit Init_SetTcpLoad_Response_message(::xarm_msgs::srv::SetTcpLoad_Response & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::SetTcpLoad_Response message(::xarm_msgs::srv::SetTcpLoad_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::SetTcpLoad_Response msg_;
};

class Init_SetTcpLoad_Response_ret
{
public:
  Init_SetTcpLoad_Response_ret()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SetTcpLoad_Response_message ret(::xarm_msgs::srv::SetTcpLoad_Response::_ret_type arg)
  {
    msg_.ret = std::move(arg);
    return Init_SetTcpLoad_Response_message(msg_);
  }

private:
  ::xarm_msgs::srv::SetTcpLoad_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::SetTcpLoad_Response>()
{
  return xarm_msgs::srv::builder::Init_SetTcpLoad_Response_ret();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__SET_TCP_LOAD__BUILDER_HPP_
