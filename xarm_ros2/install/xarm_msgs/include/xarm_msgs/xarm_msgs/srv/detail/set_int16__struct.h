// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from xarm_msgs:srv/SetInt16.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__SET_INT16__STRUCT_H_
#define XARM_MSGS__SRV__DETAIL__SET_INT16__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/SetInt16 in the package xarm_msgs.
typedef struct xarm_msgs__srv__SetInt16_Request
{
  int16_t data;
} xarm_msgs__srv__SetInt16_Request;

// Struct for a sequence of xarm_msgs__srv__SetInt16_Request.
typedef struct xarm_msgs__srv__SetInt16_Request__Sequence
{
  xarm_msgs__srv__SetInt16_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__SetInt16_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'message'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/SetInt16 in the package xarm_msgs.
typedef struct xarm_msgs__srv__SetInt16_Response
{
  int16_t ret;
  rosidl_runtime_c__String message;
} xarm_msgs__srv__SetInt16_Response;

// Struct for a sequence of xarm_msgs__srv__SetInt16_Response.
typedef struct xarm_msgs__srv__SetInt16_Response__Sequence
{
  xarm_msgs__srv__SetInt16_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__SetInt16_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // XARM_MSGS__SRV__DETAIL__SET_INT16__STRUCT_H_
