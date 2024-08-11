// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/TrajCtrl.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__TRAJ_CTRL__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__TRAJ_CTRL__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/traj_ctrl__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_TrajCtrl_Request_timeout
{
public:
  explicit Init_TrajCtrl_Request_timeout(::xarm_msgs::srv::TrajCtrl_Request & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::TrajCtrl_Request timeout(::xarm_msgs::srv::TrajCtrl_Request::_timeout_type arg)
  {
    msg_.timeout = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::TrajCtrl_Request msg_;
};

class Init_TrajCtrl_Request_filename
{
public:
  Init_TrajCtrl_Request_filename()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_TrajCtrl_Request_timeout filename(::xarm_msgs::srv::TrajCtrl_Request::_filename_type arg)
  {
    msg_.filename = std::move(arg);
    return Init_TrajCtrl_Request_timeout(msg_);
  }

private:
  ::xarm_msgs::srv::TrajCtrl_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::TrajCtrl_Request>()
{
  return xarm_msgs::srv::builder::Init_TrajCtrl_Request_filename();
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_TrajCtrl_Response_message
{
public:
  explicit Init_TrajCtrl_Response_message(::xarm_msgs::srv::TrajCtrl_Response & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::TrajCtrl_Response message(::xarm_msgs::srv::TrajCtrl_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::TrajCtrl_Response msg_;
};

class Init_TrajCtrl_Response_ret
{
public:
  Init_TrajCtrl_Response_ret()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_TrajCtrl_Response_message ret(::xarm_msgs::srv::TrajCtrl_Response::_ret_type arg)
  {
    msg_.ret = std::move(arg);
    return Init_TrajCtrl_Response_message(msg_);
  }

private:
  ::xarm_msgs::srv::TrajCtrl_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::TrajCtrl_Response>()
{
  return xarm_msgs::srv::builder::Init_TrajCtrl_Response_ret();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__TRAJ_CTRL__BUILDER_HPP_
