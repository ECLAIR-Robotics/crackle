// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from xarm_msgs:msg/RobotMsg.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "xarm_msgs/msg/detail/robot_msg__rosidl_typesupport_introspection_c.h"
#include "xarm_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "xarm_msgs/msg/detail/robot_msg__functions.h"
#include "xarm_msgs/msg/detail/robot_msg__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `angle`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__RobotMsg_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  xarm_msgs__msg__RobotMsg__init(message_memory);
}

void xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__RobotMsg_fini_function(void * message_memory)
{
  xarm_msgs__msg__RobotMsg__fini(message_memory);
}

size_t xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__size_function__RobotMsg__angle(
  const void * untyped_member)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return member->size;
}

const void * xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__get_const_function__RobotMsg__angle(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void * xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__get_function__RobotMsg__angle(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__fetch_function__RobotMsg__angle(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__get_const_function__RobotMsg__angle(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__assign_function__RobotMsg__angle(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__get_function__RobotMsg__angle(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

bool xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__resize_function__RobotMsg__angle(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  rosidl_runtime_c__float__Sequence__fini(member);
  return rosidl_runtime_c__float__Sequence__init(member, size);
}

size_t xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__size_function__RobotMsg__pose(
  const void * untyped_member)
{
  (void)untyped_member;
  return 6;
}

const void * xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__get_const_function__RobotMsg__pose(
  const void * untyped_member, size_t index)
{
  const float * member =
    (const float *)(untyped_member);
  return &member[index];
}

void * xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__get_function__RobotMsg__pose(
  void * untyped_member, size_t index)
{
  float * member =
    (float *)(untyped_member);
  return &member[index];
}

void xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__fetch_function__RobotMsg__pose(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__get_const_function__RobotMsg__pose(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__assign_function__RobotMsg__pose(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__get_function__RobotMsg__pose(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

size_t xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__size_function__RobotMsg__offset(
  const void * untyped_member)
{
  (void)untyped_member;
  return 6;
}

const void * xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__get_const_function__RobotMsg__offset(
  const void * untyped_member, size_t index)
{
  const float * member =
    (const float *)(untyped_member);
  return &member[index];
}

void * xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__get_function__RobotMsg__offset(
  void * untyped_member, size_t index)
{
  float * member =
    (float *)(untyped_member);
  return &member[index];
}

void xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__fetch_function__RobotMsg__offset(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__get_const_function__RobotMsg__offset(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__assign_function__RobotMsg__offset(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__get_function__RobotMsg__offset(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

static rosidl_typesupport_introspection_c__MessageMember xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__RobotMsg_message_member_array[11] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__msg__RobotMsg, header),  // bytes offset in struct
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
    offsetof(xarm_msgs__msg__RobotMsg, state),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "mode",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__msg__RobotMsg, mode),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "cmdnum",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__msg__RobotMsg, cmdnum),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "mt_brake",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__msg__RobotMsg, mt_brake),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "mt_able",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__msg__RobotMsg, mt_able),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "err",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__msg__RobotMsg, err),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "warn",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__msg__RobotMsg, warn),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "angle",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__msg__RobotMsg, angle),  // bytes offset in struct
    NULL,  // default value
    xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__size_function__RobotMsg__angle,  // size() function pointer
    xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__get_const_function__RobotMsg__angle,  // get_const(index) function pointer
    xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__get_function__RobotMsg__angle,  // get(index) function pointer
    xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__fetch_function__RobotMsg__angle,  // fetch(index, &value) function pointer
    xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__assign_function__RobotMsg__angle,  // assign(index, value) function pointer
    xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__resize_function__RobotMsg__angle  // resize(index) function pointer
  },
  {
    "pose",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    6,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__msg__RobotMsg, pose),  // bytes offset in struct
    NULL,  // default value
    xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__size_function__RobotMsg__pose,  // size() function pointer
    xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__get_const_function__RobotMsg__pose,  // get_const(index) function pointer
    xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__get_function__RobotMsg__pose,  // get(index) function pointer
    xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__fetch_function__RobotMsg__pose,  // fetch(index, &value) function pointer
    xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__assign_function__RobotMsg__pose,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "offset",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    6,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__msg__RobotMsg, offset),  // bytes offset in struct
    NULL,  // default value
    xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__size_function__RobotMsg__offset,  // size() function pointer
    xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__get_const_function__RobotMsg__offset,  // get_const(index) function pointer
    xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__get_function__RobotMsg__offset,  // get(index) function pointer
    xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__fetch_function__RobotMsg__offset,  // fetch(index, &value) function pointer
    xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__assign_function__RobotMsg__offset,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__RobotMsg_message_members = {
  "xarm_msgs__msg",  // message namespace
  "RobotMsg",  // message name
  11,  // number of fields
  sizeof(xarm_msgs__msg__RobotMsg),
  xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__RobotMsg_message_member_array,  // message members
  xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__RobotMsg_init_function,  // function to initialize message memory (memory has to be allocated)
  xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__RobotMsg_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__RobotMsg_message_type_support_handle = {
  0,
  &xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__RobotMsg_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_xarm_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, xarm_msgs, msg, RobotMsg)() {
  xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__RobotMsg_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  if (!xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__RobotMsg_message_type_support_handle.typesupport_identifier) {
    xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__RobotMsg_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &xarm_msgs__msg__RobotMsg__rosidl_typesupport_introspection_c__RobotMsg_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
