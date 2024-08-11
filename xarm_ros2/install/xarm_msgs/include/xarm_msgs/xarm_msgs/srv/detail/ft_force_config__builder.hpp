// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/FtForceConfig.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__FT_FORCE_CONFIG__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__FT_FORCE_CONFIG__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/ft_force_config__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_FtForceConfig_Request_limits
{
public:
  explicit Init_FtForceConfig_Request_limits(::xarm_msgs::srv::FtForceConfig_Request & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::FtForceConfig_Request limits(::xarm_msgs::srv::FtForceConfig_Request::_limits_type arg)
  {
    msg_.limits = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::FtForceConfig_Request msg_;
};

class Init_FtForceConfig_Request_ref
{
public:
  explicit Init_FtForceConfig_Request_ref(::xarm_msgs::srv::FtForceConfig_Request & msg)
  : msg_(msg)
  {}
  Init_FtForceConfig_Request_limits ref(::xarm_msgs::srv::FtForceConfig_Request::_ref_type arg)
  {
    msg_.ref = std::move(arg);
    return Init_FtForceConfig_Request_limits(msg_);
  }

private:
  ::xarm_msgs::srv::FtForceConfig_Request msg_;
};

class Init_FtForceConfig_Request_c_axis
{
public:
  explicit Init_FtForceConfig_Request_c_axis(::xarm_msgs::srv::FtForceConfig_Request & msg)
  : msg_(msg)
  {}
  Init_FtForceConfig_Request_ref c_axis(::xarm_msgs::srv::FtForceConfig_Request::_c_axis_type arg)
  {
    msg_.c_axis = std::move(arg);
    return Init_FtForceConfig_Request_ref(msg_);
  }

private:
  ::xarm_msgs::srv::FtForceConfig_Request msg_;
};

class Init_FtForceConfig_Request_coord
{
public:
  Init_FtForceConfig_Request_coord()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_FtForceConfig_Request_c_axis coord(::xarm_msgs::srv::FtForceConfig_Request::_coord_type arg)
  {
    msg_.coord = std::move(arg);
    return Init_FtForceConfig_Request_c_axis(msg_);
  }

private:
  ::xarm_msgs::srv::FtForceConfig_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::FtForceConfig_Request>()
{
  return xarm_msgs::srv::builder::Init_FtForceConfig_Request_coord();
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_FtForceConfig_Response_message
{
public:
  explicit Init_FtForceConfig_Response_message(::xarm_msgs::srv::FtForceConfig_Response & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::FtForceConfig_Response message(::xarm_msgs::srv::FtForceConfig_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::FtForceConfig_Response msg_;
};

class Init_FtForceConfig_Response_ret
{
public:
  Init_FtForceConfig_Response_ret()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_FtForceConfig_Response_message ret(::xarm_msgs::srv::FtForceConfig_Response::_ret_type arg)
  {
    msg_.ret = std::move(arg);
    return Init_FtForceConfig_Response_message(msg_);
  }

private:
  ::xarm_msgs::srv::FtForceConfig_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::FtForceConfig_Response>()
{
  return xarm_msgs::srv::builder::Init_FtForceConfig_Response_ret();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__FT_FORCE_CONFIG__BUILDER_HPP_
