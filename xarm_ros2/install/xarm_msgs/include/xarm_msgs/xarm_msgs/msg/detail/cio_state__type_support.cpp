// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from xarm_msgs:msg/CIOState.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "xarm_msgs/msg/detail/cio_state__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace xarm_msgs
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void CIOState_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) xarm_msgs::msg::CIOState(_init);
}

void CIOState_fini_function(void * message_memory)
{
  auto typed_message = static_cast<xarm_msgs::msg::CIOState *>(message_memory);
  typed_message->~CIOState();
}

size_t size_function__CIOState__input_digitals(const void * untyped_member)
{
  (void)untyped_member;
  return 2;
}

const void * get_const_function__CIOState__input_digitals(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::array<uint16_t, 2> *>(untyped_member);
  return &member[index];
}

void * get_function__CIOState__input_digitals(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::array<uint16_t, 2> *>(untyped_member);
  return &member[index];
}

void fetch_function__CIOState__input_digitals(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const uint16_t *>(
    get_const_function__CIOState__input_digitals(untyped_member, index));
  auto & value = *reinterpret_cast<uint16_t *>(untyped_value);
  value = item;
}

void assign_function__CIOState__input_digitals(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<uint16_t *>(
    get_function__CIOState__input_digitals(untyped_member, index));
  const auto & value = *reinterpret_cast<const uint16_t *>(untyped_value);
  item = value;
}

size_t size_function__CIOState__output_digitals(const void * untyped_member)
{
  (void)untyped_member;
  return 2;
}

const void * get_const_function__CIOState__output_digitals(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::array<uint16_t, 2> *>(untyped_member);
  return &member[index];
}

void * get_function__CIOState__output_digitals(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::array<uint16_t, 2> *>(untyped_member);
  return &member[index];
}

void fetch_function__CIOState__output_digitals(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const uint16_t *>(
    get_const_function__CIOState__output_digitals(untyped_member, index));
  auto & value = *reinterpret_cast<uint16_t *>(untyped_value);
  value = item;
}

void assign_function__CIOState__output_digitals(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<uint16_t *>(
    get_function__CIOState__output_digitals(untyped_member, index));
  const auto & value = *reinterpret_cast<const uint16_t *>(untyped_value);
  item = value;
}

size_t size_function__CIOState__input_analogs(const void * untyped_member)
{
  (void)untyped_member;
  return 2;
}

const void * get_const_function__CIOState__input_analogs(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::array<float, 2> *>(untyped_member);
  return &member[index];
}

void * get_function__CIOState__input_analogs(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::array<float, 2> *>(untyped_member);
  return &member[index];
}

void fetch_function__CIOState__input_analogs(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const float *>(
    get_const_function__CIOState__input_analogs(untyped_member, index));
  auto & value = *reinterpret_cast<float *>(untyped_value);
  value = item;
}

void assign_function__CIOState__input_analogs(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<float *>(
    get_function__CIOState__input_analogs(untyped_member, index));
  const auto & value = *reinterpret_cast<const float *>(untyped_value);
  item = value;
}

size_t size_function__CIOState__output_analogs(const void * untyped_member)
{
  (void)untyped_member;
  return 2;
}

const void * get_const_function__CIOState__output_analogs(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::array<float, 2> *>(untyped_member);
  return &member[index];
}

void * get_function__CIOState__output_analogs(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::array<float, 2> *>(untyped_member);
  return &member[index];
}

void fetch_function__CIOState__output_analogs(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const float *>(
    get_const_function__CIOState__output_analogs(untyped_member, index));
  auto & value = *reinterpret_cast<float *>(untyped_value);
  value = item;
}

void assign_function__CIOState__output_analogs(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<float *>(
    get_function__CIOState__output_analogs(untyped_member, index));
  const auto & value = *reinterpret_cast<const float *>(untyped_value);
  item = value;
}

size_t size_function__CIOState__input_conf(const void * untyped_member)
{
  (void)untyped_member;
  return 16;
}

const void * get_const_function__CIOState__input_conf(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::array<uint8_t, 16> *>(untyped_member);
  return &member[index];
}

void * get_function__CIOState__input_conf(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::array<uint8_t, 16> *>(untyped_member);
  return &member[index];
}

void fetch_function__CIOState__input_conf(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const uint8_t *>(
    get_const_function__CIOState__input_conf(untyped_member, index));
  auto & value = *reinterpret_cast<uint8_t *>(untyped_value);
  value = item;
}

void assign_function__CIOState__input_conf(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<uint8_t *>(
    get_function__CIOState__input_conf(untyped_member, index));
  const auto & value = *reinterpret_cast<const uint8_t *>(untyped_value);
  item = value;
}

