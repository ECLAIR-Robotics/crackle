// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from xarm_msgs:srv/PlanExec.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__PLAN_EXEC__TRAITS_HPP_
#define XARM_MSGS__SRV__DETAIL__PLAN_EXEC__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "xarm_msgs/srv/detail/plan_exec__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const PlanExec_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: wait
  {
    out << "wait: ";
    rosidl_generator_traits::value_to_yaml(msg.wait, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const PlanExec_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: wait
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "wait: ";
    rosidl_generator_traits::value_to_yaml(msg.wait, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const PlanExec_Request & msg, bool use_flow_style = false)
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
  const xarm_msgs::srv::PlanExec_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::PlanExec_Request & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::PlanExec_Request>()
{
  return "xarm_msgs::srv::PlanExec_Request";
}

template<>
inline const char * name<xarm_msgs::srv::PlanExec_Request>()
{
  return "xarm_msgs/srv/PlanExec_Request";
}

template<>
struct has_fixed_size<xarm_msgs::srv::PlanExec_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<xarm_msgs::srv::PlanExec_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<xarm_msgs::srv::PlanExec_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const PlanExec_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: success
  {
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const PlanExec_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: success
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const PlanExec_Response & msg, bool use_flow_style = false)
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
  const xarm_msgs::srv::PlanExec_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::PlanExec_Response & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::PlanExec_Response>()
{
  return "xarm_msgs::srv::PlanExec_Response";
}

template<>
inline const char * name<xarm_msgs::srv::PlanExec_Response>()
{
  return "xarm_msgs/srv/PlanExec_Response";
}

template<>
struct has_fixed_size<xarm_msgs::srv::PlanExec_Response>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<xarm_msgs::srv::PlanExec_Response>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<xarm_msgs::srv::PlanExec_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<xarm_msgs::srv::PlanExec>()
{
  return "xarm_msgs::srv::PlanExec";
}

template<>
inline const char * name<xarm_msgs::srv::PlanExec>()
{
  return "xarm_msgs/srv/PlanExec";
}

template<>
struct has_fixed_size<xarm_msgs::srv::PlanExec>
  : std::integral_constant<
    bool,
    has_fixed_size<xarm_msgs::srv::PlanExec_Request>::value &&
    has_fixed_size<xarm_msgs::srv::PlanExec_Response>::value
  >
{
};

template<>
struct has_bounded_size<xarm_msgs::srv::PlanExec>
  : std::integral_constant<
    bool,
    has_bounded_size<xarm_msgs::srv::PlanExec_Request>::value &&
    has_bounded_size<xarm_msgs::srv::PlanExec_Response>::value
  >
{
};

template<>
struct is_service<xarm_msgs::srv::PlanExec>
  : std::true_type
{
};

template<>
struct is_service_request<xarm_msgs::srv::PlanExec_Request>
  : std::true_type
{
};

template<>
struct is_service_response<xarm_msgs::srv::PlanExec_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // XARM_MSGS__SRV__DETAIL__PLAN_EXEC__TRAITS_HPP_
