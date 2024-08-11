// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/MoveVelocity.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__MOVE_VELOCITY__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__MOVE_VELOCITY__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/move_velocity__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_MoveVelocity_Request_duration
{
public:
  explicit Init_MoveVelocity_Request_duration(::xarm_msgs::srv::MoveVelocity_Request & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::MoveVelocity_Request duration(::xarm_msgs::srv::MoveVelocity_Request::_duration_type arg)
  {
    msg_.duration = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::MoveVelocity_Request msg_;
};

class Init_MoveVelocity_Request_is_tool_coord
{
public:
  explicit Init_MoveVelocity_Request_is_tool_coord(::xarm_msgs::srv::MoveVelocity_Request & msg)
  : msg_(msg)
  {}
  Init_MoveVelocity_Request_duration is_tool_coord(::xarm_msgs::srv::MoveVelocity_Request::_is_tool_coord_type arg)
  {
    msg_.is_tool_coord = std::move(arg);
    return Init_MoveVelocity_Request_duration(msg_);
  }

private:
  ::xarm_msgs::srv::MoveVelocity_Request msg_;
};

class Init_MoveVelocity_Request_is_sync
{
public:
  explicit Init_MoveVelocity_Request_is_sync(::xarm_msgs::srv::MoveVelocity_Request & msg)
  : msg_(msg)
  {}
  Init_MoveVelocity_Request_is_tool_coord is_sync(::xarm_msgs::srv::MoveVelocity_Request::_is_sync_type arg)
  {
    msg_.is_sync = std::move(arg);
    return Init_MoveVelocity_Request_is_tool_coord(msg_);
  }

private:
  ::xarm_msgs::srv::MoveVelocity_Request msg_;
};

class Init_MoveVelocity_Request_speeds
{
public:
  Init_MoveVelocity_Request_speeds()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveVelocity_Request_is_sync speeds(::xarm_msgs::srv::MoveVelocity_Request::_speeds_type arg)
  {
    msg_.speeds = std::move(arg);
    return Init_MoveVelocity_Request_is_sync(msg_);
  }

private:
  ::xarm_msgs::srv::MoveVelocity_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::MoveVelocity_Request>()
{
  return xarm_msgs::srv::builder::Init_MoveVelocity_Request_speeds();
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_MoveVelocity_Response_message
{
public:
  explicit Init_MoveVelocity_Response_message(::xarm_msgs::srv::MoveVelocity_Response & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::MoveVelocity_Response message(::xarm_msgs::srv::MoveVelocity_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::MoveVelocity_Response msg_;
};

class Init_MoveVelocity_Response_ret
{
public:
  Init_MoveVelocity_Response_ret()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveVelocity_Response_message ret(::xarm_msgs::srv::MoveVelocity_Response::_ret_type arg)
  {
    msg_.ret = std::move(arg);
    return Init_MoveVelocity_Response_message(msg_);
  }

private:
  ::xarm_msgs::srv::MoveVelocity_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::MoveVelocity_Response>()
{
  return xarm_msgs::srv::builder::Init_MoveVelocity_Response_ret();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__MOVE_VELOCITY__BUILDER_HPP_
