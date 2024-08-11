// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from xarm_msgs:msg/CIOState.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__MSG__DETAIL__CIO_STATE__STRUCT_H_
#define XARM_MSGS__MSG__DETAIL__CIO_STATE__STRUCT_H_

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

/// Struct defined in msg/CIOState in the package xarm_msgs.
typedef struct xarm_msgs__msg__CIOState
{
  std_msgs__msg__Header header;
  /// contorller gpio module state
  int16_t state;
  /// controller gpio module error code
  int16_t code;
  /// input_digitals[0]: digital input functional gpio state
  /// input_digitals[1]: digital input configuring gpio state
  ///    CI0: (input_digitals[1] >> 0) & 0x0001
  ///    CI1: (input_digitals[1] >> 1) & 0x0001
  ///    CI7: (input_digitals[1] >> 7) & 0x0001
  ///    DI0: (input_digitals[1] >> 8) & 0x0001
  ///    DI1: (input_digitals[1] >> 9) & 0x0001
  ///    DI7: (input_digitals[1] >> 15) & 0x0001
  uint16_t input_digitals[2];
  /// output_digitals[0]: digital output functional gpio state
  /// output_digitals[1]: digital output configuring gpio state
  ///    CO0: (output_digitals[1] >> 0) & 0x0001
  ///    CO1: (output_digitals[1] >> 1) & 0x0001
  ///    CO7: (output_digitals[1] >> 7) & 0x0001
  ///    DO0: (output_digitals[1] >> 8) & 0x0001
  ///    DO1: (output_digitals[1] >> 9) & 0x0001
  ///    DO7: (output_digitals[1] >> 15) & 0x0001
  uint16_t output_digitals[2];
  /// input_analogs[0]: the value of AI0
  /// input_analogs[1]: the value of AI1
  float input_analogs[2];
  /// output_analogs[0]: the value of AO0
  /// output_analogs[1]: the value of AO1
  float output_analogs[2];
  /// digital input functional info
  uint8_t input_conf[16];
  /// digital output functional info
  uint8_t output_conf[16];
} xarm_msgs__msg__CIOState;

// Struct for a sequence of xarm_msgs__msg__CIOState.
typedef struct xarm_msgs__msg__CIOState__Sequence
{
  xarm_msgs__msg__CIOState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__msg__CIOState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // XARM_MSGS__MSG__DETAIL__CIO_STATE__STRUCT_H_
