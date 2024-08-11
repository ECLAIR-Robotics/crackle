// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from xarm_msgs:srv/PlanSingleStraight.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__PLAN_SINGLE_STRAIGHT__STRUCT_H_
#define XARM_MSGS__SRV__DETAIL__PLAN_SINGLE_STRAIGHT__STRUCT_H_

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
#include "geometry_msgs/msg/detail/pose__struct.h"

/// Struct defined in srv/PlanSingleStraight in the package xarm_msgs.
typedef struct xarm_msgs__srv__PlanSingleStraight_Request
{
  geometry_msgs__msg__Pose target;
} xarm_msgs__srv__PlanSingleStraight_Request;

// Struct for a sequence of xarm_msgs__srv__PlanSingleStraight_Request.
typedef struct xarm_msgs__srv__PlanSingleStraight_Request__Sequence
{
  xarm_msgs__srv__PlanSingleStraight_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__PlanSingleStraight_Request__Sequence;


// Constants defined in the message

/// Struct defined in srv/PlanSingleStraight in the package xarm_msgs.
typedef struct xarm_msgs__srv__PlanSingleStraight_Response
{
  bool success;
} xarm_msgs__srv__PlanSingleStraight_Response;

// Struct for a sequence of xarm_msgs__srv__PlanSingleStraight_Response.
typedef struct xarm_msgs__srv__PlanSingleStraight_Response__Sequence
{
  xarm_msgs__srv__PlanSingleStraight_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__PlanSingleStraight_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // XARM_MSGS__SRV__DETAIL__PLAN_SINGLE_STRAIGHT__STRUCT_H_
