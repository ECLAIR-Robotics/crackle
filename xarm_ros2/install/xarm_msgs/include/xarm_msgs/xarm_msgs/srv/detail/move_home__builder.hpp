// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/MoveHome.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__MOVE_HOME__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__MOVE_HOME__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/move_home__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_MoveHome_Request_timeout
{
public:
  explicit Init_MoveHome_Request_timeout(::xarm_msgs::srv::MoveHome_Request & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::MoveHome_Request timeout(::xarm_msgs::srv::MoveHome_Request::_timeout_type arg)
  {
    msg_.timeout = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::MoveHome_Request msg_;
};

class Init_MoveHome_Request_wait
{
public:
  explicit Init_MoveHome_Request_wait(::xarm_msgs::srv::MoveHome_Request & msg)
  : msg_(msg)
  {}
  Init_MoveHome_Request_timeout wait(::xarm_msgs::srv::MoveHome_Request::_wait_type arg)
  {
    msg_.wait = std::move(arg);
    return Init_MoveHome_Request_timeout(msg_);
  }

private:
  ::xarm_msgs::srv::MoveHome_Request msg_;
};

class Init_MoveHome_Request_mvtime
{
public:
  explicit Init_MoveHome_Request_mvtime(::xarm_msgs::srv::MoveHome_Request & msg)
  : msg_(msg)
  {}
  Init_MoveHome_Request_wait mvtime(::xarm_msgs::srv::MoveHome_Request::_mvtime_type arg)
  {
    msg_.mvtime = std::move(arg);
    return Init_MoveHome_Request_wait(msg_);
  }

private:
  ::xarm_msgs::srv::MoveHome_Request msg_;
};

class Init_MoveHome_Request_acc
{
public:
  explicit Init_MoveHome_Request_acc(::xarm_msgs::srv::MoveHome_Request & msg)
  : msg_(msg)
  {}
  Init_MoveHome_Request_mvtime acc(::xarm_msgs::srv::MoveHome_Request::_acc_type arg)
  {
    msg_.acc = std::move(arg);
    return Init_MoveHome_Request_mvtime(msg_);
  }

private:
  ::xarm_msgs::srv::MoveHome_Request msg_;
};

class Init_MoveHome_Request_speed
{
public:
  Init_MoveHome_Request_speed()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveHome_Request_acc speed(::xarm_msgs::srv::MoveHome_Request::_speed_type arg)
  {
    msg_.speed = std::move(arg);
    return Init_MoveHome_Request_acc(msg_);
  }

private:
  ::xarm_msgs::srv::MoveHome_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::MoveHome_Request>()
{
  return xarm_msgs::srv::builder::Init_MoveHome_Request_speed();
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_MoveHome_Response_message
{
public:
  explicit Init_MoveHome_Response_message(::xarm_msgs::srv::MoveHome_Response & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::MoveHome_Response message(::xarm_msgs::srv::MoveHome_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::MoveHome_Response msg_;
};

class Init_MoveHome_Response_ret
{
public:
  Init_MoveHome_Response_ret()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveHome_Response_message ret(::xarm_msgs::srv::MoveHome_Response::_ret_type arg)
  {
    msg_.ret = std::move(arg);
    return Init_MoveHome_Response_message(msg_);
  }

private:
  ::xarm_msgs::srv::MoveHome_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::MoveHome_Response>()
{
  return xarm_msgs::srv::builder::Init_MoveHome_Response_ret();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__MOVE_HOME__BUILDER_HPP_
