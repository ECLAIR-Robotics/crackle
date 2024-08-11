// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from xarm_msgs:srv/MoveCircle.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__MOVE_CIRCLE__STRUCT_H_
#define XARM_MSGS__SRV__DETAIL__MOVE_CIRCLE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'pose1'
// Member 'pose2'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in srv/MoveCircle in the package xarm_msgs.
typedef struct xarm_msgs__srv__MoveCircle_Request
{
  rosidl_runtime_c__float__Sequence pose1;
  rosidl_runtime_c__float__Sequence pose2;
  float percent;
  float speed;
  float acc;
  float mvtime;
  bool wait;
  float timeout;
  bool is_tool_coord;
  bool is_axis_angle;
} xarm_msgs__srv__MoveCircle_Request;

// Struct for a sequence of xarm_msgs__srv__MoveCircle_Request.
typedef struct xarm_msgs__srv__MoveCircle_Request__Sequence
{
  xarm_msgs__srv__MoveCircle_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__MoveCircle_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'message'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/MoveCircle in the package xarm_msgs.
typedef struct xarm_msgs__srv__MoveCircle_Response
{
  int16_t ret;
  rosidl_runtime_c__String message;
} xarm_msgs__srv__MoveCircle_Response;

// Struct for a sequence of xarm_msgs__srv__MoveCircle_Response.
typedef struct xarm_msgs__srv__MoveCircle_Response__Sequence
{
  xarm_msgs__srv__MoveCircle_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__MoveCircle_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // XARM_MSGS__SRV__DETAIL__MOVE_CIRCLE__STRUCT_H_
