// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from xarm_msgs:srv/IdenLoad.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__IDEN_LOAD__STRUCT_H_
#define XARM_MSGS__SRV__DETAIL__IDEN_LOAD__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/IdenLoad in the package xarm_msgs.
typedef struct xarm_msgs__srv__IdenLoad_Request
{
  /// estimated mass(kg), only required for Lite6 models via the `iden_tcp_load` service
  float estimated_mass;
} xarm_msgs__srv__IdenLoad_Request;

// Struct for a sequence of xarm_msgs__srv__IdenLoad_Request.
typedef struct xarm_msgs__srv__IdenLoad_Request__Sequence
{
  xarm_msgs__srv__IdenLoad_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__IdenLoad_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'message'
#include "rosidl_runtime_c/string.h"
// Member 'datas'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in srv/IdenLoad in the package xarm_msgs.
typedef struct xarm_msgs__srv__IdenLoad_Response
{
  int16_t ret;
  rosidl_runtime_c__String message;
  /// the result of identification
  ///   iden_tcp_load: [mass(kg)，x_centroid(mm)，y_centroid(mm)，z_centroid(mm)]
  ///   ft_sensor_iden_load: [mass(kg)，x_centroid(mm)，y_centroid(mm)，z_centroid(mm)，Fx_offset，Fy_offset，Fz_offset，Tx_offset，Ty_offset，Tz_ffset]
  rosidl_runtime_c__float__Sequence datas;
} xarm_msgs__srv__IdenLoad_Response;

// Struct for a sequence of xarm_msgs__srv__IdenLoad_Response.
typedef struct xarm_msgs__srv__IdenLoad_Response__Sequence
{
  xarm_msgs__srv__IdenLoad_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__IdenLoad_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // XARM_MSGS__SRV__DETAIL__IDEN_LOAD__STRUCT_H_
