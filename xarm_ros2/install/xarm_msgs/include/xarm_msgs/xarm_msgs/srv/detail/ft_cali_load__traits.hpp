// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from xarm_msgs:srv/FtCaliLoad.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__FT_CALI_LOAD__TRAITS_HPP_
#define XARM_MSGS__SRV__DETAIL__FT_CALI_LOAD__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "xarm_msgs/srv/detail/ft_cali_load__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const FtCaliLoad_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: datas
  {
    if (msg.datas.size() == 0) {
      out << "datas: []";
    } else {
      out << "datas: [";
      size_t pending_items = msg.datas.size();
      for (auto item : msg.datas) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: association_setting_tcp_load
  {
    out << "association_setting_tcp_load: ";
    rosidl_generator_traits::value_to_yaml(msg.association_setting_tcp_load, out);
    out << ", ";
  }

  // member: m
  {
    out << "m: ";
    rosidl_generator_traits::value_to_yaml(msg.m, out);
    out << ", ";
  }

  // member: x
  {
    out << "x: ";
    rosidl_generator_traits::value_to_yaml(msg.x, out);
    out << ", ";
  }

  // member: y
  {
    out << "y: ";
    rosidl_generator_traits::value_to_yaml(msg.y, out);
    out << ", ";
  }

  // member: z
  {
    out << "z: ";
    rosidl_generator_traits::value_to_yaml(msg.z, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const FtCaliLoad_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: datas
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.datas.size() == 0) {
      out << "datas: []\n";
    } else {
      out << "datas:\n";
      for (auto item : msg.datas) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: association_setting_tcp_load
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "association_setting_tcp_load: ";
    rosidl_generator_traits::value_to_yaml(msg.association_setting_tcp_load, out);
    out << "\n";
  }

  // member: m
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "m: ";
    rosidl_generator_traits::value_to_yaml(msg.m, out);
    out << "\n";
  }

  // member: x
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "x: ";
    rosidl_generator_traits::value_to_yaml(msg.x, out);
    out << "\n";
  }

  // member: y
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "y: ";
    rosidl_generator_traits::value_to_yaml(msg.y, out);
    out << "\n";
  }

  // member: z
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "z: ";
    rosidl_generator_traits::value_to_yaml(msg.z, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const FtCaliLoad_Request & msg, bool use_flow_style = false)
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
  const xarm_msgs::srv::FtCaliLoad_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::FtCaliLoad_Request & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::FtCaliLoad_Request>()
{
  return "xarm_msgs::srv::FtCaliLoad_Request";
}

template<>
inline const char * name<xarm_msgs::srv::FtCaliLoad_Request>()
{
  return "xarm_msgs/srv/FtCaliLoad_Request";
}

template<>
struct has_fixed_size<xarm_msgs::srv::FtCaliLoad_Request>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<xarm_msgs::srv::FtCaliLoad_Request>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<xarm_msgs::srv::FtCaliLoad_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const FtCaliLoad_Response & msg,
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
  const FtCaliLoad_Response & msg,
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

inline std::string to_yaml(const FtCaliLoad_Response & msg, bool use_flow_style = false)
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
  const xarm_msgs::srv::FtCaliLoad_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::FtCaliLoad_Response & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::FtCaliLoad_Response>()
{
  return "xarm_msgs::srv::FtCaliLoad_Response";
}

template<>
inline const char * name<xarm_msgs::srv::FtCaliLoad_Response>()
{
  return "xarm_msgs/srv/FtCaliLoad_Response";
}

template<>
struct has_fixed_size<xarm_msgs::srv::FtCaliLoad_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<xarm_msgs::srv::FtCaliLoad_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<xarm_msgs::srv::FtCaliLoad_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<xarm_msgs::srv::FtCaliLoad>()
{
  return "xarm_msgs::srv::FtCaliLoad";
}

template<>
inline const char * name<xarm_msgs::srv::FtCaliLoad>()
{
  return "xarm_msgs/srv/FtCaliLoad";
}

template<>
struct has_fixed_size<xarm_msgs::srv::FtCaliLoad>
  : std::integral_constant<
    bool,
    has_fixed_size<xarm_msgs::srv::FtCaliLoad_Request>::value &&
    has_fixed_size<xarm_msgs::srv::FtCaliLoad_Response>::value
  >
{
};

template<>
struct has_bounded_size<xarm_msgs::srv::FtCaliLoad>
  : std::integral_constant<
    bool,
    has_bounded_size<xarm_msgs::srv::FtCaliLoad_Request>::value &&
    has_bounded_size<xarm_msgs::srv::FtCaliLoad_Response>::value
  >
{
};

template<>
struct is_service<xarm_msgs::srv::FtCaliLoad>
  : std::true_type
{
};

template<>
struct is_service_request<xarm_msgs::srv::FtCaliLoad_Request>
  : std::true_type
{
};

template<>
struct is_service_response<xarm_msgs::srv::FtCaliLoad_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // XARM_MSGS__SRV__DETAIL__FT_CALI_LOAD__TRAITS_HPP_
