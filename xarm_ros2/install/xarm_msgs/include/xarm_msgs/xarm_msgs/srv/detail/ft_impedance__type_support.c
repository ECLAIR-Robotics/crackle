// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from xarm_msgs:srv/FtImpedance.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "xarm_msgs/srv/detail/ft_impedance__rosidl_typesupport_introspection_c.h"
#include "xarm_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "xarm_msgs/srv/detail/ft_impedance__functions.h"
#include "xarm_msgs/srv/detail/ft_impedance__struct.h"


// Include directives for member types
// Member `c_axis`
// Member `m`
// Member `k`
// Member `b`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__FtImpedance_Request_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  xarm_msgs__srv__FtImpedance_Request__init(message_memory);
}

void xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__FtImpedance_Request_fini_function(void * message_memory)
{
  xarm_msgs__srv__FtImpedance_Request__fini(message_memory);
}

size_t xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__size_function__FtImpedance_Request__c_axis(
  const void * untyped_member)
{
  const rosidl_runtime_c__int16__Sequence * member =
    (const rosidl_runtime_c__int16__Sequence *)(untyped_member);
  return member->size;
}

const void * xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_const_function__FtImpedance_Request__c_axis(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__int16__Sequence * member =
    (const rosidl_runtime_c__int16__Sequence *)(untyped_member);
  return &member->data[index];
}

void * xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_function__FtImpedance_Request__c_axis(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__int16__Sequence * member =
    (rosidl_runtime_c__int16__Sequence *)(untyped_member);
  return &member->data[index];
}

void xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__fetch_function__FtImpedance_Request__c_axis(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const int16_t * item =
    ((const int16_t *)
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_const_function__FtImpedance_Request__c_axis(untyped_member, index));
  int16_t * value =
    (int16_t *)(untyped_value);
  *value = *item;
}

void xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__assign_function__FtImpedance_Request__c_axis(
  void * untyped_member, size_t index, const void * untyped_value)
{
  int16_t * item =
    ((int16_t *)
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_function__FtImpedance_Request__c_axis(untyped_member, index));
  const int16_t * value =
    (const int16_t *)(untyped_value);
  *item = *value;
}

bool xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__resize_function__FtImpedance_Request__c_axis(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__int16__Sequence * member =
    (rosidl_runtime_c__int16__Sequence *)(untyped_member);
  rosidl_runtime_c__int16__Sequence__fini(member);
  return rosidl_runtime_c__int16__Sequence__init(member, size);
}

size_t xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__size_function__FtImpedance_Request__m(
  const void * untyped_member)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return member->size;
}

