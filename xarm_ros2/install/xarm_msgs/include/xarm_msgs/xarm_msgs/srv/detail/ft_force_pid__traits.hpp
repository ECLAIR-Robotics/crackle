// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from xarm_msgs:srv/FtForcePid.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__FT_FORCE_PID__TRAITS_HPP_
#define XARM_MSGS__SRV__DETAIL__FT_FORCE_PID__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "xarm_msgs/srv/detail/ft_force_pid__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const FtForcePid_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: kp
  {
    if (msg.kp.size() == 0) {
      out << "kp: []";
    } else {
      out << "kp: [";
      size_t pending_items = msg.kp.size();
      for (auto item : msg.kp) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: ki
  {
    if (msg.ki.size() == 0) {
      out << "ki: []";
    } else {
      out << "ki: [";
      size_t pending_items = msg.ki.size();
      for (auto item : msg.ki) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: kd
  {
    if (msg.kd.size() == 0) {
      out << "kd: []";
    } else {
      out << "kd: [";
      size_t pending_items = msg.kd.size();
      for (auto item : msg.kd) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: xe_limit
  {
    if (msg.xe_limit.size() == 0) {
      out << "xe_limit: []";
    } else {
      out << "xe_limit: [";
      size_t pending_items = msg.xe_limit.size();
      for (auto item : msg.xe_limit) {
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
  const FtForcePid_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: kp
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.kp.size() == 0) {
      out << "kp: []\n";
    } else {
      out << "kp:\n";
      for (auto item : msg.kp) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: ki
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.ki.size() == 0) {
      out << "ki: []\n";
    } else {
      out << "ki:\n";
      for (auto item : msg.ki) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: kd
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.kd.size() == 0) {
      out << "kd: []\n";
    } else {
      out << "kd:\n";
      for (auto item : msg.kd) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: xe_limit
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.xe_limit.size() == 0) {
      out << "xe_limit: []\n";
    } else {
      out << "xe_limit:\n";
      for (auto item : msg.xe_limit) {
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

inline std::string to_yaml(const FtForcePid_Request & msg, bool use_flow_style = false)
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
  const xarm_msgs::srv::FtForcePid_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::FtForcePid_Request & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::FtForcePid_Request>()
{
  return "xarm_msgs::srv::FtForcePid_Request";
}

template<>
inline const char * name<xarm_msgs::srv::FtForcePid_Request>()
{
  return "xarm_msgs/srv/FtForcePid_Request";
}

template<>
struct has_fixed_size<xarm_msgs::srv::FtForcePid_Request>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<xarm_msgs::srv::FtForcePid_Request>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<xarm_msgs::srv::FtForcePid_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const FtForcePid_Response & msg,
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
  const FtForcePid_Response & msg,
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

inline std::string to_yaml(const FtForcePid_Response & msg, bool use_flow_style = false)
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
  const xarm_msgs::srv::FtForcePid_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::FtForcePid_Response & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::FtForcePid_Response>()
{
  return "xarm_msgs::srv::FtForcePid_Response";
}

template<>
inline const char * name<xarm_msgs::srv::FtForcePid_Response>()
{
  return "xarm_msgs/srv/FtForcePid_Response";
}

template<>
struct has_fixed_size<xarm_msgs::srv::FtForcePid_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<xarm_msgs::srv::FtForcePid_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<xarm_msgs::srv::FtForcePid_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<xarm_msgs::srv::FtForcePid>()
{
  return "xarm_msgs::srv::FtForcePid";
}

template<>
inline const char * name<xarm_msgs::srv::FtForcePid>()
{
  return "xarm_msgs/srv/FtForcePid";
}

template<>
struct has_fixed_size<xarm_msgs::srv::FtForcePid>
  : std::integral_constant<
    bool,
    has_fixed_size<xarm_msgs::srv::FtForcePid_Request>::value &&
    has_fixed_size<xarm_msgs::srv::FtForcePid_Response>::value
  >
{
};

template<>
struct has_bounded_size<xarm_msgs::srv::FtForcePid>
  : std::integral_constant<
    bool,
    has_bounded_size<xarm_msgs::srv::FtForcePid_Request>::value &&
    has_bounded_size<xarm_msgs::srv::FtForcePid_Response>::value
  >
{
};

template<>
struct is_service<xarm_msgs::srv::FtForcePid>
  : std::true_type
{
};

template<>
struct is_service_request<xarm_msgs::srv::FtForcePid_Request>
  : std::true_type
{
};

template<>
struct is_service_response<xarm_msgs::srv::FtForcePid_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // XARM_MSGS__SRV__DETAIL__FT_FORCE_PID__TRAITS_HPP_
