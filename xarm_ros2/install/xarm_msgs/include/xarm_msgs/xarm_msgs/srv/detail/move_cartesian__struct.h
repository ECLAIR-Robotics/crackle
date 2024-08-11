// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from xarm_msgs:srv/MoveCartesian.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__MOVE_CARTESIAN__STRUCT_H_
#define XARM_MSGS__SRV__DETAIL__MOVE_CARTESIAN__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'pose'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in srv/MoveCartesian in the package xarm_msgs.
typedef struct xarm_msgs__srv__MoveCartesian_Request
{
  /// set_position/set_tool_position/set_position_aa/set_servo_cartesian/set_servo_cartesian_aa
  rosidl_runtime_c__float__Sequence pose;
  float speed;
  float acc;
  float mvtime;
  /// set_position/set_position_aa/set_tool_position
  bool wait;
  float timeout;
  /// set_position/set_tool_position/set_position_aa
  float radius;
  /// set_position_aa/set_servo_cartesian/set_servo_cartesian_aa
  bool is_tool_coord;
  /// set_position_aa/set_servo_cartesian_aa
  bool relative;
  /// set_position/set_tool_position/set_position_aa
  ///   motion_type == 0: default, linear planning
  ///   motion_type == 1: prioritize linear planning, and turn to IK for joint planning when linear planning is not possible
  ///   motion_type == 2: direct transfer to IK using joint planning
  ///   Note:
  ///       1. only available if firmware_version >= 1.11.100
  ///       2. when motion_type is 1 or 2, linear motion cannot be guaranteed
  ///       3. once IK is transferred to joint planning, the given Cartesian velocity and acceleration are converted into joint velocity and acceleration according to the percentage
  ///           speed = speed / max_tcp_speed * max_joint_speed
  ///           acc = acc / max_tcp_acc * max_joint_acc
  ///       4. if there is no suitable IK, a C40 error will be triggered
  uint8_t motion_type;
} xarm_msgs__srv__MoveCartesian_Request;

// Struct for a sequence of xarm_msgs__srv__MoveCartesian_Request.
typedef struct xarm_msgs__srv__MoveCartesian_Request__Sequence
{
  xarm_msgs__srv__MoveCartesian_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__MoveCartesian_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'message'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/MoveCartesian in the package xarm_msgs.
typedef struct xarm_msgs__srv__MoveCartesian_Response
{
  int16_t ret;
  rosidl_runtime_c__String message;
} xarm_msgs__srv__MoveCartesian_Response;

// Struct for a sequence of xarm_msgs__srv__MoveCartesian_Response.
typedef struct xarm_msgs__srv__MoveCartesian_Response__Sequence
{
  xarm_msgs__srv__MoveCartesian_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__MoveCartesian_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // XARM_MSGS__SRV__DETAIL__MOVE_CARTESIAN__STRUCT_H_
