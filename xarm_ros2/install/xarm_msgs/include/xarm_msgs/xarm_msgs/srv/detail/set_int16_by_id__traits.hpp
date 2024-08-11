// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from xarm_msgs:srv/SetInt16ById.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__SET_INT16_BY_ID__TRAITS_HPP_
#define XARM_MSGS__SRV__DETAIL__SET_INT16_BY_ID__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "xarm_msgs/srv/detail/set_int16_by_id__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const SetInt16ById_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: id
  {
    out << "id: ";
    rosidl_generator_traits::value_to_yaml(msg.id, out);
    out << ", ";
  }

  // member: data
  {
    out << "data: ";
    rosidl_generator_traits::value_to_yaml(msg.data, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SetInt16ById_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "id: ";
    rosidl_generator_traits::value_to_yaml(msg.id, out);
    out << "\n";
  }

  // member: data
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "data: ";
    rosidl_generator_traits::value_to_yaml(msg.data, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SetInt16ById_Request & msg, bool use_flow_style = false)
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
  const xarm_msgs::srv::SetInt16ById_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::SetInt16ById_Request & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::SetInt16ById_Request>()
{
  return "xarm_msgs::srv::SetInt16ById_Request";
}

template<>
inline const char * name<xarm_msgs::srv::SetInt16ById_Request>()
{
  return "xarm_msgs/srv/SetInt16ById_Request";
}

template<>
struct has_fixed_size<xarm_msgs::srv::SetInt16ById_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<xarm_msgs::srv::SetInt16ById_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<xarm_msgs::srv::SetInt16ById_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const SetInt16ById_Response & msg,
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
  const SetInt16ById_Response & msg,
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

inline std::string to_yaml(const SetInt16ById_Response & msg, bool use_flow_style = false)
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
  const xarm_msgs::srv::SetInt16ById_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::SetInt16ById_Response & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::SetInt16ById_Response>()
{
  return "xarm_msgs::srv::SetInt16ById_Response";
}

template<>
inline const char * name<xarm_msgs::srv::SetInt16ById_Response>()
{
  return "xarm_msgs/srv/SetInt16ById_Response";
}

template<>
struct has_fixed_size<xarm_msgs::srv::SetInt16ById_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<xarm_msgs::srv::SetInt16ById_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<xarm_msgs::srv::SetInt16ById_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<xarm_msgs::srv::SetInt16ById>()
{
  return "xarm_msgs::srv::SetInt16ById";
}

template<>
inline const char * name<xarm_msgs::srv::SetInt16ById>()
{
  return "xarm_msgs/srv/SetInt16ById";
}

template<>
struct has_fixed_size<xarm_msgs::srv::SetInt16ById>
  : std::integral_constant<
    bool,
    has_fixed_size<xarm_msgs::srv::SetInt16ById_Request>::value &&
    has_fixed_size<xarm_msgs::srv::SetInt16ById_Response>::value
  >
{
};

template<>
struct has_bounded_size<xarm_msgs::srv::SetInt16ById>
  : std::integral_constant<
    bool,
    has_bounded_size<xarm_msgs::srv::SetInt16ById_Request>::value &&
    has_bounded_size<xarm_msgs::srv::SetInt16ById_Response>::value
  >
{
};

template<>
struct is_service<xarm_msgs::srv::SetInt16ById>
  : std::true_type
{
};

template<>
struct is_service_request<xarm_msgs::srv::SetInt16ById_Request>
  : std::true_type
{
};

template<>
struct is_service_response<xarm_msgs::srv::SetInt16ById_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // XARM_MSGS__SRV__DETAIL__SET_INT16_BY_ID__TRAITS_HPP_