const void * xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_const_function__FtImpedance_Request__m(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void * xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_function__FtImpedance_Request__m(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__fetch_function__FtImpedance_Request__m(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_const_function__FtImpedance_Request__m(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__assign_function__FtImpedance_Request__m(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_function__FtImpedance_Request__m(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

bool xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__resize_function__FtImpedance_Request__m(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  rosidl_runtime_c__float__Sequence__fini(member);
  return rosidl_runtime_c__float__Sequence__init(member, size);
}

size_t xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__size_function__FtImpedance_Request__k(
  const void * untyped_member)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return member->size;
}

const void * xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_const_function__FtImpedance_Request__k(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void * xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_function__FtImpedance_Request__k(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__fetch_function__FtImpedance_Request__k(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_const_function__FtImpedance_Request__k(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__assign_function__FtImpedance_Request__k(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_function__FtImpedance_Request__k(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

bool xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__resize_function__FtImpedance_Request__k(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  rosidl_runtime_c__float__Sequence__fini(member);
  return rosidl_runtime_c__float__Sequence__init(member, size);
}

size_t xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__size_function__FtImpedance_Request__b(
  const void * untyped_member)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return member->size;
}

const void * xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_const_function__FtImpedance_Request__b(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void * xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_function__FtImpedance_Request__b(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__fetch_function__FtImpedance_Request__b(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_const_function__FtImpedance_Request__b(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__assign_function__FtImpedance_Request__b(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_function__FtImpedance_Request__b(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

bool xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__resize_function__FtImpedance_Request__b(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  rosidl_runtime_c__float__Sequence__fini(member);
  return rosidl_runtime_c__float__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__FtImpedance_Request_message_member_array[5] = {
  {
    "coord",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__srv__FtImpedance_Request, coord),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "c_axis",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__srv__FtImpedance_Request, c_axis),  // bytes offset in struct
    NULL,  // default value
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__size_function__FtImpedance_Request__c_axis,  // size() function pointer
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_const_function__FtImpedance_Request__c_axis,  // get_const(index) function pointer
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_function__FtImpedance_Request__c_axis,  // get(index) function pointer
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__fetch_function__FtImpedance_Request__c_axis,  // fetch(index, &value) function pointer
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__assign_function__FtImpedance_Request__c_axis,  // assign(index, value) function pointer
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__resize_function__FtImpedance_Request__c_axis  // resize(index) function pointer
  },
  {
    "m",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__srv__FtImpedance_Request, m),  // bytes offset in struct
    NULL,  // default value
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__size_function__FtImpedance_Request__m,  // size() function pointer
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_const_function__FtImpedance_Request__m,  // get_const(index) function pointer
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_function__FtImpedance_Request__m,  // get(index) function pointer
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__fetch_function__FtImpedance_Request__m,  // fetch(index, &value) function pointer
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__assign_function__FtImpedance_Request__m,  // assign(index, value) function pointer
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__resize_function__FtImpedance_Request__m  // resize(index) function pointer
  },
  {
    "k",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__srv__FtImpedance_Request, k),  // bytes offset in struct
    NULL,  // default value
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__size_function__FtImpedance_Request__k,  // size() function pointer
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_const_function__FtImpedance_Request__k,  // get_const(index) function pointer
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_function__FtImpedance_Request__k,  // get(index) function pointer
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__fetch_function__FtImpedance_Request__k,  // fetch(index, &value) function pointer
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__assign_function__FtImpedance_Request__k,  // assign(index, value) function pointer
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__resize_function__FtImpedance_Request__k  // resize(index) function pointer
  },
  {
    "b",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__srv__FtImpedance_Request, b),  // bytes offset in struct
    NULL,  // default value
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__size_function__FtImpedance_Request__b,  // size() function pointer
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_const_function__FtImpedance_Request__b,  // get_const(index) function pointer
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__get_function__FtImpedance_Request__b,  // get(index) function pointer
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__fetch_function__FtImpedance_Request__b,  // fetch(index, &value) function pointer
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__assign_function__FtImpedance_Request__b,  // assign(index, value) function pointer
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__resize_function__FtImpedance_Request__b  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__FtImpedance_Request_message_members = {
  "xarm_msgs__srv",  // message namespace
  "FtImpedance_Request",  // message name
  5,  // number of fields
  sizeof(xarm_msgs__srv__FtImpedance_Request),
  xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__FtImpedance_Request_message_member_array,  // message members
  xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__FtImpedance_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__FtImpedance_Request_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__FtImpedance_Request_message_type_support_handle = {
  0,
  &xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__FtImpedance_Request_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_xarm_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, xarm_msgs, srv, FtImpedance_Request)() {
  if (!xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__FtImpedance_Request_message_type_support_handle.typesupport_identifier) {
    xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__FtImpedance_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &xarm_msgs__srv__FtImpedance_Request__rosidl_typesupport_introspection_c__FtImpedance_Request_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "xarm_msgs/srv/detail/ft_impedance__rosidl_typesupport_introspection_c.h"
// already included above
// #include "xarm_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "xarm_msgs/srv/detail/ft_impedance__functions.h"
// already included above
// #include "xarm_msgs/srv/detail/ft_impedance__struct.h"


// Include directives for member types
// Member `message`
#include "rosidl_runtime_c/string_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void xarm_msgs__srv__FtImpedance_Response__rosidl_typesupport_introspection_c__FtImpedance_Response_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  xarm_msgs__srv__FtImpedance_Response__init(message_memory);
}

void xarm_msgs__srv__FtImpedance_Response__rosidl_typesupport_introspection_c__FtImpedance_Response_fini_function(void * message_memory)
{
  xarm_msgs__srv__FtImpedance_Response__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember xarm_msgs__srv__FtImpedance_Response__rosidl_typesupport_introspection_c__FtImpedance_Response_message_member_array[2] = {
  {
    "ret",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__srv__FtImpedance_Response, ret),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "message",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__srv__FtImpedance_Response, message),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers xarm_msgs__srv__FtImpedance_Response__rosidl_typesupport_introspection_c__FtImpedance_Response_message_members = {
  "xarm_msgs__srv",  // message namespace
  "FtImpedance_Response",  // message name
  2,  // number of fields
  sizeof(xarm_msgs__srv__FtImpedance_Response),
  xarm_msgs__srv__FtImpedance_Response__rosidl_typesupport_introspection_c__FtImpedance_Response_message_member_array,  // message members
  xarm_msgs__srv__FtImpedance_Response__rosidl_typesupport_introspection_c__FtImpedance_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  xarm_msgs__srv__FtImpedance_Response__rosidl_typesupport_introspection_c__FtImpedance_Response_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t xarm_msgs__srv__FtImpedance_Response__rosidl_typesupport_introspection_c__FtImpedance_Response_message_type_support_handle = {
  0,
  &xarm_msgs__srv__FtImpedance_Response__rosidl_typesupport_introspection_c__FtImpedance_Response_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_xarm_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, xarm_msgs, srv, FtImpedance_Response)() {
  if (!xarm_msgs__srv__FtImpedance_Response__rosidl_typesupport_introspection_c__FtImpedance_Response_message_type_support_handle.typesupport_identifier) {
    xarm_msgs__srv__FtImpedance_Response__rosidl_typesupport_introspection_c__FtImpedance_Response_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &xarm_msgs__srv__FtImpedance_Response__rosidl_typesupport_introspection_c__FtImpedance_Response_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "xarm_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "xarm_msgs/srv/detail/ft_impedance__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/service_introspection.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers xarm_msgs__srv__detail__ft_impedance__rosidl_typesupport_introspection_c__FtImpedance_service_members = {
  "xarm_msgs__srv",  // service namespace
  "FtImpedance",  // service name
  // these two fields are initialized below on the first access
  NULL,  // request message
  // xarm_msgs__srv__detail__ft_impedance__rosidl_typesupport_introspection_c__FtImpedance_Request_message_type_support_handle,
  NULL  // response message
  // xarm_msgs__srv__detail__ft_impedance__rosidl_typesupport_introspection_c__FtImpedance_Response_message_type_support_handle
};

static rosidl_service_type_support_t xarm_msgs__srv__detail__ft_impedance__rosidl_typesupport_introspection_c__FtImpedance_service_type_support_handle = {
  0,
  &xarm_msgs__srv__detail__ft_impedance__rosidl_typesupport_introspection_c__FtImpedance_service_members,
  get_service_typesupport_handle_function,
};

// Forward declaration of request/response type support functions
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, xarm_msgs, srv, FtImpedance_Request)();

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, xarm_msgs, srv, FtImpedance_Response)();

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_xarm_msgs
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, xarm_msgs, srv, FtImpedance)() {
  if (!xarm_msgs__srv__detail__ft_impedance__rosidl_typesupport_introspection_c__FtImpedance_service_type_support_handle.typesupport_identifier) {
    xarm_msgs__srv__detail__ft_impedance__rosidl_typesupport_introspection_c__FtImpedance_service_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  rosidl_typesupport_introspection_c__ServiceMembers * service_members =
    (rosidl_typesupport_introspection_c__ServiceMembers *)xarm_msgs__srv__detail__ft_impedance__rosidl_typesupport_introspection_c__FtImpedance_service_type_support_handle.data;

  if (!service_members->request_members_) {
    service_members->request_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, xarm_msgs, srv, FtImpedance_Request)()->data;
  }
  if (!service_members->response_members_) {
    service_members->response_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, xarm_msgs, srv, FtImpedance_Response)()->data;
  }

  return &xarm_msgs__srv__detail__ft_impedance__rosidl_typesupport_introspection_c__FtImpedance_service_type_support_handle;
}
