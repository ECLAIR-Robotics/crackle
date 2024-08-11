// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/RobotiqGetStatus.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__ROBOTIQ_GET_STATUS__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__ROBOTIQ_GET_STATUS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/robotiq_get_status__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_RobotiqGetStatus_Request_number_of_registers
{
public:
  Init_RobotiqGetStatus_Request_number_of_registers()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::xarm_msgs::srv::RobotiqGetStatus_Request number_of_registers(::xarm_msgs::srv::RobotiqGetStatus_Request::_number_of_registers_type arg)
  {
    msg_.number_of_registers = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::RobotiqGetStatus_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::RobotiqGetStatus_Request>()
{
  return xarm_msgs::srv::builder::Init_RobotiqGetStatus_Request_number_of_registers();
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_RobotiqGetStatus_Response_ret_data
{
public:
  explicit Init_RobotiqGetStatus_Response_ret_data(::xarm_msgs::srv::RobotiqGetStatus_Response & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::RobotiqGetStatus_Response ret_data(::xarm_msgs::srv::RobotiqGetStatus_Response::_ret_data_type arg)
  {
    msg_.ret_data = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::RobotiqGetStatus_Response msg_;
};

class Init_RobotiqGetStatus_Response_message
{
public:
  explicit Init_RobotiqGetStatus_Response_message(::xarm_msgs::srv::RobotiqGetStatus_Response & msg)
  : msg_(msg)
  {}
  Init_RobotiqGetStatus_Response_ret_data message(::xarm_msgs::srv::RobotiqGetStatus_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return Init_RobotiqGetStatus_Response_ret_data(msg_);
  }

private:
  ::xarm_msgs::srv::RobotiqGetStatus_Response msg_;
};

class Init_RobotiqGetStatus_Response_ret
{
public:
  Init_RobotiqGetStatus_Response_ret()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_RobotiqGetStatus_Response_message ret(::xarm_msgs::srv::RobotiqGetStatus_Response::_ret_type arg)
  {
    msg_.ret = std::move(arg);
    return Init_RobotiqGetStatus_Response_message(msg_);
  }

private:
  ::xarm_msgs::srv::RobotiqGetStatus_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::RobotiqGetStatus_Response>()
{
  return xarm_msgs::srv::builder::Init_RobotiqGetStatus_Response_ret();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__ROBOTIQ_GET_STATUS__BUILDER_HPP_
