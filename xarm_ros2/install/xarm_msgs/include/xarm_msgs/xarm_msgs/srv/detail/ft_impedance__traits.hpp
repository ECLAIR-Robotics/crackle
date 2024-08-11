// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from xarm_msgs:srv/FtImpedance.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__FT_IMPEDANCE__TRAITS_HPP_
#define XARM_MSGS__SRV__DETAIL__FT_IMPEDANCE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "xarm_msgs/srv/detail/ft_impedance__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const FtImpedance_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: coord
  {
    out << "coord: ";
    rosidl_generator_traits::value_to_yaml(msg.coord, out);
    out << ", ";
  }

  // member: c_axis
  {
    if (msg.c_axis.size() == 0) {
      out << "c_axis: []";
    } else {
      out << "c_axis: [";
      size_t pending_items = msg.c_axis.size();
      for (auto item : msg.c_axis) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: m
  {
    if (msg.m.size() == 0) {
      out << "m: []";
    } else {
      out << "m: [";
      size_t pending_items = msg.m.size();
      for (auto item : msg.m) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: k
  {
    if (msg.k.size() == 0) {
      out << "k: []";
    } else {
      out << "k: [";
      size_t pending_items = msg.k.size();
      for (auto item : msg.k) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: b
  {
    if (msg.b.size() == 0) {
      out << "b: []";
    } else {
      out << "b: [";
      size_t pending_items = msg.b.size();
      for (auto item : msg.b) {
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
  const FtImpedance_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: coord
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "coord: ";
    rosidl_generator_traits::value_to_yaml(msg.coord, out);
    out << "\n";
  }

  // member: c_axis
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.c_axis.size() == 0) {
      out << "c_axis: []\n";
    } else {
      out << "c_axis:\n";
      for (auto item : msg.c_axis) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: m
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.m.size() == 0) {
      out << "m: []\n";
    } else {
      out << "m:\n";
      for (auto item : msg.m) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: k
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.k.size() == 0) {
      out << "k: []\n";
    } else {
      out << "k:\n";
      for (auto item : msg.k) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: b
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.b.size() == 0) {
      out << "b: []\n";
    } else {
      out << "b:\n";
      for (auto item : msg.b) {
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

inline std::string to_yaml(const FtImpedance_Request & msg, bool use_flow_style = false)
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
  const xarm_msgs::srv::FtImpedance_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::FtImpedance_Request & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::FtImpedance_Request>()
{
  return "xarm_msgs::srv::FtImpedance_Request";
}

template<>
inline const char * name<xarm_msgs::srv::FtImpedance_Request>()
{
  return "xarm_msgs/srv/FtImpedance_Request";
}

template<>
struct has_fixed_size<xarm_msgs::srv::FtImpedance_Request>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<xarm_msgs::srv::FtImpedance_Request>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<xarm_msgs::srv::FtImpedance_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const FtImpedance_Response & msg,
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
  const FtImpedance_Response & msg,
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

inline std::string to_yaml(const FtImpedance_Response & msg, bool use_flow_style = false)
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
  const xarm_msgs::srv::FtImpedance_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::FtImpedance_Response & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::FtImpedance_Response>()
{
  return "xarm_msgs::srv::FtImpedance_Response";
}

template<>
inline const char * name<xarm_msgs::srv::FtImpedance_Response>()
{
  return "xarm_msgs/srv/FtImpedance_Response";
}

template<>
struct has_fixed_size<xarm_msgs::srv::FtImpedance_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<xarm_msgs::srv::FtImpedance_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<xarm_msgs::srv::FtImpedance_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<xarm_msgs::srv::FtImpedance>()
{
  return "xarm_msgs::srv::FtImpedance";
}

template<>
inline const char * name<xarm_msgs::srv::FtImpedance>()
{
  return "xarm_msgs/srv/FtImpedance";
}

template<>
struct has_fixed_size<xarm_msgs::srv::FtImpedance>
  : std::integral_constant<
    bool,
    has_fixed_size<xarm_msgs::srv::FtImpedance_Request>::value &&
    has_fixed_size<xarm_msgs::srv::FtImpedance_Response>::value
  >
{
};

template<>
struct has_bounded_size<xarm_msgs::srv::FtImpedance>
  : std::integral_constant<
    bool,
    has_bounded_size<xarm_msgs::srv::FtImpedance_Request>::value &&
    has_bounded_size<xarm_msgs::srv::FtImpedance_Response>::value
  >
{
};

template<>
struct is_service<xarm_msgs::srv::FtImpedance>
  : std::true_type
{
};

template<>
struct is_service_request<xarm_msgs::srv::FtImpedance_Request>
  : std::true_type
{
};

template<>
struct is_service_response<xarm_msgs::srv::FtImpedance_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // XARM_MSGS__SRV__DETAIL__FT_IMPEDANCE__TRAITS_HPP_
