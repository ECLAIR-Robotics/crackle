// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/FtForcePid.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__FT_FORCE_PID__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__FT_FORCE_PID__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/ft_force_pid__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_FtForcePid_Request_xe_limit
{
public:
  explicit Init_FtForcePid_Request_xe_limit(::xarm_msgs::srv::FtForcePid_Request & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::FtForcePid_Request xe_limit(::xarm_msgs::srv::FtForcePid_Request::_xe_limit_type arg)
  {
    msg_.xe_limit = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::FtForcePid_Request msg_;
};

class Init_FtForcePid_Request_kd
{
public:
  explicit Init_FtForcePid_Request_kd(::xarm_msgs::srv::FtForcePid_Request & msg)
  : msg_(msg)
  {}
  Init_FtForcePid_Request_xe_limit kd(::xarm_msgs::srv::FtForcePid_Request::_kd_type arg)
  {
    msg_.kd = std::move(arg);
    return Init_FtForcePid_Request_xe_limit(msg_);
  }

private:
  ::xarm_msgs::srv::FtForcePid_Request msg_;
};

class Init_FtForcePid_Request_ki
{
public:
  explicit Init_FtForcePid_Request_ki(::xarm_msgs::srv::FtForcePid_Request & msg)
  : msg_(msg)
  {}
  Init_FtForcePid_Request_kd ki(::xarm_msgs::srv::FtForcePid_Request::_ki_type arg)
  {
    msg_.ki = std::move(arg);
    return Init_FtForcePid_Request_kd(msg_);
  }

private:
  ::xarm_msgs::srv::FtForcePid_Request msg_;
};

class Init_FtForcePid_Request_kp
{
public:
  Init_FtForcePid_Request_kp()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_FtForcePid_Request_ki kp(::xarm_msgs::srv::FtForcePid_Request::_kp_type arg)
  {
    msg_.kp = std::move(arg);
    return Init_FtForcePid_Request_ki(msg_);
  }

private:
  ::xarm_msgs::srv::FtForcePid_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::FtForcePid_Request>()
{
  return xarm_msgs::srv::builder::Init_FtForcePid_Request_kp();
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_FtForcePid_Response_message
{
public:
  explicit Init_FtForcePid_Response_message(::xarm_msgs::srv::FtForcePid_Response & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::FtForcePid_Response message(::xarm_msgs::srv::FtForcePid_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::FtForcePid_Response msg_;
};

class Init_FtForcePid_Response_ret
{
public:
  Init_FtForcePid_Response_ret()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_FtForcePid_Response_message ret(::xarm_msgs::srv::FtForcePid_Response::_ret_type arg)
  {
    msg_.ret = std::move(arg);
    return Init_FtForcePid_Response_message(msg_);
  }

private:
  ::xarm_msgs::srv::FtForcePid_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::FtForcePid_Response>()
{
  return xarm_msgs::srv::builder::Init_FtForcePid_Response_ret();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__FT_FORCE_PID__BUILDER_HPP_
