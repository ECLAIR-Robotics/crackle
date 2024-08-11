// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from xarm_msgs:srv/FtForceConfig.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__FT_FORCE_CONFIG__TRAITS_HPP_
#define XARM_MSGS__SRV__DETAIL__FT_FORCE_CONFIG__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "xarm_msgs/srv/detail/ft_force_config__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const FtForceConfig_Request & msg,
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

  // member: ref
  {
    if (msg.ref.size() == 0) {
      out << "ref: []";
    } else {
      out << "ref: [";
      size_t pending_items = msg.ref.size();
      for (auto item : msg.ref) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: limits
  {
    if (msg.limits.size() == 0) {
      out << "limits: []";
    } else {
      out << "limits: [";
      size_t pending_items = msg.limits.size();
      for (auto item : msg.limits) {
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
  const FtForceConfig_Request & msg,
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

  // member: ref
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.ref.size() == 0) {
      out << "ref: []\n";
    } else {
      out << "ref:\n";
      for (auto item : msg.ref) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: limits
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.limits.size() == 0) {
      out << "limits: []\n";
    } else {
      out << "limits:\n";
      for (auto item : msg.limits) {
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

inline std::string to_yaml(const FtForceConfig_Request & msg, bool use_flow_style = false)
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
  const xarm_msgs::srv::FtForceConfig_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::FtForceConfig_Request & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::FtForceConfig_Request>()
{
  return "xarm_msgs::srv::FtForceConfig_Request";
}

template<>
inline const char * name<xarm_msgs::srv::FtForceConfig_Request>()
{
  return "xarm_msgs/srv/FtForceConfig_Request";
}

template<>
struct has_fixed_size<xarm_msgs::srv::FtForceConfig_Request>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<xarm_msgs::srv::FtForceConfig_Request>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<xarm_msgs::srv::FtForceConfig_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const FtForceConfig_Response & msg,
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
  const FtForceConfig_Response & msg,
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

inline std::string to_yaml(const FtForceConfig_Response & msg, bool use_flow_style = false)
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
  const xarm_msgs::srv::FtForceConfig_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::FtForceConfig_Response & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::FtForceConfig_Response>()
{
  return "xarm_msgs::srv::FtForceConfig_Response";
}

template<>
inline const char * name<xarm_msgs::srv::FtForceConfig_Response>()
{
  return "xarm_msgs/srv/FtForceConfig_Response";
}

template<>
struct has_fixed_size<xarm_msgs::srv::FtForceConfig_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<xarm_msgs::srv::FtForceConfig_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<xarm_msgs::srv::FtForceConfig_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<xarm_msgs::srv::FtForceConfig>()
{
  return "xarm_msgs::srv::FtForceConfig";
}

template<>
inline const char * name<xarm_msgs::srv::FtForceConfig>()
{
  return "xarm_msgs/srv/FtForceConfig";
}

template<>
struct has_fixed_size<xarm_msgs::srv::FtForceConfig>
  : std::integral_constant<
    bool,
    has_fixed_size<xarm_msgs::srv::FtForceConfig_Request>::value &&
    has_fixed_size<xarm_msgs::srv::FtForceConfig_Response>::value
  >
{
};

template<>
struct has_bounded_size<xarm_msgs::srv::FtForceConfig>
  : std::integral_constant<
    bool,
    has_bounded_size<xarm_msgs::srv::FtForceConfig_Request>::value &&
    has_bounded_size<xarm_msgs::srv::FtForceConfig_Response>::value
  >
{
};

template<>
struct is_service<xarm_msgs::srv::FtForceConfig>
  : std::true_type
{
};

template<>
struct is_service_request<xarm_msgs::srv::FtForceConfig_Request>
  : std::true_type
{
};

template<>
struct is_service_response<xarm_msgs::srv::FtForceConfig_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // XARM_MSGS__SRV__DETAIL__FT_FORCE_CONFIG__TRAITS_HPP_
