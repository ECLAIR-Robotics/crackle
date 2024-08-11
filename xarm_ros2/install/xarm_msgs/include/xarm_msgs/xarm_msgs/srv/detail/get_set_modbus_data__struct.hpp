// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from xarm_msgs:srv/GetSetModbusData.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__GET_SET_MODBUS_DATA__STRUCT_HPP_
#define XARM_MSGS__SRV__DETAIL__GET_SET_MODBUS_DATA__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__xarm_msgs__srv__GetSetModbusData_Request __attribute__((deprecated))
#else
# define DEPRECATED__xarm_msgs__srv__GetSetModbusData_Request __declspec(deprecated)
#endif

namespace xarm_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct GetSetModbusData_Request_
{
  using Type = GetSetModbusData_Request_<ContainerAllocator>;

  explicit GetSetModbusData_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->modbus_length = 0;
      this->host_id = 9;
      this->is_transparent_transmission = false;
      this->use_503_port = false;
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->modbus_length = 0;
      this->ret_length = 0;
      this->host_id = 0;
      this->is_transparent_transmission = false;
      this->use_503_port = false;
    }
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->ret_length = 0;
    }
  }

  explicit GetSetModbusData_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->modbus_length = 0;
      this->host_id = 9;
      this->is_transparent_transmission = false;
      this->use_503_port = false;
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->modbus_length = 0;
      this->ret_length = 0;
      this->host_id = 0;
      this->is_transparent_transmission = false;
      this->use_503_port = false;
    }
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->ret_length = 0;
    }
  }

  // field types and members
  using _modbus_data_type =
    std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>>;
  _modbus_data_type modbus_data;
  using _modbus_length_type =
    int16_t;
  _modbus_length_type modbus_length;
  using _ret_length_type =
    int16_t;
  _ret_length_type ret_length;
  using _host_id_type =
    uint8_t;
  _host_id_type host_id;
  using _is_transparent_transmission_type =
    bool;
  _is_transparent_transmission_type is_transparent_transmission;
  using _use_503_port_type =
    bool;
  _use_503_port_type use_503_port;

  // setters for named parameter idiom
  Type & set__modbus_data(
    const std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>> & _arg)
  {
    this->modbus_data = _arg;
    return *this;
  }
  Type & set__modbus_length(
    const int16_t & _arg)
  {
    this->modbus_length = _arg;
    return *this;
  }
  Type & set__ret_length(
    const int16_t & _arg)
  {
    this->ret_length = _arg;
    return *this;
  }
  Type & set__host_id(
    const uint8_t & _arg)
  {
    this->host_id = _arg;
    return *this;
  }
  Type & set__is_transparent_transmission(
    const bool & _arg)
  {
    this->is_transparent_transmission = _arg;
    return *this;
  }
  Type & set__use_503_port(
    const bool & _arg)
  {
    this->use_503_port = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    xarm_msgs::srv::GetSetModbusData_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const xarm_msgs::srv::GetSetModbusData_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<xarm_msgs::srv::GetSetModbusData_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<xarm_msgs::srv::GetSetModbusData_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::GetSetModbusData_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::GetSetModbusData_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::GetSetModbusData_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::GetSetModbusData_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<xarm_msgs::srv::GetSetModbusData_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<xarm_msgs::srv::GetSetModbusData_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__xarm_msgs__srv__GetSetModbusData_Request
    std::shared_ptr<xarm_msgs::srv::GetSetModbusData_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__xarm_msgs__srv__GetSetModbusData_Request
    std::shared_ptr<xarm_msgs::srv::GetSetModbusData_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const GetSetModbusData_Request_ & other) const
  {
    if (this->modbus_data != other.modbus_data) {
      return false;
    }
    if (this->modbus_length != other.modbus_length) {
      return false;
    }
    if (this->ret_length != other.ret_length) {
      return false;
    }
    if (this->host_id != other.host_id) {
      return false;
    }
    if (this->is_transparent_transmission != other.is_transparent_transmission) {
      return false;
    }
    if (this->use_503_port != other.use_503_port) {
      return false;
    }
    return true;
  }
  bool operator!=(const GetSetModbusData_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct GetSetModbusData_Request_

// alias to use template instance with default allocator
using GetSetModbusData_Request =
  xarm_msgs::srv::GetSetModbusData_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace xarm_msgs


#ifndef _WIN32
# define DEPRECATED__xarm_msgs__srv__GetSetModbusData_Response __attribute__((deprecated))
#else
# define DEPRECATED__xarm_msgs__srv__GetSetModbusData_Response __declspec(deprecated)
#endif

namespace xarm_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct GetSetModbusData_Response_
{
  using Type = GetSetModbusData_Response_<ContainerAllocator>;

  explicit GetSetModbusData_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->ret = 0;
      this->message = "";
    }
  }

  explicit GetSetModbusData_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : message(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->ret = 0;
      this->message = "";
    }
  }

  // field types and members
  using _ret_type =
    int16_t;
  _ret_type ret;
  using _message_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _message_type message;
  using _ret_data_type =
    std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>>;
  _ret_data_type ret_data;

  // setters for named parameter idiom
  Type & set__ret(
    const int16_t & _arg)
  {
    this->ret = _arg;
    return *this;
  }
  Type & set__message(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->message = _arg;
    return *this;
  }
  Type & set__ret_data(
    const std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>> & _arg)
  {
    this->ret_data = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    xarm_msgs::srv::GetSetModbusData_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const xarm_msgs::srv::GetSetModbusData_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<xarm_msgs::srv::GetSetModbusData_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<xarm_msgs::srv::GetSetModbusData_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::GetSetModbusData_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::GetSetModbusData_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      xarm_msgs::srv::GetSetModbusData_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<xarm_msgs::srv::GetSetModbusData_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<xarm_msgs::srv::GetSetModbusData_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<xarm_msgs::srv::GetSetModbusData_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__xarm_msgs__srv__GetSetModbusData_Response
    std::shared_ptr<xarm_msgs::srv::GetSetModbusData_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__xarm_msgs__srv__GetSetModbusData_Response
    std::shared_ptr<xarm_msgs::srv::GetSetModbusData_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const GetSetModbusData_Response_ & other) const
  {
    if (this->ret != other.ret) {
      return false;
    }
    if (this->message != other.message) {
      return false;
    }
    if (this->ret_data != other.ret_data) {
      return false;
    }
    return true;
  }
  bool operator!=(const GetSetModbusData_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct GetSetModbusData_Response_

// alias to use template instance with default allocator
using GetSetModbusData_Response =
  xarm_msgs::srv::GetSetModbusData_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace xarm_msgs

namespace xarm_msgs
{

namespace srv
{

struct GetSetModbusData
{
  using Request = xarm_msgs::srv::GetSetModbusData_Request;
  using Response = xarm_msgs::srv::GetSetModbusData_Response;
};

}  // namespace srv

}  // namespace xarm_msgs

#endif  // XARM_MSGS__SRV__DETAIL__GET_SET_MODBUS_DATA__STRUCT_HPP_
