// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from xarm_msgs:msg/IOState.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "xarm_msgs/msg/detail/io_state__rosidl_typesupport_introspection_c.h"
#include "xarm_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "xarm_msgs/msg/detail/io_state__functions.h"
#include "xarm_msgs/msg/detail/io_state__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void xarm_msgs__msg__IOState__rosidl_typesupport_introspection_c__IOState_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  xarm_msgs__msg__IOState__init(message_memory);
}

void xarm_msgs__msg__IOState__rosidl_typesupport_introspection_c__IOState_fini_function(void * message_memory)
{
  xarm_msgs__msg__IOState__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember xarm_msgs__msg__IOState__rosidl_typesupport_introspection_c__IOState_message_member_array[4] = {
  {
    "digital_1",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__msg__IOState, digital_1),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "digital_2",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__msg__IOState, digital_2),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "analog_1",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__msg__IOState, analog_1),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "analog_2",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__msg__IOState, analog_2),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers xarm_msgs__msg__IOState__rosidl_typesupport_introspection_c__IOState_message_members = {
  "xarm_msgs__msg",  // message namespace
  "IOState",  // message name
  4,  // number of fields
  sizeof(xarm_msgs__msg__IOState),
  xarm_msgs__msg__IOState__rosidl_typesupport_introspection_c__IOState_message_member_array,  // message members
  xarm_msgs__msg__IOState__rosidl_typesupport_introspection_c__IOState_init_function,  // function to initialize message memory (memory has to be allocated)
  xarm_msgs__msg__IOState__rosidl_typesupport_introspection_c__IOState_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t xarm_msgs__msg__IOState__rosidl_typesupport_introspection_c__IOState_message_type_support_handle = {
  0,
  &xarm_msgs__msg__IOState__rosidl_typesupport_introspection_c__IOState_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_xarm_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, xarm_msgs, msg, IOState)() {
  if (!xarm_msgs__msg__IOState__rosidl_typesupport_introspection_c__IOState_message_type_support_handle.typesupport_identifier) {
    xarm_msgs__msg__IOState__rosidl_typesupport_introspection_c__IOState_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &xarm_msgs__msg__IOState__rosidl_typesupport_introspection_c__IOState_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
