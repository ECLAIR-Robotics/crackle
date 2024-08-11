// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from xarm_msgs:srv/SetDigitalIO.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__SET_DIGITAL_IO__STRUCT_H_
#define XARM_MSGS__SRV__DETAIL__SET_DIGITAL_IO__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'xyz'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in srv/SetDigitalIO in the package xarm_msgs.
typedef struct xarm_msgs__srv__SetDigitalIO_Request
{
  /// set_tgpio_digital/set_cgpio_digital/set_tgpio_digital_with_xyz/set_cgpio_digital_with_xyz
  int16_t ionum;
  int16_t value;
  /// set_tgpio_digital/set_cgpio_digital
  float delay_sec;
  /// set_tgpio_digital_with_xyz/set_cgpio_digital_with_xyz
  rosidl_runtime_c__float__Sequence xyz;
  float tol_r;
} xarm_msgs__srv__SetDigitalIO_Request;

// Struct for a sequence of xarm_msgs__srv__SetDigitalIO_Request.
typedef struct xarm_msgs__srv__SetDigitalIO_Request__Sequence
{
  xarm_msgs__srv__SetDigitalIO_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__SetDigitalIO_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'message'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/SetDigitalIO in the package xarm_msgs.
typedef struct xarm_msgs__srv__SetDigitalIO_Response
{
  int16_t ret;
  rosidl_runtime_c__String message;
} xarm_msgs__srv__SetDigitalIO_Response;

// Struct for a sequence of xarm_msgs__srv__SetDigitalIO_Response.
typedef struct xarm_msgs__srv__SetDigitalIO_Response__Sequence
{
  xarm_msgs__srv__SetDigitalIO_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__SetDigitalIO_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // XARM_MSGS__SRV__DETAIL__SET_DIGITAL_IO__STRUCT_H_
