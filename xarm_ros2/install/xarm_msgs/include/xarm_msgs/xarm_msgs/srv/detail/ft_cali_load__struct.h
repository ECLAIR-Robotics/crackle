// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from xarm_msgs:srv/FtCaliLoad.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__FT_CALI_LOAD__STRUCT_H_
#define XARM_MSGS__SRV__DETAIL__FT_CALI_LOAD__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'datas'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in srv/FtCaliLoad in the package xarm_msgs.
typedef struct xarm_msgs__srv__FtCaliLoad_Request
{
  /// iden result()
  rosidl_runtime_c__float__Sequence datas;
  /// whether to convert the paramster to the corresponding tcp load and set, default is false
  bool association_setting_tcp_load;
  float m;
  float x;
  float y;
  float z;
} xarm_msgs__srv__FtCaliLoad_Request;

// Struct for a sequence of xarm_msgs__srv__FtCaliLoad_Request.
typedef struct xarm_msgs__srv__FtCaliLoad_Request__Sequence
{
  xarm_msgs__srv__FtCaliLoad_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__FtCaliLoad_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'message'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/FtCaliLoad in the package xarm_msgs.
typedef struct xarm_msgs__srv__FtCaliLoad_Response
{
  int16_t ret;
  rosidl_runtime_c__String message;
} xarm_msgs__srv__FtCaliLoad_Response;

// Struct for a sequence of xarm_msgs__srv__FtCaliLoad_Response.
typedef struct xarm_msgs__srv__FtCaliLoad_Response__Sequence
{
  xarm_msgs__srv__FtCaliLoad_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__FtCaliLoad_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // XARM_MSGS__SRV__DETAIL__FT_CALI_LOAD__STRUCT_H_
