// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from xarm_msgs:msg/IOState.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__MSG__DETAIL__IO_STATE__TRAITS_HPP_
#define XARM_MSGS__MSG__DETAIL__IO_STATE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "xarm_msgs/msg/detail/io_state__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace xarm_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const IOState & msg,
  std::ostream & out)
{
  out << "{";
  // member: digital_1
  {
    out << "digital_1: ";
    rosidl_generator_traits::value_to_yaml(msg.digital_1, out);
    out << ", ";
  }

  // member: digital_2
  {
    out << "digital_2: ";
    rosidl_generator_traits::value_to_yaml(msg.digital_2, out);
    out << ", ";
  }

  // member: analog_1
  {
    out << "analog_1: ";
    rosidl_generator_traits::value_to_yaml(msg.analog_1, out);
    out << ", ";
  }

  // member: analog_2
  {
    out << "analog_2: ";
    rosidl_generator_traits::value_to_yaml(msg.analog_2, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const IOState & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: digital_1
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "digital_1: ";
    rosidl_generator_traits::value_to_yaml(msg.digital_1, out);
    out << "\n";
  }

  // member: digital_2
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "digital_2: ";
    rosidl_generator_traits::value_to_yaml(msg.digital_2, out);
    out << "\n";
  }

  // member: analog_1
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "analog_1: ";
    rosidl_generator_traits::value_to_yaml(msg.analog_1, out);
    out << "\n";
  }

  // member: analog_2
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "analog_2: ";
    rosidl_generator_traits::value_to_yaml(msg.analog_2, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const IOState & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace xarm_msgs

namespace rosidl_generator_traits
{

[[deprecated("use xarm_msgs::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const xarm_msgs::msg::IOState & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::msg::IOState & msg)
{
  return xarm_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::msg::IOState>()
{
  return "xarm_msgs::msg::IOState";
}

template<>
inline const char * name<xarm_msgs::msg::IOState>()
{
  return "xarm_msgs/msg/IOState";
}

template<>
struct has_fixed_size<xarm_msgs::msg::IOState>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<xarm_msgs::msg::IOState>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<xarm_msgs::msg::IOState>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // XARM_MSGS__MSG__DETAIL__IO_STATE__TRAITS_HPP_
