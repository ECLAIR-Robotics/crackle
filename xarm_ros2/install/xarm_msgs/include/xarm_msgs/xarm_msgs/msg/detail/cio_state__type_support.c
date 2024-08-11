// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from xarm_msgs:msg/CIOState.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "xarm_msgs/msg/detail/cio_state__rosidl_typesupport_introspection_c.h"
#include "xarm_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "xarm_msgs/msg/detail/cio_state__functions.h"
#include "xarm_msgs/msg/detail/cio_state__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__CIOState_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  xarm_msgs__msg__CIOState__init(message_memory);
}

void xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__CIOState_fini_function(void * message_memory)
{
  xarm_msgs__msg__CIOState__fini(message_memory);
}

size_t xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__size_function__CIOState__input_digitals(
  const void * untyped_member)
{
  (void)untyped_member;
  return 2;
}

const void * xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_const_function__CIOState__input_digitals(
  const void * untyped_member, size_t index)
{
  const uint16_t * member =
    (const uint16_t *)(untyped_member);
  return &member[index];
}

void * xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_function__CIOState__input_digitals(
  void * untyped_member, size_t index)
{
  uint16_t * member =
    (uint16_t *)(untyped_member);
  return &member[index];
}

void xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__fetch_function__CIOState__input_digitals(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint16_t * item =
    ((const uint16_t *)
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_const_function__CIOState__input_digitals(untyped_member, index));
  uint16_t * value =
    (uint16_t *)(untyped_value);
  *value = *item;
}

void xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__assign_function__CIOState__input_digitals(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint16_t * item =
    ((uint16_t *)
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_function__CIOState__input_digitals(untyped_member, index));
  const uint16_t * value =
    (const uint16_t *)(untyped_value);
  *item = *value;
}

size_t xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__size_function__CIOState__output_digitals(
  const void * untyped_member)
{
  (void)untyped_member;
  return 2;
}

const void * xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_const_function__CIOState__output_digitals(
  const void * untyped_member, size_t index)
{
  const uint16_t * member =
    (const uint16_t *)(untyped_member);
  return &member[index];
}

void * xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_function__CIOState__output_digitals(
  void * untyped_member, size_t index)
{
  uint16_t * member =
    (uint16_t *)(untyped_member);
  return &member[index];
}

void xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__fetch_function__CIOState__output_digitals(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint16_t * item =
    ((const uint16_t *)
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_const_function__CIOState__output_digitals(untyped_member, index));
  uint16_t * value =
    (uint16_t *)(untyped_value);
  *value = *item;
}

void xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__assign_function__CIOState__output_digitals(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint16_t * item =
    ((uint16_t *)
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_function__CIOState__output_digitals(untyped_member, index));
  const uint16_t * value =
    (const uint16_t *)(untyped_value);
  *item = *value;
}

size_t xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__size_function__CIOState__input_analogs(
  const void * untyped_member)
{
  (void)untyped_member;
  return 2;
}

const void * xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_const_function__CIOState__input_analogs(
  const void * untyped_member, size_t index)
{
  const float * member =
    (const float *)(untyped_member);
  return &member[index];
}

void * xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_function__CIOState__input_analogs(
  void * untyped_member, size_t index)
{
  float * member =
    (float *)(untyped_member);
  return &member[index];
}

void xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__fetch_function__CIOState__input_analogs(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_const_function__CIOState__input_analogs(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__assign_function__CIOState__input_analogs(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_function__CIOState__input_analogs(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

size_t xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__size_function__CIOState__output_analogs(
  const void * untyped_member)
{
  (void)untyped_member;
  return 2;
}

const void * xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_const_function__CIOState__output_analogs(
  const void * untyped_member, size_t index)
{
  const float * member =
    (const float *)(untyped_member);
  return &member[index];
}

void * xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_function__CIOState__output_analogs(
  void * untyped_member, size_t index)
{
  float * member =
    (float *)(untyped_member);
  return &member[index];
}

void xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__fetch_function__CIOState__output_analogs(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_const_function__CIOState__output_analogs(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__assign_function__CIOState__output_analogs(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_function__CIOState__output_analogs(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

size_t xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__size_function__CIOState__input_conf(
  const void * untyped_member)
{
  (void)untyped_member;
  return 16;
}

const void * xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_const_function__CIOState__input_conf(
  const void * untyped_member, size_t index)
{
  const uint8_t * member =
    (const uint8_t *)(untyped_member);
  return &member[index];
}

void * xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_function__CIOState__input_conf(
  void * untyped_member, size_t index)
{
  uint8_t * member =
    (uint8_t *)(untyped_member);
  return &member[index];
}

void xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__fetch_function__CIOState__input_conf(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint8_t * item =
    ((const uint8_t *)
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_const_function__CIOState__input_conf(untyped_member, index));
  uint8_t * value =
    (uint8_t *)(untyped_value);
  *value = *item;
}

void xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__assign_function__CIOState__input_conf(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint8_t * item =
    ((uint8_t *)
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_function__CIOState__input_conf(untyped_member, index));
  const uint8_t * value =
    (const uint8_t *)(untyped_value);
  *item = *value;
}

size_t xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__size_function__CIOState__output_conf(
  const void * untyped_member)
{
  (void)untyped_member;
  return 16;
}

const void * xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_const_function__CIOState__output_conf(
  const void * untyped_member, size_t index)
{
  const uint8_t * member =
    (const uint8_t *)(untyped_member);
  return &member[index];
}

void * xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_function__CIOState__output_conf(
  void * untyped_member, size_t index)
{
  uint8_t * member =
    (uint8_t *)(untyped_member);
  return &member[index];
}

void xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__fetch_function__CIOState__output_conf(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint8_t * item =
    ((const uint8_t *)
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_const_function__CIOState__output_conf(untyped_member, index));
  uint8_t * value =
    (uint8_t *)(untyped_value);
  *value = *item;
}

void xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__assign_function__CIOState__output_conf(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint8_t * item =
    ((uint8_t *)
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_function__CIOState__output_conf(untyped_member, index));
  const uint8_t * value =
    (const uint8_t *)(untyped_value);
  *item = *value;
}

static rosidl_typesupport_introspection_c__MessageMember xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__CIOState_message_member_array[9] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__msg__CIOState, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "state",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__msg__CIOState, state),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "code",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__msg__CIOState, code),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "input_digitals",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    2,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__msg__CIOState, input_digitals),  // bytes offset in struct
    NULL,  // default value
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__size_function__CIOState__input_digitals,  // size() function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_const_function__CIOState__input_digitals,  // get_const(index) function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_function__CIOState__input_digitals,  // get(index) function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__fetch_function__CIOState__input_digitals,  // fetch(index, &value) function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__assign_function__CIOState__input_digitals,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "output_digitals",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    2,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__msg__CIOState, output_digitals),  // bytes offset in struct
    NULL,  // default value
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__size_function__CIOState__output_digitals,  // size() function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_const_function__CIOState__output_digitals,  // get_const(index) function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_function__CIOState__output_digitals,  // get(index) function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__fetch_function__CIOState__output_digitals,  // fetch(index, &value) function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__assign_function__CIOState__output_digitals,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "input_analogs",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    2,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__msg__CIOState, input_analogs),  // bytes offset in struct
    NULL,  // default value
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__size_function__CIOState__input_analogs,  // size() function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_const_function__CIOState__input_analogs,  // get_const(index) function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_function__CIOState__input_analogs,  // get(index) function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__fetch_function__CIOState__input_analogs,  // fetch(index, &value) function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__assign_function__CIOState__input_analogs,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "output_analogs",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    2,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__msg__CIOState, output_analogs),  // bytes offset in struct
    NULL,  // default value
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__size_function__CIOState__output_analogs,  // size() function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_const_function__CIOState__output_analogs,  // get_const(index) function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_function__CIOState__output_analogs,  // get(index) function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__fetch_function__CIOState__output_analogs,  // fetch(index, &value) function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__assign_function__CIOState__output_analogs,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "input_conf",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    16,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__msg__CIOState, input_conf),  // bytes offset in struct
    NULL,  // default value
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__size_function__CIOState__input_conf,  // size() function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_const_function__CIOState__input_conf,  // get_const(index) function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_function__CIOState__input_conf,  // get(index) function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__fetch_function__CIOState__input_conf,  // fetch(index, &value) function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__assign_function__CIOState__input_conf,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "output_conf",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    16,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__msg__CIOState, output_conf),  // bytes offset in struct
    NULL,  // default value
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__size_function__CIOState__output_conf,  // size() function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_const_function__CIOState__output_conf,  // get_const(index) function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__get_function__CIOState__output_conf,  // get(index) function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__fetch_function__CIOState__output_conf,  // fetch(index, &value) function pointer
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__assign_function__CIOState__output_conf,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__CIOState_message_members = {
  "xarm_msgs__msg",  // message namespace
  "CIOState",  // message name
  9,  // number of fields
  sizeof(xarm_msgs__msg__CIOState),
  xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__CIOState_message_member_array,  // message members
  xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__CIOState_init_function,  // function to initialize message memory (memory has to be allocated)
  xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__CIOState_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__CIOState_message_type_support_handle = {
  0,
  &xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__CIOState_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_xarm_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, xarm_msgs, msg, CIOState)() {
  xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__CIOState_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  if (!xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__CIOState_message_type_support_handle.typesupport_identifier) {
    xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__CIOState_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &xarm_msgs__msg__CIOState__rosidl_typesupport_introspection_c__CIOState_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
