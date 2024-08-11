// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/LinearTrackBackOrigin.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__LINEAR_TRACK_BACK_ORIGIN__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__LINEAR_TRACK_BACK_ORIGIN__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/linear_track_back_origin__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_LinearTrackBackOrigin_Request_auto_enable
{
public:
  explicit Init_LinearTrackBackOrigin_Request_auto_enable(::xarm_msgs::srv::LinearTrackBackOrigin_Request & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::LinearTrackBackOrigin_Request auto_enable(::xarm_msgs::srv::LinearTrackBackOrigin_Request::_auto_enable_type arg)
  {
    msg_.auto_enable = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::LinearTrackBackOrigin_Request msg_;
};

class Init_LinearTrackBackOrigin_Request_wait
{
public:
  Init_LinearTrackBackOrigin_Request_wait()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_LinearTrackBackOrigin_Request_auto_enable wait(::xarm_msgs::srv::LinearTrackBackOrigin_Request::_wait_type arg)
  {
    msg_.wait = std::move(arg);
    return Init_LinearTrackBackOrigin_Request_auto_enable(msg_);
  }

private:
  ::xarm_msgs::srv::LinearTrackBackOrigin_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::LinearTrackBackOrigin_Request>()
{
  return xarm_msgs::srv::builder::Init_LinearTrackBackOrigin_Request_wait();
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_LinearTrackBackOrigin_Response_message
{
public:
  explicit Init_LinearTrackBackOrigin_Response_message(::xarm_msgs::srv::LinearTrackBackOrigin_Response & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::LinearTrackBackOrigin_Response message(::xarm_msgs::srv::LinearTrackBackOrigin_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::LinearTrackBackOrigin_Response msg_;
};

class Init_LinearTrackBackOrigin_Response_ret
{
public:
  Init_LinearTrackBackOrigin_Response_ret()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_LinearTrackBackOrigin_Response_message ret(::xarm_msgs::srv::LinearTrackBackOrigin_Response::_ret_type arg)
  {
    msg_.ret = std::move(arg);
    return Init_LinearTrackBackOrigin_Response_message(msg_);
  }

private:
  ::xarm_msgs::srv::LinearTrackBackOrigin_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::LinearTrackBackOrigin_Response>()
{
  return xarm_msgs::srv::builder::Init_LinearTrackBackOrigin_Response_ret();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__LINEAR_TRACK_BACK_ORIGIN__BUILDER_HPP_
