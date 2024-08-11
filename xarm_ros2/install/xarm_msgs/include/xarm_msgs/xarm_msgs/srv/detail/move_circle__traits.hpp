// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from xarm_msgs:srv/MoveCircle.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__MOVE_CIRCLE__TRAITS_HPP_
#define XARM_MSGS__SRV__DETAIL__MOVE_CIRCLE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "xarm_msgs/srv/detail/move_circle__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const MoveCircle_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: pose1
  {
    if (msg.pose1.size() == 0) {
      out << "pose1: []";
    } else {
      out << "pose1: [";
      size_t pending_items = msg.pose1.size();
      for (auto item : msg.pose1) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: pose2
  {
    if (msg.pose2.size() == 0) {
      out << "pose2: []";
    } else {
      out << "pose2: [";
      size_t pending_items = msg.pose2.size();
      for (auto item : msg.pose2) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: percent
  {
    out << "percent: ";
    rosidl_generator_traits::value_to_yaml(msg.percent, out);
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

  // member: is_tool_coord
  {
    out << "is_tool_coord: ";
    rosidl_generator_traits::value_to_yaml(msg.is_tool_coord, out);
    out << ", ";
  }

  // member: is_axis_angle
  {
    out << "is_axis_angle: ";
    rosidl_generator_traits::value_to_yaml(msg.is_axis_angle, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const MoveCircle_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: pose1
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.pose1.size() == 0) {
      out << "pose1: []\n";
    } else {
      out << "pose1:\n";
      for (auto item : msg.pose1) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: pose2
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.pose2.size() == 0) {
      out << "pose2: []\n";
    } else {
      out << "pose2:\n";
      for (auto item : msg.pose2) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: percent
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "percent: ";
    rosidl_generator_traits::value_to_yaml(msg.percent, out);
    out << "\n";
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

  // member: is_tool_coord
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "is_tool_coord: ";
    rosidl_generator_traits::value_to_yaml(msg.is_tool_coord, out);
    out << "\n";
  }

  // member: is_axis_angle
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "is_axis_angle: ";
    rosidl_generator_traits::value_to_yaml(msg.is_axis_angle, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const MoveCircle_Request & msg, bool use_flow_style = false)
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
  const xarm_msgs::srv::MoveCircle_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::MoveCircle_Request & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::MoveCircle_Request>()
{
  return "xarm_msgs::srv::MoveCircle_Request";
}

template<>
inline const char * name<xarm_msgs::srv::MoveCircle_Request>()
{
  return "xarm_msgs/srv/MoveCircle_Request";
}

template<>
struct has_fixed_size<xarm_msgs::srv::MoveCircle_Request>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<xarm_msgs::srv::MoveCircle_Request>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<xarm_msgs::srv::MoveCircle_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const MoveCircle_Response & msg,
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
  const MoveCircle_Response & msg,
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

inline std::string to_yaml(const MoveCircle_Response & msg, bool use_flow_style = false)
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
  const xarm_msgs::srv::MoveCircle_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::MoveCircle_Response & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::MoveCircle_Response>()
{
  return "xarm_msgs::srv::MoveCircle_Response";
}

template<>
inline const char * name<xarm_msgs::srv::MoveCircle_Response>()
{
  return "xarm_msgs/srv/MoveCircle_Response";
}

template<>
struct has_fixed_size<xarm_msgs::srv::MoveCircle_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<xarm_msgs::srv::MoveCircle_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<xarm_msgs::srv::MoveCircle_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<xarm_msgs::srv::MoveCircle>()
{
  return "xarm_msgs::srv::MoveCircle";
}

template<>
inline const char * name<xarm_msgs::srv::MoveCircle>()
{
  return "xarm_msgs/srv/MoveCircle";
}

template<>
struct has_fixed_size<xarm_msgs::srv::MoveCircle>
  : std::integral_constant<
    bool,
    has_fixed_size<xarm_msgs::srv::MoveCircle_Request>::value &&
    has_fixed_size<xarm_msgs::srv::MoveCircle_Response>::value
  >
{
};

template<>
struct has_bounded_size<xarm_msgs::srv::MoveCircle>
  : std::integral_constant<
    bool,
    has_bounded_size<xarm_msgs::srv::MoveCircle_Request>::value &&
    has_bounded_size<xarm_msgs::srv::MoveCircle_Response>::value
  >
{
};

template<>
struct is_service<xarm_msgs::srv::MoveCircle>
  : std::true_type
{
};

template<>
struct is_service_request<xarm_msgs::srv::MoveCircle_Request>
  : std::true_type
{
};

template<>
struct is_service_response<xarm_msgs::srv::MoveCircle_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // XARM_MSGS__SRV__DETAIL__MOVE_CIRCLE__TRAITS_HPP_
