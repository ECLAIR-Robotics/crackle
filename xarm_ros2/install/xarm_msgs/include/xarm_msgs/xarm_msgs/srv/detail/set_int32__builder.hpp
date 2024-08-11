// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/SetInt32.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__SET_INT32__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__SET_INT32__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/set_int32__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_SetInt32_Request_data
{
public:
  Init_SetInt32_Request_data()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::xarm_msgs::srv::SetInt32_Request data(::xarm_msgs::srv::SetInt32_Request::_data_type arg)
  {
    msg_.data = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::SetInt32_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::SetInt32_Request>()
{
  return xarm_msgs::srv::builder::Init_SetInt32_Request_data();
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_SetInt32_Response_message
{
public:
  explicit Init_SetInt32_Response_message(::xarm_msgs::srv::SetInt32_Response & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::SetInt32_Response message(::xarm_msgs::srv::SetInt32_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::SetInt32_Response msg_;
};

class Init_SetInt32_Response_ret
{
public:
  Init_SetInt32_Response_ret()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SetInt32_Response_message ret(::xarm_msgs::srv::SetInt32_Response::_ret_type arg)
  {
    msg_.ret = std::move(arg);
    return Init_SetInt32_Response_message(msg_);
  }

private:
  ::xarm_msgs::srv::SetInt32_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::SetInt32_Response>()
{
  return xarm_msgs::srv::builder::Init_SetInt32_Response_ret();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__SET_INT32__BUILDER_HPP_
