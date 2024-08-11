// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/BioGripperEnable.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__BIO_GRIPPER_ENABLE__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__BIO_GRIPPER_ENABLE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/bio_gripper_enable__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_BioGripperEnable_Request_timeout
{
public:
  explicit Init_BioGripperEnable_Request_timeout(::xarm_msgs::srv::BioGripperEnable_Request & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::BioGripperEnable_Request timeout(::xarm_msgs::srv::BioGripperEnable_Request::_timeout_type arg)
  {
    msg_.timeout = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::BioGripperEnable_Request msg_;
};

class Init_BioGripperEnable_Request_wait
{
public:
  explicit Init_BioGripperEnable_Request_wait(::xarm_msgs::srv::BioGripperEnable_Request & msg)
  : msg_(msg)
  {}
  Init_BioGripperEnable_Request_timeout wait(::xarm_msgs::srv::BioGripperEnable_Request::_wait_type arg)
  {
    msg_.wait = std::move(arg);
    return Init_BioGripperEnable_Request_timeout(msg_);
  }

private:
  ::xarm_msgs::srv::BioGripperEnable_Request msg_;
};

class Init_BioGripperEnable_Request_enable
{
public:
  Init_BioGripperEnable_Request_enable()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_BioGripperEnable_Request_wait enable(::xarm_msgs::srv::BioGripperEnable_Request::_enable_type arg)
  {
    msg_.enable = std::move(arg);
    return Init_BioGripperEnable_Request_wait(msg_);
  }

private:
  ::xarm_msgs::srv::BioGripperEnable_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::BioGripperEnable_Request>()
{
  return xarm_msgs::srv::builder::Init_BioGripperEnable_Request_enable();
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_BioGripperEnable_Response_message
{
public:
  explicit Init_BioGripperEnable_Response_message(::xarm_msgs::srv::BioGripperEnable_Response & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::BioGripperEnable_Response message(::xarm_msgs::srv::BioGripperEnable_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::BioGripperEnable_Response msg_;
};

class Init_BioGripperEnable_Response_ret
{
public:
  Init_BioGripperEnable_Response_ret()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_BioGripperEnable_Response_message ret(::xarm_msgs::srv::BioGripperEnable_Response::_ret_type arg)
  {
    msg_.ret = std::move(arg);
    return Init_BioGripperEnable_Response_message(msg_);
  }

private:
  ::xarm_msgs::srv::BioGripperEnable_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::BioGripperEnable_Response>()
{
  return xarm_msgs::srv::builder::Init_BioGripperEnable_Response_ret();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__BIO_GRIPPER_ENABLE__BUILDER_HPP_
