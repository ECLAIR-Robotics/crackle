// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/PlanExec.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__PLAN_EXEC__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__PLAN_EXEC__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/plan_exec__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_PlanExec_Request_wait
{
public:
  Init_PlanExec_Request_wait()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::xarm_msgs::srv::PlanExec_Request wait(::xarm_msgs::srv::PlanExec_Request::_wait_type arg)
  {
    msg_.wait = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::PlanExec_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::PlanExec_Request>()
{
  return xarm_msgs::srv::builder::Init_PlanExec_Request_wait();
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_PlanExec_Response_success
{
public:
  Init_PlanExec_Response_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::xarm_msgs::srv::PlanExec_Response success(::xarm_msgs::srv::PlanExec_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::PlanExec_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::PlanExec_Response>()
{
  return xarm_msgs::srv::builder::Init_PlanExec_Response_success();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__PLAN_EXEC__BUILDER_HPP_
