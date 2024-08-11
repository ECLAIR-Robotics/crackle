// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from xarm_msgs:srv/PlanJoint.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__PLAN_JOINT__STRUCT_H_
#define XARM_MSGS__SRV__DETAIL__PLAN_JOINT__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'target'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in srv/PlanJoint in the package xarm_msgs.
typedef struct xarm_msgs__srv__PlanJoint_Request
{
  /// list of target joint positions in radian.
  rosidl_runtime_c__double__Sequence target;
} xarm_msgs__srv__PlanJoint_Request;

// Struct for a sequence of xarm_msgs__srv__PlanJoint_Request.
typedef struct xarm_msgs__srv__PlanJoint_Request__Sequence
{
  xarm_msgs__srv__PlanJoint_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__PlanJoint_Request__Sequence;


// Constants defined in the message

/// Struct defined in srv/PlanJoint in the package xarm_msgs.
typedef struct xarm_msgs__srv__PlanJoint_Response
{
  bool success;
} xarm_msgs__srv__PlanJoint_Response;

// Struct for a sequence of xarm_msgs__srv__PlanJoint_Response.
typedef struct xarm_msgs__srv__PlanJoint_Response__Sequence
{
  xarm_msgs__srv__PlanJoint_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__PlanJoint_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // XARM_MSGS__SRV__DETAIL__PLAN_JOINT__STRUCT_H_
