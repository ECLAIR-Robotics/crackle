// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from xarm_msgs:srv/SetAnalogIO.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__SET_ANALOG_IO__TRAITS_HPP_
#define XARM_MSGS__SRV__DETAIL__SET_ANALOG_IO__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "xarm_msgs/srv/detail/set_analog_io__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const SetAnalogIO_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: ionum
  {
    out << "ionum: ";
    rosidl_generator_traits::value_to_yaml(msg.ionum, out);
    out << ", ";
  }

  // member: value
  {
    out << "value: ";
    rosidl_generator_traits::value_to_yaml(msg.value, out);
    out << ", ";
  }

  // member: xyz
  {
    if (msg.xyz.size() == 0) {
      out << "xyz: []";
    } else {
      out << "xyz: [";
      size_t pending_items = msg.xyz.size();
      for (auto item : msg.xyz) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: tol_r
  {
    out << "tol_r: ";
    rosidl_generator_traits::value_to_yaml(msg.tol_r, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SetAnalogIO_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: ionum
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "ionum: ";
    rosidl_generator_traits::value_to_yaml(msg.ionum, out);
    out << "\n";
  }

  // member: value
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "value: ";
    rosidl_generator_traits::value_to_yaml(msg.value, out);
    out << "\n";
  }

  // member: xyz
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.xyz.size() == 0) {
      out << "xyz: []\n";
    } else {
      out << "xyz:\n";
      for (auto item : msg.xyz) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: tol_r
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "tol_r: ";
    rosidl_generator_traits::value_to_yaml(msg.tol_r, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SetAnalogIO_Request & msg, bool use_flow_style = false)
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
  const xarm_msgs::srv::SetAnalogIO_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::SetAnalogIO_Request & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::SetAnalogIO_Request>()
{
  return "xarm_msgs::srv::SetAnalogIO_Request";
}

template<>
inline const char * name<xarm_msgs::srv::SetAnalogIO_Request>()
{
  return "xarm_msgs/srv/SetAnalogIO_Request";
}

template<>
struct has_fixed_size<xarm_msgs::srv::SetAnalogIO_Request>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<xarm_msgs::srv::SetAnalogIO_Request>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<xarm_msgs::srv::SetAnalogIO_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const SetAnalogIO_Response & msg,
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
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SetAnalogIO_Response & msg,
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
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SetAnalogIO_Response & msg, bool use_flow_style = false)
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
  const xarm_msgs::srv::SetAnalogIO_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::SetAnalogIO_Response & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::SetAnalogIO_Response>()
{
  return "xarm_msgs::srv::SetAnalogIO_Response";
}

template<>
inline const char * name<xarm_msgs::srv::SetAnalogIO_Response>()
{
  return "xarm_msgs/srv/SetAnalogIO_Response";
}

template<>
struct has_fixed_size<xarm_msgs::srv::SetAnalogIO_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<xarm_msgs::srv::SetAnalogIO_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<xarm_msgs::srv::SetAnalogIO_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<xarm_msgs::srv::SetAnalogIO>()
{
  return "xarm_msgs::srv::SetAnalogIO";
}

template<>
inline const char * name<xarm_msgs::srv::SetAnalogIO>()
{
  return "xarm_msgs/srv/SetAnalogIO";
}

template<>
struct has_fixed_size<xarm_msgs::srv::SetAnalogIO>
  : std::integral_constant<
    bool,
    has_fixed_size<xarm_msgs::srv::SetAnalogIO_Request>::value &&
    has_fixed_size<xarm_msgs::srv::SetAnalogIO_Response>::value
  >
{
};

template<>
struct has_bounded_size<xarm_msgs::srv::SetAnalogIO>
  : std::integral_constant<
    bool,
    has_bounded_size<xarm_msgs::srv::SetAnalogIO_Request>::value &&
    has_bounded_size<xarm_msgs::srv::SetAnalogIO_Response>::value
  >
{
};

template<>
struct is_service<xarm_msgs::srv::SetAnalogIO>
  : std::true_type
{
};

template<>
struct is_service_request<xarm_msgs::srv::SetAnalogIO_Request>
  : std::true_type
{
};

template<>
struct is_service_response<xarm_msgs::srv::SetAnalogIO_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // XARM_MSGS__SRV__DETAIL__SET_ANALOG_IO__TRAITS_HPP_
