// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/MoveCartesian.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__MOVE_CARTESIAN__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__MOVE_CARTESIAN__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/move_cartesian__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_MoveCartesian_Request_motion_type
{
public:
  explicit Init_MoveCartesian_Request_motion_type(::xarm_msgs::srv::MoveCartesian_Request & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::MoveCartesian_Request motion_type(::xarm_msgs::srv::MoveCartesian_Request::_motion_type_type arg)
  {
    msg_.motion_type = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCartesian_Request msg_;
};

class Init_MoveCartesian_Request_relative
{
public:
  explicit Init_MoveCartesian_Request_relative(::xarm_msgs::srv::MoveCartesian_Request & msg)
  : msg_(msg)
  {}
  Init_MoveCartesian_Request_motion_type relative(::xarm_msgs::srv::MoveCartesian_Request::_relative_type arg)
  {
    msg_.relative = std::move(arg);
    return Init_MoveCartesian_Request_motion_type(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCartesian_Request msg_;
};

class Init_MoveCartesian_Request_is_tool_coord
{
public:
  explicit Init_MoveCartesian_Request_is_tool_coord(::xarm_msgs::srv::MoveCartesian_Request & msg)
  : msg_(msg)
  {}
  Init_MoveCartesian_Request_relative is_tool_coord(::xarm_msgs::srv::MoveCartesian_Request::_is_tool_coord_type arg)
  {
    msg_.is_tool_coord = std::move(arg);
    return Init_MoveCartesian_Request_relative(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCartesian_Request msg_;
};

class Init_MoveCartesian_Request_radius
{
public:
  explicit Init_MoveCartesian_Request_radius(::xarm_msgs::srv::MoveCartesian_Request & msg)
  : msg_(msg)
  {}
  Init_MoveCartesian_Request_is_tool_coord radius(::xarm_msgs::srv::MoveCartesian_Request::_radius_type arg)
  {
    msg_.radius = std::move(arg);
    return Init_MoveCartesian_Request_is_tool_coord(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCartesian_Request msg_;
};

class Init_MoveCartesian_Request_timeout
{
public:
  explicit Init_MoveCartesian_Request_timeout(::xarm_msgs::srv::MoveCartesian_Request & msg)
  : msg_(msg)
  {}
  Init_MoveCartesian_Request_radius timeout(::xarm_msgs::srv::MoveCartesian_Request::_timeout_type arg)
  {
    msg_.timeout = std::move(arg);
    return Init_MoveCartesian_Request_radius(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCartesian_Request msg_;
};

class Init_MoveCartesian_Request_wait
{
public:
  explicit Init_MoveCartesian_Request_wait(::xarm_msgs::srv::MoveCartesian_Request & msg)
  : msg_(msg)
  {}
  Init_MoveCartesian_Request_timeout wait(::xarm_msgs::srv::MoveCartesian_Request::_wait_type arg)
  {
    msg_.wait = std::move(arg);
    return Init_MoveCartesian_Request_timeout(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCartesian_Request msg_;
};

class Init_MoveCartesian_Request_mvtime
{
public:
  explicit Init_MoveCartesian_Request_mvtime(::xarm_msgs::srv::MoveCartesian_Request & msg)
  : msg_(msg)
  {}
  Init_MoveCartesian_Request_wait mvtime(::xarm_msgs::srv::MoveCartesian_Request::_mvtime_type arg)
  {
    msg_.mvtime = std::move(arg);
    return Init_MoveCartesian_Request_wait(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCartesian_Request msg_;
};

class Init_MoveCartesian_Request_acc
{
public:
  explicit Init_MoveCartesian_Request_acc(::xarm_msgs::srv::MoveCartesian_Request & msg)
  : msg_(msg)
  {}
  Init_MoveCartesian_Request_mvtime acc(::xarm_msgs::srv::MoveCartesian_Request::_acc_type arg)
  {
    msg_.acc = std::move(arg);
    return Init_MoveCartesian_Request_mvtime(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCartesian_Request msg_;
};

class Init_MoveCartesian_Request_speed
{
public:
  explicit Init_MoveCartesian_Request_speed(::xarm_msgs::srv::MoveCartesian_Request & msg)
  : msg_(msg)
  {}
  Init_MoveCartesian_Request_acc speed(::xarm_msgs::srv::MoveCartesian_Request::_speed_type arg)
  {
    msg_.speed = std::move(arg);
    return Init_MoveCartesian_Request_acc(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCartesian_Request msg_;
};

class Init_MoveCartesian_Request_pose
{
public:
  Init_MoveCartesian_Request_pose()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveCartesian_Request_speed pose(::xarm_msgs::srv::MoveCartesian_Request::_pose_type arg)
  {
    msg_.pose = std::move(arg);
    return Init_MoveCartesian_Request_speed(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCartesian_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::MoveCartesian_Request>()
{
  return xarm_msgs::srv::builder::Init_MoveCartesian_Request_pose();
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_MoveCartesian_Response_message
{
public:
  explicit Init_MoveCartesian_Response_message(::xarm_msgs::srv::MoveCartesian_Response & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::MoveCartesian_Response message(::xarm_msgs::srv::MoveCartesian_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCartesian_Response msg_;
};

class Init_MoveCartesian_Response_ret
{
public:
  Init_MoveCartesian_Response_ret()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveCartesian_Response_message ret(::xarm_msgs::srv::MoveCartesian_Response::_ret_type arg)
  {
    msg_.ret = std::move(arg);
    return Init_MoveCartesian_Response_message(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCartesian_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::MoveCartesian_Response>()
{
  return xarm_msgs::srv::builder::Init_MoveCartesian_Response_ret();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__MOVE_CARTESIAN__BUILDER_HPP_
