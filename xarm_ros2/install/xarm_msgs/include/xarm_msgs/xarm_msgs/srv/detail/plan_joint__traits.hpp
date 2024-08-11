// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from xarm_msgs:srv/PlanJoint.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__PLAN_JOINT__TRAITS_HPP_
#define XARM_MSGS__SRV__DETAIL__PLAN_JOINT__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "xarm_msgs/srv/detail/plan_joint__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const PlanJoint_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: target
  {
    if (msg.target.size() == 0) {
      out << "target: []";
    } else {
      out << "target: [";
      size_t pending_items = msg.target.size();
      for (auto item : msg.target) {
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
  const PlanJoint_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: target
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.target.size() == 0) {
      out << "target: []\n";
    } else {
      out << "target:\n";
      for (auto item : msg.target) {
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

inline std::string to_yaml(const PlanJoint_Request & msg, bool use_flow_style = false)
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
  const xarm_msgs::srv::PlanJoint_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::PlanJoint_Request & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::PlanJoint_Request>()
{
  return "xarm_msgs::srv::PlanJoint_Request";
}

template<>
inline const char * name<xarm_msgs::srv::PlanJoint_Request>()
{
  return "xarm_msgs/srv/PlanJoint_Request";
}

template<>
struct has_fixed_size<xarm_msgs::srv::PlanJoint_Request>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<xarm_msgs::srv::PlanJoint_Request>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<xarm_msgs::srv::PlanJoint_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace xarm_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const PlanJoint_Response & msg,
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
  const PlanJoint_Response & msg,
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

inline std::string to_yaml(const PlanJoint_Response & msg, bool use_flow_style = false)
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
  const xarm_msgs::srv::PlanJoint_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  xarm_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xarm_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xarm_msgs::srv::PlanJoint_Response & msg)
{
  return xarm_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xarm_msgs::srv::PlanJoint_Response>()
{
  return "xarm_msgs::srv::PlanJoint_Response";
}

template<>
inline const char * name<xarm_msgs::srv::PlanJoint_Response>()
{
  return "xarm_msgs/srv/PlanJoint_Response";
}

template<>
struct has_fixed_size<xarm_msgs::srv::PlanJoint_Response>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<xarm_msgs::srv::PlanJoint_Response>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<xarm_msgs::srv::PlanJoint_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<xarm_msgs::srv::PlanJoint>()
{
  return "xarm_msgs::srv::PlanJoint";
}

template<>
inline const char * name<xarm_msgs::srv::PlanJoint>()
{
  return "xarm_msgs/srv/PlanJoint";
}

template<>
struct has_fixed_size<xarm_msgs::srv::PlanJoint>
  : std::integral_constant<
    bool,
    has_fixed_size<xarm_msgs::srv::PlanJoint_Request>::value &&
    has_fixed_size<xarm_msgs::srv::PlanJoint_Response>::value
  >
{
};

template<>
struct has_bounded_size<xarm_msgs::srv::PlanJoint>
  : std::integral_constant<
    bool,
    has_bounded_size<xarm_msgs::srv::PlanJoint_Request>::value &&
    has_bounded_size<xarm_msgs::srv::PlanJoint_Response>::value
  >
{
};

template<>
struct is_service<xarm_msgs::srv::PlanJoint>
  : std::true_type
{
};

template<>
struct is_service_request<xarm_msgs::srv::PlanJoint_Request>
  : std::true_type
{
};

template<>
struct is_service_response<xarm_msgs::srv::PlanJoint_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // XARM_MSGS__SRV__DETAIL__PLAN_JOINT__TRAITS_HPP_
