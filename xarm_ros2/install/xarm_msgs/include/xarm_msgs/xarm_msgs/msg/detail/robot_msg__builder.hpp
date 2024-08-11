// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:msg/RobotMsg.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__MSG__DETAIL__ROBOT_MSG__BUILDER_HPP_
#define XARM_MSGS__MSG__DETAIL__ROBOT_MSG__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/msg/detail/robot_msg__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace msg
{

namespace builder
{

class Init_RobotMsg_offset
{
public:
  explicit Init_RobotMsg_offset(::xarm_msgs::msg::RobotMsg & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::msg::RobotMsg offset(::xarm_msgs::msg::RobotMsg::_offset_type arg)
  {
    msg_.offset = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::msg::RobotMsg msg_;
};

class Init_RobotMsg_pose
{
public:
  explicit Init_RobotMsg_pose(::xarm_msgs::msg::RobotMsg & msg)
  : msg_(msg)
  {}
  Init_RobotMsg_offset pose(::xarm_msgs::msg::RobotMsg::_pose_type arg)
  {
    msg_.pose = std::move(arg);
    return Init_RobotMsg_offset(msg_);
  }

private:
  ::xarm_msgs::msg::RobotMsg msg_;
};

class Init_RobotMsg_angle
{
public:
  explicit Init_RobotMsg_angle(::xarm_msgs::msg::RobotMsg & msg)
  : msg_(msg)
  {}
  Init_RobotMsg_pose angle(::xarm_msgs::msg::RobotMsg::_angle_type arg)
  {
    msg_.angle = std::move(arg);
    return Init_RobotMsg_pose(msg_);
  }

private:
  ::xarm_msgs::msg::RobotMsg msg_;
};

class Init_RobotMsg_warn
{
public:
  explicit Init_RobotMsg_warn(::xarm_msgs::msg::RobotMsg & msg)
  : msg_(msg)
  {}
  Init_RobotMsg_angle warn(::xarm_msgs::msg::RobotMsg::_warn_type arg)
  {
    msg_.warn = std::move(arg);
    return Init_RobotMsg_angle(msg_);
  }

private:
  ::xarm_msgs::msg::RobotMsg msg_;
};

class Init_RobotMsg_err
{
public:
  explicit Init_RobotMsg_err(::xarm_msgs::msg::RobotMsg & msg)
  : msg_(msg)
  {}
  Init_RobotMsg_warn err(::xarm_msgs::msg::RobotMsg::_err_type arg)
  {
    msg_.err = std::move(arg);
    return Init_RobotMsg_warn(msg_);
  }

private:
  ::xarm_msgs::msg::RobotMsg msg_;
};

class Init_RobotMsg_mt_able
{
public:
  explicit Init_RobotMsg_mt_able(::xarm_msgs::msg::RobotMsg & msg)
  : msg_(msg)
  {}
  Init_RobotMsg_err mt_able(::xarm_msgs::msg::RobotMsg::_mt_able_type arg)
  {
    msg_.mt_able = std::move(arg);
    return Init_RobotMsg_err(msg_);
  }

private:
  ::xarm_msgs::msg::RobotMsg msg_;
};

class Init_RobotMsg_mt_brake
{
public:
  explicit Init_RobotMsg_mt_brake(::xarm_msgs::msg::RobotMsg & msg)
  : msg_(msg)
  {}
  Init_RobotMsg_mt_able mt_brake(::xarm_msgs::msg::RobotMsg::_mt_brake_type arg)
  {
    msg_.mt_brake = std::move(arg);
    return Init_RobotMsg_mt_able(msg_);
  }

private:
  ::xarm_msgs::msg::RobotMsg msg_;
};

class Init_RobotMsg_cmdnum
{
public:
  explicit Init_RobotMsg_cmdnum(::xarm_msgs::msg::RobotMsg & msg)
  : msg_(msg)
  {}
  Init_RobotMsg_mt_brake cmdnum(::xarm_msgs::msg::RobotMsg::_cmdnum_type arg)
  {
    msg_.cmdnum = std::move(arg);
    return Init_RobotMsg_mt_brake(msg_);
  }

private:
  ::xarm_msgs::msg::RobotMsg msg_;
};

class Init_RobotMsg_mode
{
public:
  explicit Init_RobotMsg_mode(::xarm_msgs::msg::RobotMsg & msg)
  : msg_(msg)
  {}
  Init_RobotMsg_cmdnum mode(::xarm_msgs::msg::RobotMsg::_mode_type arg)
  {
    msg_.mode = std::move(arg);
    return Init_RobotMsg_cmdnum(msg_);
  }

private:
  ::xarm_msgs::msg::RobotMsg msg_;
};

class Init_RobotMsg_state
{
public:
  explicit Init_RobotMsg_state(::xarm_msgs::msg::RobotMsg & msg)
  : msg_(msg)
  {}
  Init_RobotMsg_mode state(::xarm_msgs::msg::RobotMsg::_state_type arg)
  {
    msg_.state = std::move(arg);
    return Init_RobotMsg_mode(msg_);
  }

private:
  ::xarm_msgs::msg::RobotMsg msg_;
};

class Init_RobotMsg_header
{
public:
  Init_RobotMsg_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_RobotMsg_state header(::xarm_msgs::msg::RobotMsg::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_RobotMsg_state(msg_);
  }

private:
  ::xarm_msgs::msg::RobotMsg msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::msg::RobotMsg>()
{
  return xarm_msgs::msg::builder::Init_RobotMsg_header();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__MSG__DETAIL__ROBOT_MSG__BUILDER_HPP_
