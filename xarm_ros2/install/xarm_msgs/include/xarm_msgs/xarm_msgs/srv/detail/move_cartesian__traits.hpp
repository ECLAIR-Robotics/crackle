// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from xarm_msgs:srv/MoveCartesian.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__MOVE_CARTESIAN__TRAITS_HPP_
#define XARM_MSGS__SRV__DETAIL__MOVE_CARTESIAN__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "xarm_msgs/srv/detail/move_cartesian__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const MoveCartesian_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: pose
  {
    if (msg.pose.size() == 0) {
      out << "pose: []";
    } else {
      out << "pose: [";
      size_t pending_items = msg.pose.size();
      for (auto item : msg.pose) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: speed
  {
    out << "speed: ";
    rosidl_generator_traits::value_to_yaml(msg.speed, out);
    out << ", ";
  }

  // member: acc
  {
    out << "acc: ";
    rosidl_generator_traits::value_to_yaml(msg.acc, out);
    out << ", ";
  }

  // member: mvtime
  {
    out << "mvtime: ";
    rosidl_generator_traits::value_to_yaml(msg.mvtime, out);
    out << ", ";
  }

  // member: wait
  {
    out << "wait: ";
    rosidl_generator_traits::value_to_yaml(msg.wait, out);
    out << ", ";
  }

  // member: timeout
  {
    out << "timeout: ";
    rosidl_generator_traits::value_to_yaml(msg.timeout, out);
    out << ", ";
  }

  // member: radius
  {
    out << "radius: ";
    rosidl_generator_traits::value_to_yaml(msg.radius, out);
    out << ", ";
  }

  // member: is_tool_coord
  {
    out << "is_tool_coord: ";
    rosidl_generator_traits::value_to_yaml(msg.is_tool_coord, out);
    out << ", ";
  }

  // member: relative
  {
    out << "relative: ";
    rosidl_generator_traits::value_to_yaml(msg.relative, out);
    out << ", ";
  }

  // member: motion_type
  {
    out << "motion_type: ";
    rosidl_generator_traits::value_to_yaml(msg.motion_type, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const MoveCartesian_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: pose
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.pose.size() == 0) {
      out << "pose: []\n";
    } else {
      out << "pose:\n";
      for (auto item : msg.pose) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: speed
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "speed: ";
    rosidl_generator_traits::value_to_yaml(msg.speed, out);
    out << "\n";
  }

  // member: acc
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "acc: ";
    rosidl_generator_traits::value_to_yaml(msg.acc, out);
    out << "\n";
  }

  // member: mvtime
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "mvtime: ";
    rosidl_generator_traits::value_to_yaml(msg.mvtime, out);
    out << "\n";
  }

  // member: wait
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "wait: ";
    rosidl_generator_traits::value_to_yaml(msg.wait, out);
    out << "\n";
  }

  // member: timeout
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "timeout: ";
    rosidl_generator_traits::value_to_yaml(msg.timeout, out);
    out << "\n";
  }

  // member: radius
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "radius: ";
    rosidl_generator_traits::value_to_yaml(msg.radius, out);
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

  // member: relative
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "relative: ";
    rosidl_generator_traits::value_to_yaml(msg.relative, out);
    out << "\n";
  }

  // member: motion_type
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "motion_type: ";
    rosidl_generator_traits::value_to_yaml(msg.motion_type, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const MoveCartesian_Request & msg, bool use_flow_style = false)
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
  const xarm_msgs::srv::MoveCartesian_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::MoveCartesian_Request & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::MoveCartesian_Request>()
{
  return "xarm_msgs::srv::MoveCartesian_Request";
}

template<>
inline const char * name<xarm_msgs::srv::MoveCartesian_Request>()
{
  return "xarm_msgs/srv/MoveCartesian_Request";
}

template<>
struct has_fixed_size<xarm_msgs::srv::MoveCartesian_Request>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<xarm_msgs::srv::MoveCartesian_Request>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<xarm_msgs::srv::MoveCartesian_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const MoveCartesian_Response & msg,
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
  const MoveCartesian_Response & msg,
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

inline std::string to_yaml(const MoveCartesian_Response & msg, bool use_flow_style = false)
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
  const xarm_msgs::srv::MoveCartesian_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::MoveCartesian_Response & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::MoveCartesian_Response>()
{
  return "xarm_msgs::srv::MoveCartesian_Response";
}

template<>
inline const char * name<xarm_msgs::srv::MoveCartesian_Response>()
{
  return "xarm_msgs/srv/MoveCartesian_Response";
}

template<>
struct has_fixed_size<xarm_msgs::srv::MoveCartesian_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<xarm_msgs::srv::MoveCartesian_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<xarm_msgs::srv::MoveCartesian_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<xarm_msgs::srv::MoveCartesian>()
{
  return "xarm_msgs::srv::MoveCartesian";
}

template<>
inline const char * name<xarm_msgs::srv::MoveCartesian>()
{
  return "xarm_msgs/srv/MoveCartesian";
}

template<>
struct has_fixed_size<xarm_msgs::srv::MoveCartesian>
  : std::integral_constant<
    bool,
    has_fixed_size<xarm_msgs::srv::MoveCartesian_Request>::value &&
    has_fixed_size<xarm_msgs::srv::MoveCartesian_Response>::value
  >
{
};

template<>
struct has_bounded_size<xarm_msgs::srv::MoveCartesian>
  : std::integral_constant<
    bool,
    has_bounded_size<xarm_msgs::srv::MoveCartesian_Request>::value &&
    has_bounded_size<xarm_msgs::srv::MoveCartesian_Response>::value
  >
{
};

template<>
struct is_service<xarm_msgs::srv::MoveCartesian>
  : std::true_type
{
};

template<>
struct is_service_request<xarm_msgs::srv::MoveCartesian_Request>
  : std::true_type
{
};

template<>
struct is_service_response<xarm_msgs::srv::MoveCartesian_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // XARM_MSGS__SRV__DETAIL__MOVE_CARTESIAN__TRAITS_HPP_
