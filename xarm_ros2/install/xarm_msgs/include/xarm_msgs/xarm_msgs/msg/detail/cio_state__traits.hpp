// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from xarm_msgs:msg/CIOState.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__MSG__DETAIL__CIO_STATE__TRAITS_HPP_
#define XARM_MSGS__MSG__DETAIL__CIO_STATE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "xarm_msgs/msg/detail/cio_state__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace xarm_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const CIOState & msg,
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

  // member: code
  {
    out << "code: ";
    rosidl_generator_traits::value_to_yaml(msg.code, out);
    out << ", ";
  }

  // member: input_digitals
  {
    if (msg.input_digitals.size() == 0) {
      out << "input_digitals: []";
    } else {
      out << "input_digitals: [";
      size_t pending_items = msg.input_digitals.size();
      for (auto item : msg.input_digitals) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: output_digitals
  {
    if (msg.output_digitals.size() == 0) {
      out << "output_digitals: []";
    } else {
      out << "output_digitals: [";
      size_t pending_items = msg.output_digitals.size();
      for (auto item : msg.output_digitals) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: input_analogs
  {
    if (msg.input_analogs.size() == 0) {
      out << "input_analogs: []";
    } else {
      out << "input_analogs: [";
      size_t pending_items = msg.input_analogs.size();
      for (auto item : msg.input_analogs) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: output_analogs
  {
    if (msg.output_analogs.size() == 0) {
      out << "output_analogs: []";
    } else {
      out << "output_analogs: [";
      size_t pending_items = msg.output_analogs.size();
      for (auto item : msg.output_analogs) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: input_conf
  {
    if (msg.input_conf.size() == 0) {
      out << "input_conf: []";
    } else {
      out << "input_conf: [";
      size_t pending_items = msg.input_conf.size();
      for (auto item : msg.input_conf) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: output_conf
  {
    if (msg.output_conf.size() == 0) {
      out << "output_conf: []";
    } else {
      out << "output_conf: [";
      size_t pending_items = msg.output_conf.size();
      for (auto item : msg.output_conf) {
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
  const CIOState & msg,
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

  // member: code
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "code: ";
    rosidl_generator_traits::value_to_yaml(msg.code, out);
    out << "\n";
  }

  // member: input_digitals
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.input_digitals.size() == 0) {
      out << "input_digitals: []\n";
    } else {
      out << "input_digitals:\n";
      for (auto item : msg.input_digitals) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: output_digitals
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.output_digitals.size() == 0) {
      out << "output_digitals: []\n";
    } else {
      out << "output_digitals:\n";
      for (auto item : msg.output_digitals) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: input_analogs
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.input_analogs.size() == 0) {
      out << "input_analogs: []\n";
    } else {
      out << "input_analogs:\n";
      for (auto item : msg.input_analogs) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: output_analogs
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.output_analogs.size() == 0) {
      out << "output_analogs: []\n";
    } else {
      out << "output_analogs:\n";
      for (auto item : msg.output_analogs) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: input_conf
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.input_conf.size() == 0) {
      out << "input_conf: []\n";
    } else {
      out << "input_conf:\n";
      for (auto item : msg.input_conf) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: output_conf
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.output_conf.size() == 0) {
      out << "output_conf: []\n";
    } else {
      out << "output_conf:\n";
      for (auto item : msg.output_conf) {
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

inline std::string to_yaml(const CIOState & msg, bool use_flow_style = false)
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
  const xarm_msgs::msg::CIOState & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::msg::CIOState & msg)
{
  return xarm_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::msg::CIOState>()
{
  return "xarm_msgs::msg::CIOState";
}

template<>
inline const char * name<xarm_msgs::msg::CIOState>()
{
  return "xarm_msgs/msg/CIOState";
}

template<>
struct has_fixed_size<xarm_msgs::msg::CIOState>
  : std::integral_constant<bool, has_fixed_size<std_msgs::msg::Header>::value> {};

template<>
struct has_bounded_size<xarm_msgs::msg::CIOState>
  : std::integral_constant<bool, has_bounded_size<std_msgs::msg::Header>::value> {};

template<>
struct is_message<xarm_msgs::msg::CIOState>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // XARM_MSGS__MSG__DETAIL__CIO_STATE__TRAITS_HPP_
