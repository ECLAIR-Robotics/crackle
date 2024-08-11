// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xarm_msgs:srv/GetSetModbusData.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__GET_SET_MODBUS_DATA__BUILDER_HPP_
#define XARM_MSGS__SRV__DETAIL__GET_SET_MODBUS_DATA__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xarm_msgs/srv/detail/get_set_modbus_data__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_GetSetModbusData_Request_use_503_port
{
public:
  explicit Init_GetSetModbusData_Request_use_503_port(::xarm_msgs::srv::GetSetModbusData_Request & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::GetSetModbusData_Request use_503_port(::xarm_msgs::srv::GetSetModbusData_Request::_use_503_port_type arg)
  {
    msg_.use_503_port = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::GetSetModbusData_Request msg_;
};

class Init_GetSetModbusData_Request_is_transparent_transmission
{
public:
  explicit Init_GetSetModbusData_Request_is_transparent_transmission(::xarm_msgs::srv::GetSetModbusData_Request & msg)
  : msg_(msg)
  {}
  Init_GetSetModbusData_Request_use_503_port is_transparent_transmission(::xarm_msgs::srv::GetSetModbusData_Request::_is_transparent_transmission_type arg)
  {
    msg_.is_transparent_transmission = std::move(arg);
    return Init_GetSetModbusData_Request_use_503_port(msg_);
  }

private:
  ::xarm_msgs::srv::GetSetModbusData_Request msg_;
};

class Init_GetSetModbusData_Request_host_id
{
public:
  explicit Init_GetSetModbusData_Request_host_id(::xarm_msgs::srv::GetSetModbusData_Request & msg)
  : msg_(msg)
  {}
  Init_GetSetModbusData_Request_is_transparent_transmission host_id(::xarm_msgs::srv::GetSetModbusData_Request::_host_id_type arg)
  {
    msg_.host_id = std::move(arg);
    return Init_GetSetModbusData_Request_is_transparent_transmission(msg_);
  }

private:
  ::xarm_msgs::srv::GetSetModbusData_Request msg_;
};

class Init_GetSetModbusData_Request_ret_length
{
public:
  explicit Init_GetSetModbusData_Request_ret_length(::xarm_msgs::srv::GetSetModbusData_Request & msg)
  : msg_(msg)
  {}
  Init_GetSetModbusData_Request_host_id ret_length(::xarm_msgs::srv::GetSetModbusData_Request::_ret_length_type arg)
  {
    msg_.ret_length = std::move(arg);
    return Init_GetSetModbusData_Request_host_id(msg_);
  }

private:
  ::xarm_msgs::srv::GetSetModbusData_Request msg_;
};

class Init_GetSetModbusData_Request_modbus_length
{
public:
  explicit Init_GetSetModbusData_Request_modbus_length(::xarm_msgs::srv::GetSetModbusData_Request & msg)
  : msg_(msg)
  {}
  Init_GetSetModbusData_Request_ret_length modbus_length(::xarm_msgs::srv::GetSetModbusData_Request::_modbus_length_type arg)
  {
    msg_.modbus_length = std::move(arg);
    return Init_GetSetModbusData_Request_ret_length(msg_);
  }

private:
  ::xarm_msgs::srv::GetSetModbusData_Request msg_;
};

class Init_GetSetModbusData_Request_modbus_data
{
public:
  Init_GetSetModbusData_Request_modbus_data()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_GetSetModbusData_Request_modbus_length modbus_data(::xarm_msgs::srv::GetSetModbusData_Request::_modbus_data_type arg)
  {
    msg_.modbus_data = std::move(arg);
    return Init_GetSetModbusData_Request_modbus_length(msg_);
  }

private:
  ::xarm_msgs::srv::GetSetModbusData_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::GetSetModbusData_Request>()
{
  return xarm_msgs::srv::builder::Init_GetSetModbusData_Request_modbus_data();
}

}  // namespace xarm_msgs


namespace xarm_msgs
{

namespace srv
{

namespace builder
{

class Init_GetSetModbusData_Response_ret_data
{
public:
  explicit Init_GetSetModbusData_Response_ret_data(::xarm_msgs::srv::GetSetModbusData_Response & msg)
  : msg_(msg)
  {}
  ::xarm_msgs::srv::GetSetModbusData_Response ret_data(::xarm_msgs::srv::GetSetModbusData_Response::_ret_data_type arg)
  {
    msg_.ret_data = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xarm_msgs::srv::GetSetModbusData_Response msg_;
};

class Init_GetSetModbusData_Response_message
{
public:
  explicit Init_GetSetModbusData_Response_message(::xarm_msgs::srv::GetSetModbusData_Response & msg)
  : msg_(msg)
  {}
  Init_GetSetModbusData_Response_ret_data message(::xarm_msgs::srv::GetSetModbusData_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return Init_GetSetModbusData_Response_ret_data(msg_);
  }

private:
  ::xarm_msgs::srv::GetSetModbusData_Response msg_;
};

class Init_GetSetModbusData_Response_ret
{
public:
  Init_GetSetModbusData_Response_ret()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_GetSetModbusData_Response_message ret(::xarm_msgs::srv::GetSetModbusData_Response::_ret_type arg)
  {
    msg_.ret = std::move(arg);
    return Init_GetSetModbusData_Response_message(msg_);
  }

private:
  ::xarm_msgs::srv::GetSetModbusData_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xarm_msgs::srv::GetSetModbusData_Response>()
{
  return xarm_msgs::srv::builder::Init_GetSetModbusData_Response_ret();
}

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__GET_SET_MODBUS_DATA__BUILDER_HPP_
