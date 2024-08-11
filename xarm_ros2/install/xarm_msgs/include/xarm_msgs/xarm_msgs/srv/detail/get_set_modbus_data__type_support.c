// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from xarm_msgs:srv/GetSetModbusData.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "xarm_msgs/srv/detail/get_set_modbus_data__rosidl_typesupport_introspection_c.h"
#include "xarm_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "xarm_msgs/srv/detail/get_set_modbus_data__functions.h"
#include "xarm_msgs/srv/detail/get_set_modbus_data__struct.h"


// Include directives for member types
// Member `modbus_data`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__GetSetModbusData_Request_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  xarm_msgs__srv__GetSetModbusData_Request__init(message_memory);
}

void xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__GetSetModbusData_Request_fini_function(void * message_memory)
{
  xarm_msgs__srv__GetSetModbusData_Request__fini(message_memory);
}

size_t xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__size_function__GetSetModbusData_Request__modbus_data(
  const void * untyped_member)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return member->size;
}

const void * xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__get_const_function__GetSetModbusData_Request__modbus_data(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void * xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__get_function__GetSetModbusData_Request__modbus_data(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__fetch_function__GetSetModbusData_Request__modbus_data(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint8_t * item =
    ((const uint8_t *)
    xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__get_const_function__GetSetModbusData_Request__modbus_data(untyped_member, index));
  uint8_t * value =
    (uint8_t *)(untyped_value);
  *value = *item;
}

void xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__assign_function__GetSetModbusData_Request__modbus_data(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint8_t * item =
    ((uint8_t *)
    xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__get_function__GetSetModbusData_Request__modbus_data(untyped_member, index));
  const uint8_t * value =
    (const uint8_t *)(untyped_value);
  *item = *value;
}

bool xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__resize_function__GetSetModbusData_Request__modbus_data(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  rosidl_runtime_c__uint8__Sequence__fini(member);
  return rosidl_runtime_c__uint8__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__GetSetModbusData_Request_message_member_array[6] = {
  {
    "modbus_data",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__srv__GetSetModbusData_Request, modbus_data),  // bytes offset in struct
    NULL,  // default value
    xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__size_function__GetSetModbusData_Request__modbus_data,  // size() function pointer
    xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__get_const_function__GetSetModbusData_Request__modbus_data,  // get_const(index) function pointer
    xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__get_function__GetSetModbusData_Request__modbus_data,  // get(index) function pointer
    xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__fetch_function__GetSetModbusData_Request__modbus_data,  // fetch(index, &value) function pointer
    xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__assign_function__GetSetModbusData_Request__modbus_data,  // assign(index, value) function pointer
    xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__resize_function__GetSetModbusData_Request__modbus_data  // resize(index) function pointer
  },
  {
    "modbus_length",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__srv__GetSetModbusData_Request, modbus_length),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "ret_length",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__srv__GetSetModbusData_Request, ret_length),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "host_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__srv__GetSetModbusData_Request, host_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "is_transparent_transmission",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__srv__GetSetModbusData_Request, is_transparent_transmission),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "use_503_port",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__srv__GetSetModbusData_Request, use_503_port),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__GetSetModbusData_Request_message_members = {
  "xarm_msgs__srv",  // message namespace
  "GetSetModbusData_Request",  // message name
  6,  // number of fields
  sizeof(xarm_msgs__srv__GetSetModbusData_Request),
  xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__GetSetModbusData_Request_message_member_array,  // message members
  xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__GetSetModbusData_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__GetSetModbusData_Request_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__GetSetModbusData_Request_message_type_support_handle = {
  0,
  &xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__GetSetModbusData_Request_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_xarm_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, xarm_msgs, srv, GetSetModbusData_Request)() {
  if (!xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__GetSetModbusData_Request_message_type_support_handle.typesupport_identifier) {
    xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__GetSetModbusData_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &xarm_msgs__srv__GetSetModbusData_Request__rosidl_typesupport_introspection_c__GetSetModbusData_Request_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "xarm_msgs/srv/detail/get_set_modbus_data__rosidl_typesupport_introspection_c.h"
// already included above
// #include "xarm_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "xarm_msgs/srv/detail/get_set_modbus_data__functions.h"
// already included above
// #include "xarm_msgs/srv/detail/get_set_modbus_data__struct.h"


// Include directives for member types
// Member `message`
#include "rosidl_runtime_c/string_functions.h"
// Member `ret_data`
// already included above
// #include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__GetSetModbusData_Response_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  xarm_msgs__srv__GetSetModbusData_Response__init(message_memory);
}

void xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__GetSetModbusData_Response_fini_function(void * message_memory)
{
  xarm_msgs__srv__GetSetModbusData_Response__fini(message_memory);
}

size_t xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__size_function__GetSetModbusData_Response__ret_data(
  const void * untyped_member)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return member->size;
}

const void * xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__get_const_function__GetSetModbusData_Response__ret_data(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void * xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__get_function__GetSetModbusData_Response__ret_data(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__fetch_function__GetSetModbusData_Response__ret_data(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint8_t * item =
    ((const uint8_t *)
    xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__get_const_function__GetSetModbusData_Response__ret_data(untyped_member, index));
  uint8_t * value =
    (uint8_t *)(untyped_value);
  *value = *item;
}

void xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__assign_function__GetSetModbusData_Response__ret_data(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint8_t * item =
    ((uint8_t *)
    xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__get_function__GetSetModbusData_Response__ret_data(untyped_member, index));
  const uint8_t * value =
    (const uint8_t *)(untyped_value);
  *item = *value;
}

bool xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__resize_function__GetSetModbusData_Response__ret_data(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  rosidl_runtime_c__uint8__Sequence__fini(member);
  return rosidl_runtime_c__uint8__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__GetSetModbusData_Response_message_member_array[3] = {
  {
    "ret",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__srv__GetSetModbusData_Response, ret),  // bytes offset in struct
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
    offsetof(xarm_msgs__srv__GetSetModbusData_Response, message),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "ret_data",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(xarm_msgs__srv__GetSetModbusData_Response, ret_data),  // bytes offset in struct
    NULL,  // default value
    xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__size_function__GetSetModbusData_Response__ret_data,  // size() function pointer
    xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__get_const_function__GetSetModbusData_Response__ret_data,  // get_const(index) function pointer
    xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__get_function__GetSetModbusData_Response__ret_data,  // get(index) function pointer
    xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__fetch_function__GetSetModbusData_Response__ret_data,  // fetch(index, &value) function pointer
    xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__assign_function__GetSetModbusData_Response__ret_data,  // assign(index, value) function pointer
    xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__resize_function__GetSetModbusData_Response__ret_data  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__GetSetModbusData_Response_message_members = {
  "xarm_msgs__srv",  // message namespace
  "GetSetModbusData_Response",  // message name
  3,  // number of fields
  sizeof(xarm_msgs__srv__GetSetModbusData_Response),
  xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__GetSetModbusData_Response_message_member_array,  // message members
  xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__GetSetModbusData_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__GetSetModbusData_Response_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__GetSetModbusData_Response_message_type_support_handle = {
  0,
  &xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__GetSetModbusData_Response_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_xarm_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, xarm_msgs, srv, GetSetModbusData_Response)() {
  if (!xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__GetSetModbusData_Response_message_type_support_handle.typesupport_identifier) {
    xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__GetSetModbusData_Response_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &xarm_msgs__srv__GetSetModbusData_Response__rosidl_typesupport_introspection_c__GetSetModbusData_Response_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "xarm_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "xarm_msgs/srv/detail/get_set_modbus_data__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/service_introspection.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers xarm_msgs__srv__detail__get_set_modbus_data__rosidl_typesupport_introspection_c__GetSetModbusData_service_members = {
  "xarm_msgs__srv",  // service namespace
  "GetSetModbusData",  // service name
  // these two fields are initialized below on the first access
  NULL,  // request message
  // xarm_msgs__srv__detail__get_set_modbus_data__rosidl_typesupport_introspection_c__GetSetModbusData_Request_message_type_support_handle,
  NULL  // response message
  // xarm_msgs__srv__detail__get_set_modbus_data__rosidl_typesupport_introspection_c__GetSetModbusData_Response_message_type_support_handle
};

static rosidl_service_type_support_t xarm_msgs__srv__detail__get_set_modbus_data__rosidl_typesupport_introspection_c__GetSetModbusData_service_type_support_handle = {
  0,
  &xarm_msgs__srv__detail__get_set_modbus_data__rosidl_typesupport_introspection_c__GetSetModbusData_service_members,
  get_service_typesupport_handle_function,
};

// Forward declaration of request/response type support functions
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, xarm_msgs, srv, GetSetModbusData_Request)();

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, xarm_msgs, srv, GetSetModbusData_Response)();

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_xarm_msgs
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, xarm_msgs, srv, GetSetModbusData)() {
  if (!xarm_msgs__srv__detail__get_set_modbus_data__rosidl_typesupport_introspection_c__GetSetModbusData_service_type_support_handle.typesupport_identifier) {
    xarm_msgs__srv__detail__get_set_modbus_data__rosidl_typesupport_introspection_c__GetSetModbusData_service_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  rosidl_typesupport_introspection_c__ServiceMembers * service_members =
    (rosidl_typesupport_introspection_c__ServiceMembers *)xarm_msgs__srv__detail__get_set_modbus_data__rosidl_typesupport_introspection_c__GetSetModbusData_service_type_support_handle.data;

  if (!service_members->request_members_) {
    service_members->request_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, xarm_msgs, srv, GetSetModbusData_Request)()->data;
  }
  if (!service_members->response_members_) {
    service_members->response_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, xarm_msgs, srv, GetSetModbusData_Response)()->data;
  }

  return &xarm_msgs__srv__detail__get_set_modbus_data__rosidl_typesupport_introspection_c__GetSetModbusData_service_type_support_handle;
}
