// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/PlanJoint.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__PLAN_JOINT__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__PLAN_JOINT__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/plan_joint__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_PlanJoint_Request_target
{
public:
  Init_PlanJoint_Request_target()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::xarm_msgs::srv::PlanJoint_Request target(::xarm_msgs::srv::PlanJoint_Request::_target_type arg)
  {
    msg_.target = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::PlanJoint_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::PlanJoint_Request>()
{
  return xarm_msgs::srv::builder::Init_PlanJoint_Request_target();
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_PlanJoint_Response_success
{
public:
  Init_PlanJoint_Response_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::xarm_msgs::srv::PlanJoint_Response success(::xarm_msgs::srv::PlanJoint_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::PlanJoint_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::PlanJoint_Response>()
{
  return xarm_msgs::srv::builder::Init_PlanJoint_Response_success();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__PLAN_JOINT__BUILDER_HPP_
