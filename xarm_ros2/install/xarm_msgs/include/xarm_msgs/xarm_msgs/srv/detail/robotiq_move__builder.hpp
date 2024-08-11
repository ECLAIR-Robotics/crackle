// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/RobotiqMove.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__ROBOTIQ_MOVE__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__ROBOTIQ_MOVE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/robotiq_move__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_RobotiqMove_Request_timeout
{
public:
  explicit Init_RobotiqMove_Request_timeout(::xarm_msgs::srv::RobotiqMove_Request & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::RobotiqMove_Request timeout(::xarm_msgs::srv::RobotiqMove_Request::_timeout_type arg)
  {
    msg_.timeout = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::RobotiqMove_Request msg_;
};

class Init_RobotiqMove_Request_wait
{
public:
  explicit Init_RobotiqMove_Request_wait(::xarm_msgs::srv::RobotiqMove_Request & msg)
  : msg_(msg)
  {}
  Init_RobotiqMove_Request_timeout wait(::xarm_msgs::srv::RobotiqMove_Request::_wait_type arg)
  {
    msg_.wait = std::move(arg);
    return Init_RobotiqMove_Request_timeout(msg_);
  }

private:
  ::xarm_msgs::srv::RobotiqMove_Request msg_;
};

class Init_RobotiqMove_Request_force
{
public:
  explicit Init_RobotiqMove_Request_force(::xarm_msgs::srv::RobotiqMove_Request & msg)
  : msg_(msg)
  {}
  Init_RobotiqMove_Request_wait force(::xarm_msgs::srv::RobotiqMove_Request::_force_type arg)
  {
    msg_.force = std::move(arg);
    return Init_RobotiqMove_Request_wait(msg_);
  }

private:
  ::xarm_msgs::srv::RobotiqMove_Request msg_;
};

class Init_RobotiqMove_Request_speed
{
public:
  explicit Init_RobotiqMove_Request_speed(::xarm_msgs::srv::RobotiqMove_Request & msg)
  : msg_(msg)
  {}
  Init_RobotiqMove_Request_force speed(::xarm_msgs::srv::RobotiqMove_Request::_speed_type arg)
  {
    msg_.speed = std::move(arg);
    return Init_RobotiqMove_Request_force(msg_);
  }

private:
  ::xarm_msgs::srv::RobotiqMove_Request msg_;
};

class Init_RobotiqMove_Request_pos
{
public:
  Init_RobotiqMove_Request_pos()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_RobotiqMove_Request_speed pos(::xarm_msgs::srv::RobotiqMove_Request::_pos_type arg)
  {
    msg_.pos = std::move(arg);
    return Init_RobotiqMove_Request_speed(msg_);
  }

private:
  ::xarm_msgs::srv::RobotiqMove_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::RobotiqMove_Request>()
{
  return xarm_msgs::srv::builder::Init_RobotiqMove_Request_pos();
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_RobotiqMove_Response_ret_data
{
public:
  explicit Init_RobotiqMove_Response_ret_data(::xarm_msgs::srv::RobotiqMove_Response & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::RobotiqMove_Response ret_data(::xarm_msgs::srv::RobotiqMove_Response::_ret_data_type arg)
  {
    msg_.ret_data = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::RobotiqMove_Response msg_;
};

class Init_RobotiqMove_Response_message
{
public:
  explicit Init_RobotiqMove_Response_message(::xarm_msgs::srv::RobotiqMove_Response & msg)
  : msg_(msg)
  {}
  Init_RobotiqMove_Response_ret_data message(::xarm_msgs::srv::RobotiqMove_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return Init_RobotiqMove_Response_ret_data(msg_);
  }

private:
  ::xarm_msgs::srv::RobotiqMove_Response msg_;
};

class Init_RobotiqMove_Response_ret
{
public:
  Init_RobotiqMove_Response_ret()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_RobotiqMove_Response_message ret(::xarm_msgs::srv::RobotiqMove_Response::_ret_type arg)
  {
    msg_.ret = std::move(arg);
    return Init_RobotiqMove_Response_message(msg_);
  }

private:
  ::xarm_msgs::srv::RobotiqMove_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::RobotiqMove_Response>()
{
  return xarm_msgs::srv::builder::Init_RobotiqMove_Response_ret();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__ROBOTIQ_MOVE__BUILDER_HPP_
