// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/FtCaliLoad.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__FT_CALI_LOAD__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__FT_CALI_LOAD__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/ft_cali_load__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_FtCaliLoad_Request_z
{
public:
  explicit Init_FtCaliLoad_Request_z(::xarm_msgs::srv::FtCaliLoad_Request & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::FtCaliLoad_Request z(::xarm_msgs::srv::FtCaliLoad_Request::_z_type arg)
  {
    msg_.z = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::FtCaliLoad_Request msg_;
};

class Init_FtCaliLoad_Request_y
{
public:
  explicit Init_FtCaliLoad_Request_y(::xarm_msgs::srv::FtCaliLoad_Request & msg)
  : msg_(msg)
  {}
  Init_FtCaliLoad_Request_z y(::xarm_msgs::srv::FtCaliLoad_Request::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_FtCaliLoad_Request_z(msg_);
  }

private:
  ::xarm_msgs::srv::FtCaliLoad_Request msg_;
};

class Init_FtCaliLoad_Request_x
{
public:
  explicit Init_FtCaliLoad_Request_x(::xarm_msgs::srv::FtCaliLoad_Request & msg)
  : msg_(msg)
  {}
  Init_FtCaliLoad_Request_y x(::xarm_msgs::srv::FtCaliLoad_Request::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_FtCaliLoad_Request_y(msg_);
  }

private:
  ::xarm_msgs::srv::FtCaliLoad_Request msg_;
};

class Init_FtCaliLoad_Request_m
{
public:
  explicit Init_FtCaliLoad_Request_m(::xarm_msgs::srv::FtCaliLoad_Request & msg)
  : msg_(msg)
  {}
  Init_FtCaliLoad_Request_x m(::xarm_msgs::srv::FtCaliLoad_Request::_m_type arg)
  {
    msg_.m = std::move(arg);
    return Init_FtCaliLoad_Request_x(msg_);
  }

private:
  ::xarm_msgs::srv::FtCaliLoad_Request msg_;
};

class Init_FtCaliLoad_Request_association_setting_tcp_load
{
public:
  explicit Init_FtCaliLoad_Request_association_setting_tcp_load(::xarm_msgs::srv::FtCaliLoad_Request & msg)
  : msg_(msg)
  {}
  Init_FtCaliLoad_Request_m association_setting_tcp_load(::xarm_msgs::srv::FtCaliLoad_Request::_association_setting_tcp_load_type arg)
  {
    msg_.association_setting_tcp_load = std::move(arg);
    return Init_FtCaliLoad_Request_m(msg_);
  }

private:
  ::xarm_msgs::srv::FtCaliLoad_Request msg_;
};

class Init_FtCaliLoad_Request_datas
{
public:
  Init_FtCaliLoad_Request_datas()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_FtCaliLoad_Request_association_setting_tcp_load datas(::xarm_msgs::srv::FtCaliLoad_Request::_datas_type arg)
  {
    msg_.datas = std::move(arg);
    return Init_FtCaliLoad_Request_association_setting_tcp_load(msg_);
  }

private:
  ::xarm_msgs::srv::FtCaliLoad_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::FtCaliLoad_Request>()
{
  return xarm_msgs::srv::builder::Init_FtCaliLoad_Request_datas();
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_FtCaliLoad_Response_message
{
public:
  explicit Init_FtCaliLoad_Response_message(::xarm_msgs::srv::FtCaliLoad_Response & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::FtCaliLoad_Response message(::xarm_msgs::srv::FtCaliLoad_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::FtCaliLoad_Response msg_;
};

class Init_FtCaliLoad_Response_ret
{
public:
  Init_FtCaliLoad_Response_ret()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_FtCaliLoad_Response_message ret(::xarm_msgs::srv::FtCaliLoad_Response::_ret_type arg)
  {
    msg_.ret = std::move(arg);
    return Init_FtCaliLoad_Response_message(msg_);
  }

private:
  ::xarm_msgs::srv::FtCaliLoad_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::FtCaliLoad_Response>()
{
  return xarm_msgs::srv::builder::Init_FtCaliLoad_Response_ret();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__FT_CALI_LOAD__BUILDER_HPP_
