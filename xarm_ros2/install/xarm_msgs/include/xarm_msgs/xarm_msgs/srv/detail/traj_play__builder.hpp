// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/TrajPlay.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__TRAJ_PLAY__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__TRAJ_PLAY__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/traj_play__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_TrajPlay_Request_wait
{
public:
  explicit Init_TrajPlay_Request_wait(::xarm_msgs::srv::TrajPlay_Request & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::TrajPlay_Request wait(::xarm_msgs::srv::TrajPlay_Request::_wait_type arg)
  {
    msg_.wait = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::TrajPlay_Request msg_;
};

class Init_TrajPlay_Request_double_speed
{
public:
  explicit Init_TrajPlay_Request_double_speed(::xarm_msgs::srv::TrajPlay_Request & msg)
  : msg_(msg)
  {}
  Init_TrajPlay_Request_wait double_speed(::xarm_msgs::srv::TrajPlay_Request::_double_speed_type arg)
  {
    msg_.double_speed = std::move(arg);
    return Init_TrajPlay_Request_wait(msg_);
  }

private:
  ::xarm_msgs::srv::TrajPlay_Request msg_;
};

class Init_TrajPlay_Request_times
{
public:
  explicit Init_TrajPlay_Request_times(::xarm_msgs::srv::TrajPlay_Request & msg)
  : msg_(msg)
  {}
  Init_TrajPlay_Request_double_speed times(::xarm_msgs::srv::TrajPlay_Request::_times_type arg)
  {
    msg_.times = std::move(arg);
    return Init_TrajPlay_Request_double_speed(msg_);
  }

private:
  ::xarm_msgs::srv::TrajPlay_Request msg_;
};

class Init_TrajPlay_Request_filename
{
public:
  Init_TrajPlay_Request_filename()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_TrajPlay_Request_times filename(::xarm_msgs::srv::TrajPlay_Request::_filename_type arg)
  {
    msg_.filename = std::move(arg);
    return Init_TrajPlay_Request_times(msg_);
  }

private:
  ::xarm_msgs::srv::TrajPlay_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::TrajPlay_Request>()
{
  return xarm_msgs::srv::builder::Init_TrajPlay_Request_filename();
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_TrajPlay_Response_message
{
public:
  explicit Init_TrajPlay_Response_message(::xarm_msgs::srv::TrajPlay_Response & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::TrajPlay_Response message(::xarm_msgs::srv::TrajPlay_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::TrajPlay_Response msg_;
};

class Init_TrajPlay_Response_ret
{
public:
  Init_TrajPlay_Response_ret()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_TrajPlay_Response_message ret(::xarm_msgs::srv::TrajPlay_Response::_ret_type arg)
  {
    msg_.ret = std::move(arg);
    return Init_TrajPlay_Response_message(msg_);
  }

private:
  ::xarm_msgs::srv::TrajPlay_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::TrajPlay_Response>()
{
  return xarm_msgs::srv::builder::Init_TrajPlay_Response_ret();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__TRAJ_PLAY__BUILDER_HPP_
