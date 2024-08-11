// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from xarm_msgs:srv/FtImpedance.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__FT_IMPEDANCE__STRUCT_H_
#define XARM_MSGS__SRV__DETAIL__FT_IMPEDANCE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'c_axis'
// Member 'm'
// Member 'k'
// Member 'b'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in srv/FtImpedance in the package xarm_msgs.
typedef struct xarm_msgs__srv__FtImpedance_Request
{
  /// - set_impedance
  /// - set_impedance_config
  int16_t coord;
  rosidl_runtime_c__int16__Sequence c_axis;
  /// - set_impedance
  /// - set_impedance_mbk
  rosidl_runtime_c__float__Sequence m;
  rosidl_runtime_c__float__Sequence k;
  rosidl_runtime_c__float__Sequence b;
} xarm_msgs__srv__FtImpedance_Request;

// Struct for a sequence of xarm_msgs__srv__FtImpedance_Request.
typedef struct xarm_msgs__srv__FtImpedance_Request__Sequence
{
  xarm_msgs__srv__FtImpedance_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__FtImpedance_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'message'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/FtImpedance in the package xarm_msgs.
typedef struct xarm_msgs__srv__FtImpedance_Response
{
  int16_t ret;
  rosidl_runtime_c__String message;
} xarm_msgs__srv__FtImpedance_Response;

// Struct for a sequence of xarm_msgs__srv__FtImpedance_Response.
typedef struct xarm_msgs__srv__FtImpedance_Response__Sequence
{
  xarm_msgs__srv__FtImpedance_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__FtImpedance_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // XARM_MSGS__SRV__DETAIL__FT_IMPEDANCE__STRUCT_H_
