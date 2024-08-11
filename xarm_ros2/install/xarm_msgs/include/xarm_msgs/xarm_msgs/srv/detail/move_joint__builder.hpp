// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/MoveJoint.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__MOVE_JOINT__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__MOVE_JOINT__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/move_joint__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_MoveJoint_Request_relative
{
public:
  explicit Init_MoveJoint_Request_relative(::xarm_msgs::srv::MoveJoint_Request & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::MoveJoint_Request relative(::xarm_msgs::srv::MoveJoint_Request::_relative_type arg)
  {
    msg_.relative = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::MoveJoint_Request msg_;
};

class Init_MoveJoint_Request_radius
{
public:
  explicit Init_MoveJoint_Request_radius(::xarm_msgs::srv::MoveJoint_Request & msg)
  : msg_(msg)
  {}
  Init_MoveJoint_Request_relative radius(::xarm_msgs::srv::MoveJoint_Request::_radius_type arg)
  {
    msg_.radius = std::move(arg);
    return Init_MoveJoint_Request_relative(msg_);
  }

private:
  ::xarm_msgs::srv::MoveJoint_Request msg_;
};

class Init_MoveJoint_Request_timeout
{
public:
  explicit Init_MoveJoint_Request_timeout(::xarm_msgs::srv::MoveJoint_Request & msg)
  : msg_(msg)
  {}
  Init_MoveJoint_Request_radius timeout(::xarm_msgs::srv::MoveJoint_Request::_timeout_type arg)
  {
    msg_.timeout = std::move(arg);
    return Init_MoveJoint_Request_radius(msg_);
  }

private:
  ::xarm_msgs::srv::MoveJoint_Request msg_;
};

class Init_MoveJoint_Request_wait
{
public:
  explicit Init_MoveJoint_Request_wait(::xarm_msgs::srv::MoveJoint_Request & msg)
  : msg_(msg)
  {}
  Init_MoveJoint_Request_timeout wait(::xarm_msgs::srv::MoveJoint_Request::_wait_type arg)
  {
    msg_.wait = std::move(arg);
    return Init_MoveJoint_Request_timeout(msg_);
  }

private:
  ::xarm_msgs::srv::MoveJoint_Request msg_;
};

class Init_MoveJoint_Request_mvtime
{
public:
  explicit Init_MoveJoint_Request_mvtime(::xarm_msgs::srv::MoveJoint_Request & msg)
  : msg_(msg)
  {}
  Init_MoveJoint_Request_wait mvtime(::xarm_msgs::srv::MoveJoint_Request::_mvtime_type arg)
  {
    msg_.mvtime = std::move(arg);
    return Init_MoveJoint_Request_wait(msg_);
  }

private:
  ::xarm_msgs::srv::MoveJoint_Request msg_;
};

class Init_MoveJoint_Request_acc
{
public:
  explicit Init_MoveJoint_Request_acc(::xarm_msgs::srv::MoveJoint_Request & msg)
  : msg_(msg)
  {}
  Init_MoveJoint_Request_mvtime acc(::xarm_msgs::srv::MoveJoint_Request::_acc_type arg)
  {
    msg_.acc = std::move(arg);
    return Init_MoveJoint_Request_mvtime(msg_);
  }

private:
  ::xarm_msgs::srv::MoveJoint_Request msg_;
};

class Init_MoveJoint_Request_speed
{
public:
  explicit Init_MoveJoint_Request_speed(::xarm_msgs::srv::MoveJoint_Request & msg)
  : msg_(msg)
  {}
  Init_MoveJoint_Request_acc speed(::xarm_msgs::srv::MoveJoint_Request::_speed_type arg)
  {
    msg_.speed = std::move(arg);
    return Init_MoveJoint_Request_acc(msg_);
  }

private:
  ::xarm_msgs::srv::MoveJoint_Request msg_;
};

class Init_MoveJoint_Request_angles
{
public:
  Init_MoveJoint_Request_angles()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveJoint_Request_speed angles(::xarm_msgs::srv::MoveJoint_Request::_angles_type arg)
  {
    msg_.angles = std::move(arg);
    return Init_MoveJoint_Request_speed(msg_);
  }

private:
  ::xarm_msgs::srv::MoveJoint_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::MoveJoint_Request>()
{
  return xarm_msgs::srv::builder::Init_MoveJoint_Request_angles();
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_MoveJoint_Response_message
{
public:
  explicit Init_MoveJoint_Response_message(::xarm_msgs::srv::MoveJoint_Response & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::MoveJoint_Response message(::xarm_msgs::srv::MoveJoint_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::MoveJoint_Response msg_;
};

class Init_MoveJoint_Response_ret
{
public:
  Init_MoveJoint_Response_ret()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveJoint_Response_message ret(::xarm_msgs::srv::MoveJoint_Response::_ret_type arg)
  {
    msg_.ret = std::move(arg);
    return Init_MoveJoint_Response_message(msg_);
  }

private:
  ::xarm_msgs::srv::MoveJoint_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::MoveJoint_Response>()
{
  return xarm_msgs::srv::builder::Init_MoveJoint_Response_ret();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__MOVE_JOINT__BUILDER_HPP_
