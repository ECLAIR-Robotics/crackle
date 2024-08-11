// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/GetFloat32List.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__GET_FLOAT32_LIST__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__GET_FLOAT32_LIST__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/get_float32_list__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{


}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::GetFloat32List_Request>()
{
  return ::xarm_msgs::srv::GetFloat32List_Request(rosidl_runtime_cpp::MessageInitialization::ZERO);
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_GetFloat32List_Response_datas
{
public:
  explicit Init_GetFloat32List_Response_datas(::xarm_msgs::srv::GetFloat32List_Response & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::GetFloat32List_Response datas(::xarm_msgs::srv::GetFloat32List_Response::_datas_type arg)
  {
    msg_.datas = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::GetFloat32List_Response msg_;
};

class Init_GetFloat32List_Response_message
{
public:
  explicit Init_GetFloat32List_Response_message(::xarm_msgs::srv::GetFloat32List_Response & msg)
  : msg_(msg)
  {}
  Init_GetFloat32List_Response_datas message(::xarm_msgs::srv::GetFloat32List_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return Init_GetFloat32List_Response_datas(msg_);
  }

private:
  ::xarm_msgs::srv::GetFloat32List_Response msg_;
};

class Init_GetFloat32List_Response_ret
{
public:
  Init_GetFloat32List_Response_ret()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_GetFloat32List_Response_message ret(::xarm_msgs::srv::GetFloat32List_Response::_ret_type arg)
  {
    msg_.ret = std::move(arg);
    return Init_GetFloat32List_Response_message(msg_);
  }

private:
  ::xarm_msgs::srv::GetFloat32List_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::GetFloat32List_Response>()
{
  return xarm_msgs::srv::builder::Init_GetFloat32List_Response_ret();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__GET_FLOAT32_LIST__BUILDER_HPP_
