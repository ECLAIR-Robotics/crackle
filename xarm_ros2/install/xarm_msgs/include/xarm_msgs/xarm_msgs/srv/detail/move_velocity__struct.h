// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from xarm_msgs:srv/MoveVelocity.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__MOVE_VELOCITY__STRUCT_H_
#define XARM_MSGS__SRV__DETAIL__MOVE_VELOCITY__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'speeds'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in srv/MoveVelocity in the package xarm_msgs.
typedef struct xarm_msgs__srv__MoveVelocity_Request
{
  /// vc_set_joint_velocity/vc_set_cartesian_velocity
  rosidl_runtime_c__float__Sequence speeds;
  /// vc_set_joint_velocity
  bool is_sync;
  /// vc_set_cartesian_velocity
  bool is_tool_coord;
  /// the maximum duration of the spedd, over this time will automatically set the speed to 0
  ///   duration > 0: seconds, indicates the maximum number of seconds that this speed can be maintained
  ///   duration == 0: always effective, will not stop automativally
  ///   duration < 0: default value, only used to be compatible with the old protocol, equivalent to 0
  /// avaiable for firmware_version >= 1.8.0
  float duration;
} xarm_msgs__srv__MoveVelocity_Request;

// Struct for a sequence of xarm_msgs__srv__MoveVelocity_Request.
typedef struct xarm_msgs__srv__MoveVelocity_Request__Sequence
{
  xarm_msgs__srv__MoveVelocity_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__MoveVelocity_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'message'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/MoveVelocity in the package xarm_msgs.
typedef struct xarm_msgs__srv__MoveVelocity_Response
{
  int16_t ret;
  rosidl_runtime_c__String message;
} xarm_msgs__srv__MoveVelocity_Response;

// Struct for a sequence of xarm_msgs__srv__MoveVelocity_Response.
typedef struct xarm_msgs__srv__MoveVelocity_Response__Sequence
{
  xarm_msgs__srv__MoveVelocity_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__MoveVelocity_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // XARM_MSGS__SRV__DETAIL__MOVE_VELOCITY__STRUCT_H_
