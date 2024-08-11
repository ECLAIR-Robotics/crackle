// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/FtImpedance.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__FT_IMPEDANCE__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__FT_IMPEDANCE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/ft_impedance__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_FtImpedance_Request_b
{
public:
  explicit Init_FtImpedance_Request_b(::xarm_msgs::srv::FtImpedance_Request & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::FtImpedance_Request b(::xarm_msgs::srv::FtImpedance_Request::_b_type arg)
  {
    msg_.b = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::FtImpedance_Request msg_;
};

class Init_FtImpedance_Request_k
{
public:
  explicit Init_FtImpedance_Request_k(::xarm_msgs::srv::FtImpedance_Request & msg)
  : msg_(msg)
  {}
  Init_FtImpedance_Request_b k(::xarm_msgs::srv::FtImpedance_Request::_k_type arg)
  {
    msg_.k = std::move(arg);
    return Init_FtImpedance_Request_b(msg_);
  }

private:
  ::xarm_msgs::srv::FtImpedance_Request msg_;
};

class Init_FtImpedance_Request_m
{
public:
  explicit Init_FtImpedance_Request_m(::xarm_msgs::srv::FtImpedance_Request & msg)
  : msg_(msg)
  {}
  Init_FtImpedance_Request_k m(::xarm_msgs::srv::FtImpedance_Request::_m_type arg)
  {
    msg_.m = std::move(arg);
    return Init_FtImpedance_Request_k(msg_);
  }

private:
  ::xarm_msgs::srv::FtImpedance_Request msg_;
};

class Init_FtImpedance_Request_c_axis
{
public:
  explicit Init_FtImpedance_Request_c_axis(::xarm_msgs::srv::FtImpedance_Request & msg)
  : msg_(msg)
  {}
  Init_FtImpedance_Request_m c_axis(::xarm_msgs::srv::FtImpedance_Request::_c_axis_type arg)
  {
    msg_.c_axis = std::move(arg);
    return Init_FtImpedance_Request_m(msg_);
  }

private:
  ::xarm_msgs::srv::FtImpedance_Request msg_;
};

class Init_FtImpedance_Request_coord
{
public:
  Init_FtImpedance_Request_coord()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_FtImpedance_Request_c_axis coord(::xarm_msgs::srv::FtImpedance_Request::_coord_type arg)
  {
    msg_.coord = std::move(arg);
    return Init_FtImpedance_Request_c_axis(msg_);
  }

private:
  ::xarm_msgs::srv::FtImpedance_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::FtImpedance_Request>()
{
  return xarm_msgs::srv::builder::Init_FtImpedance_Request_coord();
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_FtImpedance_Response_message
{
public:
  explicit Init_FtImpedance_Response_message(::xarm_msgs::srv::FtImpedance_Response & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::FtImpedance_Response message(::xarm_msgs::srv::FtImpedance_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::FtImpedance_Response msg_;
};

class Init_FtImpedance_Response_ret
{
public:
  Init_FtImpedance_Response_ret()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_FtImpedance_Response_message ret(::xarm_msgs::srv::FtImpedance_Response::_ret_type arg)
  {
    msg_.ret = std::move(arg);
    return Init_FtImpedance_Response_message(msg_);
  }

private:
  ::xarm_msgs::srv::FtImpedance_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::FtImpedance_Response>()
{
  return xarm_msgs::srv::builder::Init_FtImpedance_Response_ret();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__FT_IMPEDANCE__BUILDER_HPP_