size_t size_function__CIOState__output_conf(const void * untyped_member)
{
  (void)untyped_member;
  return 16;
}

const void * get_const_function__CIOState__output_conf(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::array<uint8_t, 16> *>(untyped_member);
  return &member[index];
}

void * get_function__CIOState__output_conf(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::array<uint8_t, 16> *>(untyped_member);
  return &member[index];
}

void fetch_function__CIOState__output_conf(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const uint8_t *>(
    get_const_function__CIOState__output_conf(untyped_member, index));
  auto & value = *reinterpret_cast<uint8_t *>(untyped_value);
  value = item;
}

void assign_function__CIOState__output_conf(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<uint8_t *>(
    get_function__CIOState__output_conf(untyped_member, index));
  const auto & value = *reinterpret_cast<const uint8_t *>(untyped_value);
  item = value;
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember CIOState_message_member_array[9] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<std_msgs::msg::Header>(),  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs::msg::CIOState, header),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "state",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs::msg::CIOState, state),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "code",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs::msg::CIOState, code),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "input_digitals",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT16,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    true,  // is array
    2,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs::msg::CIOState, input_digitals),  // bytes offset in struct
    nullptr,  // default value
    size_function__CIOState__input_digitals,  // size() function pointer
    get_const_function__CIOState__input_digitals,  // get_const(index) function pointer
    get_function__CIOState__input_digitals,  // get(index) function pointer
    fetch_function__CIOState__input_digitals,  // fetch(index, &value) function pointer
    assign_function__CIOState__input_digitals,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "output_digitals",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT16,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    true,  // is array
    2,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs::msg::CIOState, output_digitals),  // bytes offset in struct
    nullptr,  // default value
    size_function__CIOState__output_digitals,  // size() function pointer
    get_const_function__CIOState__output_digitals,  // get_const(index) function pointer
    get_function__CIOState__output_digitals,  // get(index) function pointer
    fetch_function__CIOState__output_digitals,  // fetch(index, &value) function pointer
    assign_function__CIOState__output_digitals,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "input_analogs",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    true,  // is array
    2,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs::msg::CIOState, input_analogs),  // bytes offset in struct
    nullptr,  // default value
    size_function__CIOState__input_analogs,  // size() function pointer
    get_const_function__CIOState__input_analogs,  // get_const(index) function pointer
    get_function__CIOState__input_analogs,  // get(index) function pointer
    fetch_function__CIOState__input_analogs,  // fetch(index, &value) function pointer
    assign_function__CIOState__input_analogs,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "output_analogs",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    true,  // is array
    2,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs::msg::CIOState, output_analogs),  // bytes offset in struct
    nullptr,  // default value
    size_function__CIOState__output_analogs,  // size() function pointer
    get_const_function__CIOState__output_analogs,  // get_const(index) function pointer
    get_function__CIOState__output_analogs,  // get(index) function pointer
    fetch_function__CIOState__output_analogs,  // fetch(index, &value) function pointer
    assign_function__CIOState__output_analogs,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "input_conf",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    true,  // is array
    16,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs::msg::CIOState, input_conf),  // bytes offset in struct
    nullptr,  // default value
    size_function__CIOState__input_conf,  // size() function pointer
    get_const_function__CIOState__input_conf,  // get_const(index) function pointer
    get_function__CIOState__input_conf,  // get(index) function pointer
    fetch_function__CIOState__input_conf,  // fetch(index, &value) function pointer
    assign_function__CIOState__input_conf,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "output_conf",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    true,  // is array
    16,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs::msg::CIOState, output_conf),  // bytes offset in struct
    nullptr,  // default value
    size_function__CIOState__output_conf,  // size() function pointer
    get_const_function__CIOState__output_conf,  // get_const(index) function pointer
    get_function__CIOState__output_conf,  // get(index) function pointer
    fetch_function__CIOState__output_conf,  // fetch(index, &value) function pointer
    assign_function__CIOState__output_conf,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers CIOState_message_members = {
  "xarm_msgs::msg",  // message namespace
  "CIOState",  // message name
  9,  // number of fields
  sizeof(xarm_msgs::msg::CIOState),
  CIOState_message_member_array,  // message members
  CIOState_init_function,  // function to initialize message memory (memory has to be allocated)
  CIOState_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t CIOState_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &CIOState_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace xarm_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<xarm_msgs::msg::CIOState>()
{
  return &::xarm_msgs::msg::rosidl_typesupport_introspection_cpp::CIOState_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, xarm_msgs, msg, CIOState)() {
  return &::xarm_msgs::msg::rosidl_typesupport_introspection_cpp::CIOState_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
