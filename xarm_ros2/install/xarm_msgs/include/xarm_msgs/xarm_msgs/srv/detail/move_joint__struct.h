// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from xarm_msgs:srv/MoveJoint.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__MOVE_JOINT__STRUCT_H_
#define XARM_MSGS__SRV__DETAIL__MOVE_JOINT__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'angles'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in srv/MoveJoint in the package xarm_msgs.
typedef struct xarm_msgs__srv__MoveJoint_Request
{
  /// set_servo_angle/set_servo_angle_j
  rosidl_runtime_c__float__Sequence angles;
  float speed;
  float acc;
  float mvtime;
  /// set_servo_angle
  bool wait;
  float timeout;
  /// set_servo_angle
  float radius;
  /// set_servo_angle
  bool relative;
} xarm_msgs__srv__MoveJoint_Request;

// Struct for a sequence of xarm_msgs__srv__MoveJoint_Request.
typedef struct xarm_msgs__srv__MoveJoint_Request__Sequence
{
  xarm_msgs__srv__MoveJoint_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__MoveJoint_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'message'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/MoveJoint in the package xarm_msgs.
typedef struct xarm_msgs__srv__MoveJoint_Response
{
  int16_t ret;
  rosidl_runtime_c__String message;
} xarm_msgs__srv__MoveJoint_Response;

// Struct for a sequence of xarm_msgs__srv__MoveJoint_Response.
typedef struct xarm_msgs__srv__MoveJoint_Response__Sequence
{
  xarm_msgs__srv__MoveJoint_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__MoveJoint_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // XARM_MSGS__SRV__DETAIL__MOVE_JOINT__STRUCT_H_
