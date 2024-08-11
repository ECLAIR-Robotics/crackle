// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from xarm_msgs:msg/RobotMsg.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__MSG__DETAIL__ROBOT_MSG__TRAITS_HPP_
#define XARM_MSGS__MSG__DETAIL__ROBOT_MSG__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "xarm_msgs/msg/detail/robot_msg__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace xarm_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const RobotMsg & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: state
  {
    out << "state: ";
    rosidl_generator_traits::value_to_yaml(msg.state, out);
    out << ", ";
  }

  // member: mode
  {
    out << "mode: ";
    rosidl_generator_traits::value_to_yaml(msg.mode, out);
    out << ", ";
  }

  // member: cmdnum
  {
    out << "cmdnum: ";
    rosidl_generator_traits::value_to_yaml(msg.cmdnum, out);
    out << ", ";
  }

  // member: mt_brake
  {
    out << "mt_brake: ";
    rosidl_generator_traits::value_to_yaml(msg.mt_brake, out);
    out << ", ";
  }

  // member: mt_able
  {
    out << "mt_able: ";
    rosidl_generator_traits::value_to_yaml(msg.mt_able, out);
    out << ", ";
  }

  // member: err
  {
    out << "err: ";
    rosidl_generator_traits::value_to_yaml(msg.err, out);
    out << ", ";
  }

  // member: warn
  {
    out << "warn: ";
    rosidl_generator_traits::value_to_yaml(msg.warn, out);
    out << ", ";
  }

  // member: angle
  {
    if (msg.angle.size() == 0) {
      out << "angle: []";
    } else {
      out << "angle: [";
      size_t pending_items = msg.angle.size();
      for (auto item : msg.angle) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

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

  // member: offset
  {
    if (msg.offset.size() == 0) {
      out << "offset: []";
    } else {
      out << "offset: [";
      size_t pending_items = msg.offset.size();
      for (auto item : msg.offset) {
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
  const RobotMsg & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: header
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "header:\n";
    to_block_style_yaml(msg.header, out, indentation + 2);
  }

  // member: state
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "state: ";
    rosidl_generator_traits::value_to_yaml(msg.state, out);
    out << "\n";
  }

  // member: mode
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "mode: ";
    rosidl_generator_traits::value_to_yaml(msg.mode, out);
    out << "\n";
  }

  // member: cmdnum
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "cmdnum: ";
    rosidl_generator_traits::value_to_yaml(msg.cmdnum, out);
    out << "\n";
  }

  // member: mt_brake
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "mt_brake: ";
    rosidl_generator_traits::value_to_yaml(msg.mt_brake, out);
    out << "\n";
  }

  // member: mt_able
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "mt_able: ";
    rosidl_generator_traits::value_to_yaml(msg.mt_able, out);
    out << "\n";
  }

  // member: err
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "err: ";
    rosidl_generator_traits::value_to_yaml(msg.err, out);
    out << "\n";
  }

  // member: warn
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "warn: ";
    rosidl_generator_traits::value_to_yaml(msg.warn, out);
    out << "\n";
  }

  // member: angle
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.angle.size() == 0) {
      out << "angle: []\n";
    } else {
      out << "angle:\n";
      for (auto item : msg.angle) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

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

  // member: offset
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.offset.size() == 0) {
      out << "offset: []\n";
    } else {
      out << "offset:\n";
      for (auto item : msg.offset) {
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

inline std::string to_yaml(const RobotMsg & msg, bool use_flow_style = false)
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
  const xarm_msgs::msg::RobotMsg & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::msg::RobotMsg & msg)
{
  return xarm_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::msg::RobotMsg>()
{
  return "xarm_msgs::msg::RobotMsg";
}

template<>
inline const char * name<xarm_msgs::msg::RobotMsg>()
{
  return "xarm_msgs/msg/RobotMsg";
}

template<>
struct has_fixed_size<xarm_msgs::msg::RobotMsg>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<xarm_msgs::msg::RobotMsg>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<xarm_msgs::msg::RobotMsg>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // XARM_MSGS__MSG__DETAIL__ROBOT_MSG__TRAITS_HPP_
