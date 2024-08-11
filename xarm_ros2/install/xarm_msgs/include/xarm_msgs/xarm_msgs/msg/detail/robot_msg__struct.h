// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from xarm_msgs:msg/RobotMsg.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__MSG__DETAIL__ROBOT_MSG__STRUCT_H_
#define XARM_MSGS__MSG__DETAIL__ROBOT_MSG__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'angle'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/RobotMsg in the package xarm_msgs.
typedef struct xarm_msgs__msg__RobotMsg
{
  std_msgs__msg__Header header;
  /// feedback information of the controlled robot
  /// state of robot:
  /// 1: RUNNING, executing motion command.
  /// 2: SLEEPING, not in execution, but ready to move.
  /// 3: PAUSED, paused in the middle of unfinished motion.
  /// 4: STOPPED, not ready for any motion commands.
  /// 5: CONFIG_CHANGED, system configuration or mode changed, not ready for motion commands.
  int16_t state;
  /// mode of robot:
  /// 0 for POSITION mode.(position control by xarm controller box, execute api standard commands)
  /// 1 for SERVOJ mode. (Immediate execution towards received joint space target, like a step response)
  /// 2 for TEACHING_JOINT mode. (Gravity compensated mode, easy for teaching)
  int16_t mode;
  /// cmdnum: number of commands waiting in the buffer.
  int16_t cmdnum;
  /// mt_brake: if translated to binary digits, each bit represent one axis, 1 for brake enabled, 0 for brake disabled
  int16_t mt_brake;
  /// mt_able: if translated to binary digits, each bit represent one axis, 1 for servo control enabled, 0 for servo disabled
  int16_t mt_able;
  /// error code (if non-zero)
  int16_t err;
  /// warning code (if non-zero)
  int16_t warn;
  /// current joint angles expressed in radian.
  rosidl_runtime_c__float__Sequence angle;
  /// current TCP Cartesian position expressed in mm(position), radian(orientation)
  float pose[6];
  /// TCP offset from center of flange, with respect to tool frame.
  float offset[6];
} xarm_msgs__msg__RobotMsg;

// Struct for a sequence of xarm_msgs__msg__RobotMsg.
typedef struct xarm_msgs__msg__RobotMsg__Sequence
{
  xarm_msgs__msg__RobotMsg * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__msg__RobotMsg__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // XARM_MSGS__MSG__DETAIL__ROBOT_MSG__STRUCT_H_
