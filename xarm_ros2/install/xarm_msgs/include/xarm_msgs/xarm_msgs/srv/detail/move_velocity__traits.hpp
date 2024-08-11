// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from xarm_msgs:srv/MoveVelocity.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__MOVE_VELOCITY__TRAITS_HPP_
#define XARM_MSGS__SRV__DETAIL__MOVE_VELOCITY__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "xarm_msgs/srv/detail/move_velocity__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const MoveVelocity_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: speeds
  {
    if (msg.speeds.size() == 0) {
      out << "speeds: []";
    } else {
      out << "speeds: [";
      size_t pending_items = msg.speeds.size();
      for (auto item : msg.speeds) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: is_sync
  {
    out << "is_sync: ";
    rosidl_generator_traits::value_to_yaml(msg.is_sync, out);
    out << ", ";
  }

  // member: is_tool_coord
  {
    out << "is_tool_coord: ";
    rosidl_generator_traits::value_to_yaml(msg.is_tool_coord, out);
    out << ", ";
  }

  // member: duration
  {
    out << "duration: ";
    rosidl_generator_traits::value_to_yaml(msg.duration, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const MoveVelocity_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: speeds
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.speeds.size() == 0) {
      out << "speeds: []\n";
    } else {
      out << "speeds:\n";
      for (auto item : msg.speeds) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: is_sync
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "is_sync: ";
    rosidl_generator_traits::value_to_yaml(msg.is_sync, out);
    out << "\n";
  }

  // member: is_tool_coord
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "is_tool_coord: ";
    rosidl_generator_traits::value_to_yaml(msg.is_tool_coord, out);
    out << "\n";
  }

  // member: duration
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "duration: ";
    rosidl_generator_traits::value_to_yaml(msg.duration, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const MoveVelocity_Request & msg, bool use_flow_style = false)
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
  const xarm_msgs::srv::MoveVelocity_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::MoveVelocity_Request & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::MoveVelocity_Request>()
{
  return "xarm_msgs::srv::MoveVelocity_Request";
}

template<>
inline const char * name<xarm_msgs::srv::MoveVelocity_Request>()
{
  return "xarm_msgs/srv/MoveVelocity_Request";
}

template<>
struct has_fixed_size<xarm_msgs::srv::MoveVelocity_Request>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<xarm_msgs::srv::MoveVelocity_Request>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<xarm_msgs::srv::MoveVelocity_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const MoveVelocity_Response & msg,
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
  const MoveVelocity_Response & msg,
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

inline std::string to_yaml(const MoveVelocity_Response & msg, bool use_flow_style = false)
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
  const xarm_msgs::srv::MoveVelocity_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::MoveVelocity_Response & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::MoveVelocity_Response>()
{
  return "xarm_msgs::srv::MoveVelocity_Response";
}

template<>
inline const char * name<xarm_msgs::srv::MoveVelocity_Response>()
{
  return "xarm_msgs/srv/MoveVelocity_Response";
}

template<>
struct has_fixed_size<xarm_msgs::srv::MoveVelocity_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<xarm_msgs::srv::MoveVelocity_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<xarm_msgs::srv::MoveVelocity_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<xarm_msgs::srv::MoveVelocity>()
{
  return "xarm_msgs::srv::MoveVelocity";
}

template<>
inline const char * name<xarm_msgs::srv::MoveVelocity>()
{
  return "xarm_msgs/srv/MoveVelocity";
}

template<>
struct has_fixed_size<xarm_msgs::srv::MoveVelocity>
  : std::integral_constant<
    bool,
    has_fixed_size<xarm_msgs::srv::MoveVelocity_Request>::value &&
    has_fixed_size<xarm_msgs::srv::MoveVelocity_Response>::value
  >
{
};

template<>
struct has_bounded_size<xarm_msgs::srv::MoveVelocity>
  : std::integral_constant<
    bool,
    has_bounded_size<xarm_msgs::srv::MoveVelocity_Request>::value &&
    has_bounded_size<xarm_msgs::srv::MoveVelocity_Response>::value
  >
{
};

template<>
struct is_service<xarm_msgs::srv::MoveVelocity>
  : std::true_type
{
};

template<>
struct is_service_request<xarm_msgs::srv::MoveVelocity_Request>
  : std::true_type
{
};

template<>
struct is_service_response<xarm_msgs::srv::MoveVelocity_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // XARM_MSGS__SRV__DETAIL__MOVE_VELOCITY__TRAITS_HPP_
