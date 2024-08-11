// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from xarm_msgs:msg/IOState.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__MSG__DETAIL__IO_STATE__STRUCT_H_
#define XARM_MSGS__MSG__DETAIL__IO_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/IOState in the package xarm_msgs.
/**
  * for indicating 2 digital and 2 analog Input port state
 */
typedef struct xarm_msgs__msg__IOState
{
  int32_t digital_1;
  int32_t digital_2;
  float analog_1;
  float analog_2;
} xarm_msgs__msg__IOState;

// Struct for a sequence of xarm_msgs__msg__IOState.
typedef struct xarm_msgs__msg__IOState__Sequence
{
  xarm_msgs__msg__IOState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__msg__IOState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // XARM_MSGS__MSG__DETAIL__IO_STATE__STRUCT_H_
