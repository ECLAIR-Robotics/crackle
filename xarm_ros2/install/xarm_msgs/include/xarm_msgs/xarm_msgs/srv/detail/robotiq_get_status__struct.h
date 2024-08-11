// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from xarm_msgs:srv/RobotiqGetStatus.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__ROBOTIQ_GET_STATUS__STRUCT_H_
#define XARM_MSGS__SRV__DETAIL__ROBOTIQ_GET_STATUS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/RobotiqGetStatus in the package xarm_msgs.
typedef struct xarm_msgs__srv__RobotiqGetStatus_Request
{
  uint8_t number_of_registers;
} xarm_msgs__srv__RobotiqGetStatus_Request;

// Struct for a sequence of xarm_msgs__srv__RobotiqGetStatus_Request.
typedef struct xarm_msgs__srv__RobotiqGetStatus_Request__Sequence
{
  xarm_msgs__srv__RobotiqGetStatus_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__RobotiqGetStatus_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'message'
#include "rosidl_runtime_c/string.h"
// Member 'ret_data'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in srv/RobotiqGetStatus in the package xarm_msgs.
typedef struct xarm_msgs__srv__RobotiqGetStatus_Response
{
  int16_t ret;
  rosidl_runtime_c__String message;
  rosidl_runtime_c__uint8__Sequence ret_data;
} xarm_msgs__srv__RobotiqGetStatus_Response;

// Struct for a sequence of xarm_msgs__srv__RobotiqGetStatus_Response.
typedef struct xarm_msgs__srv__RobotiqGetStatus_Response__Sequence
{
  xarm_msgs__srv__RobotiqGetStatus_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__RobotiqGetStatus_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // XARM_MSGS__SRV__DETAIL__ROBOTIQ_GET_STATUS__STRUCT_H_
