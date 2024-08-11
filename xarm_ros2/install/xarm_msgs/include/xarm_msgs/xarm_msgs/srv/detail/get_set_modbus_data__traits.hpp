// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from xarm_msgs:srv/GetSetModbusData.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__GET_SET_MODBUS_DATA__TRAITS_HPP_
#define XARM_MSGS__SRV__DETAIL__GET_SET_MODBUS_DATA__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "xarm_msgs/srv/detail/get_set_modbus_data__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const GetSetModbusData_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: modbus_data
  {
    if (msg.modbus_data.size() == 0) {
      out << "modbus_data: []";
    } else {
      out << "modbus_data: [";
      size_t pending_items = msg.modbus_data.size();
      for (auto item : msg.modbus_data) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: modbus_length
  {
    out << "modbus_length: ";
    rosidl_generator_traits::value_to_yaml(msg.modbus_length, out);
    out << ", ";
  }

  // member: ret_length
  {
    out << "ret_length: ";
    rosidl_generator_traits::value_to_yaml(msg.ret_length, out);
    out << ", ";
  }

  // member: host_id
  {
    out << "host_id: ";
    rosidl_generator_traits::value_to_yaml(msg.host_id, out);
    out << ", ";
  }

  // member: is_transparent_transmission
  {
    out << "is_transparent_transmission: ";
    rosidl_generator_traits::value_to_yaml(msg.is_transparent_transmission, out);
    out << ", ";
  }

  // member: use_503_port
  {
    out << "use_503_port: ";
    rosidl_generator_traits::value_to_yaml(msg.use_503_port, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const GetSetModbusData_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: modbus_data
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.modbus_data.size() == 0) {
      out << "modbus_data: []\n";
    } else {
      out << "modbus_data:\n";
      for (auto item : msg.modbus_data) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: modbus_length
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "modbus_length: ";
    rosidl_generator_traits::value_to_yaml(msg.modbus_length, out);
    out << "\n";
  }

  // member: ret_length
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "ret_length: ";
    rosidl_generator_traits::value_to_yaml(msg.ret_length, out);
    out << "\n";
  }

  // member: host_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "host_id: ";
    rosidl_generator_traits::value_to_yaml(msg.host_id, out);
    out << "\n";
  }

  // member: is_transparent_transmission
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "is_transparent_transmission: ";
    rosidl_generator_traits::value_to_yaml(msg.is_transparent_transmission, out);
    out << "\n";
  }

  // member: use_503_port
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "use_503_port: ";
    rosidl_generator_traits::value_to_yaml(msg.use_503_port, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GetSetModbusData_Request & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace xarm_msgs

namespace rosidl_generator_traits
{

[[deprecated("use xarm_msgs::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const xarm_msgs::srv::GetSetModbusData_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::GetSetModbusData_Request & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::GetSetModbusData_Request>()
{
  return "xarm_msgs::srv::GetSetModbusData_Request";
}

template<>
inline const char * name<xarm_msgs::srv::GetSetModbusData_Request>()
{
  return "xarm_msgs/srv/GetSetModbusData_Request";
}

template<>
struct has_fixed_size<xarm_msgs::srv::GetSetModbusData_Request>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<xarm_msgs::srv::GetSetModbusData_Request>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<xarm_msgs::srv::GetSetModbusData_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const GetSetModbusData_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: ret
  {
    out << "ret: ";
    rosidl_generator_traits::value_to_yaml(msg.ret, out);
    out << ", ";
  }

  // member: message
  {
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
    out << ", ";
  }

  // member: ret_data
  {
    if (msg.ret_data.size() == 0) {
      out << "ret_data: []";
    } else {
      out << "ret_data: [";
      size_t pending_items = msg.ret_data.size();
      for (auto item : msg.ret_data) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const GetSetModbusData_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: ret
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "ret: ";
    rosidl_generator_traits::value_to_yaml(msg.ret, out);
    out << "\n";
  }

  // member: message
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
    out << "\n";
  }

  // member: ret_data
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.ret_data.size() == 0) {
      out << "ret_data: []\n";
    } else {
      out << "ret_data:\n";
      for (auto item : msg.ret_data) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GetSetModbusData_Response & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace xarm_msgs

namespace rosidl_generator_traits
{

[[deprecated("use xarm_msgs::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const xarm_msgs::srv::GetSetModbusData_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::GetSetModbusData_Response & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::GetSetModbusData_Response>()
{
  return "xarm_msgs::srv::GetSetModbusData_Response";
}

template<>
inline const char * name<xarm_msgs::srv::GetSetModbusData_Response>()
{
  return "xarm_msgs/srv/GetSetModbusData_Response";
}

template<>
struct has_fixed_size<xarm_msgs::srv::GetSetModbusData_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<xarm_msgs::srv::GetSetModbusData_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<xarm_msgs::srv::GetSetModbusData_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<xarm_msgs::srv::GetSetModbusData>()
{
  return "xarm_msgs::srv::GetSetModbusData";
}

template<>
inline const char * name<xarm_msgs::srv::GetSetModbusData>()
{
  return "xarm_msgs/srv/GetSetModbusData";
}

template<>
struct has_fixed_size<xarm_msgs::srv::GetSetModbusData>
  : std::integral_constant<
    bool,
    has_fixed_size<xarm_msgs::srv::GetSetModbusData_Request>::value &&
    has_fixed_size<xarm_msgs::srv::GetSetModbusData_Response>::value
  >
{
};

template<>
struct has_bounded_size<xarm_msgs::srv::GetSetModbusData>
  : std::integral_constant<
    bool,
    has_bounded_size<xarm_msgs::srv::GetSetModbusData_Request>::value &&
    has_bounded_size<xarm_msgs::srv::GetSetModbusData_Response>::value
  >
{
};

template<>
struct is_service<xarm_msgs::srv::GetSetModbusData>
  : std::true_type
{
};

template<>
struct is_service_request<xarm_msgs::srv::GetSetModbusData_Request>
  : std::true_type
{
};

template<>
struct is_service_response<xarm_msgs::srv::GetSetModbusData_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // XARM_MSGS__SRV__DETAIL__GET_SET_MODBUS_DATA__TRAITS_HPP_
