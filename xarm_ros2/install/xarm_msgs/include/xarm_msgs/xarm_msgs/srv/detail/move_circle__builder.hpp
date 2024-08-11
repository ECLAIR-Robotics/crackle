// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/MoveCircle.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__MOVE_CIRCLE__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__MOVE_CIRCLE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/move_circle__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_MoveCircle_Request_is_axis_angle
{
public:
  explicit Init_MoveCircle_Request_is_axis_angle(::xarm_msgs::srv::MoveCircle_Request & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::MoveCircle_Request is_axis_angle(::xarm_msgs::srv::MoveCircle_Request::_is_axis_angle_type arg)
  {
    msg_.is_axis_angle = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCircle_Request msg_;
};

class Init_MoveCircle_Request_is_tool_coord
{
public:
  explicit Init_MoveCircle_Request_is_tool_coord(::xarm_msgs::srv::MoveCircle_Request & msg)
  : msg_(msg)
  {}
  Init_MoveCircle_Request_is_axis_angle is_tool_coord(::xarm_msgs::srv::MoveCircle_Request::_is_tool_coord_type arg)
  {
    msg_.is_tool_coord = std::move(arg);
    return Init_MoveCircle_Request_is_axis_angle(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCircle_Request msg_;
};

class Init_MoveCircle_Request_timeout
{
public:
  explicit Init_MoveCircle_Request_timeout(::xarm_msgs::srv::MoveCircle_Request & msg)
  : msg_(msg)
  {}
  Init_MoveCircle_Request_is_tool_coord timeout(::xarm_msgs::srv::MoveCircle_Request::_timeout_type arg)
  {
    msg_.timeout = std::move(arg);
    return Init_MoveCircle_Request_is_tool_coord(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCircle_Request msg_;
};

class Init_MoveCircle_Request_wait
{
public:
  explicit Init_MoveCircle_Request_wait(::xarm_msgs::srv::MoveCircle_Request & msg)
  : msg_(msg)
  {}
  Init_MoveCircle_Request_timeout wait(::xarm_msgs::srv::MoveCircle_Request::_wait_type arg)
  {
    msg_.wait = std::move(arg);
    return Init_MoveCircle_Request_timeout(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCircle_Request msg_;
};

class Init_MoveCircle_Request_mvtime
{
public:
  explicit Init_MoveCircle_Request_mvtime(::xarm_msgs::srv::MoveCircle_Request & msg)
  : msg_(msg)
  {}
  Init_MoveCircle_Request_wait mvtime(::xarm_msgs::srv::MoveCircle_Request::_mvtime_type arg)
  {
    msg_.mvtime = std::move(arg);
    return Init_MoveCircle_Request_wait(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCircle_Request msg_;
};

class Init_MoveCircle_Request_acc
{
public:
  explicit Init_MoveCircle_Request_acc(::xarm_msgs::srv::MoveCircle_Request & msg)
  : msg_(msg)
  {}
  Init_MoveCircle_Request_mvtime acc(::xarm_msgs::srv::MoveCircle_Request::_acc_type arg)
  {
    msg_.acc = std::move(arg);
    return Init_MoveCircle_Request_mvtime(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCircle_Request msg_;
};

class Init_MoveCircle_Request_speed
{
public:
  explicit Init_MoveCircle_Request_speed(::xarm_msgs::srv::MoveCircle_Request & msg)
  : msg_(msg)
  {}
  Init_MoveCircle_Request_acc speed(::xarm_msgs::srv::MoveCircle_Request::_speed_type arg)
  {
    msg_.speed = std::move(arg);
    return Init_MoveCircle_Request_acc(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCircle_Request msg_;
};

class Init_MoveCircle_Request_percent
{
public:
  explicit Init_MoveCircle_Request_percent(::xarm_msgs::srv::MoveCircle_Request & msg)
  : msg_(msg)
  {}
  Init_MoveCircle_Request_speed percent(::xarm_msgs::srv::MoveCircle_Request::_percent_type arg)
  {
    msg_.percent = std::move(arg);
    return Init_MoveCircle_Request_speed(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCircle_Request msg_;
};

class Init_MoveCircle_Request_pose2
{
public:
  explicit Init_MoveCircle_Request_pose2(::xarm_msgs::srv::MoveCircle_Request & msg)
  : msg_(msg)
  {}
  Init_MoveCircle_Request_percent pose2(::xarm_msgs::srv::MoveCircle_Request::_pose2_type arg)
  {
    msg_.pose2 = std::move(arg);
    return Init_MoveCircle_Request_percent(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCircle_Request msg_;
};

class Init_MoveCircle_Request_pose1
{
public:
  Init_MoveCircle_Request_pose1()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveCircle_Request_pose2 pose1(::xarm_msgs::srv::MoveCircle_Request::_pose1_type arg)
  {
    msg_.pose1 = std::move(arg);
    return Init_MoveCircle_Request_pose2(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCircle_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::MoveCircle_Request>()
{
  return xarm_msgs::srv::builder::Init_MoveCircle_Request_pose1();
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_MoveCircle_Response_message
{
public:
  explicit Init_MoveCircle_Response_message(::xarm_msgs::srv::MoveCircle_Response & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::MoveCircle_Response message(::xarm_msgs::srv::MoveCircle_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCircle_Response msg_;
};

class Init_MoveCircle_Response_ret
{
public:
  Init_MoveCircle_Response_ret()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveCircle_Response_message ret(::xarm_msgs::srv::MoveCircle_Response::_ret_type arg)
  {
    msg_.ret = std::move(arg);
    return Init_MoveCircle_Response_message(msg_);
  }

private:
  ::xarm_msgs::srv::MoveCircle_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::MoveCircle_Response>()
{
  return xarm_msgs::srv::builder::Init_MoveCircle_Response_ret();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__MOVE_CIRCLE__BUILDER_HPP_
