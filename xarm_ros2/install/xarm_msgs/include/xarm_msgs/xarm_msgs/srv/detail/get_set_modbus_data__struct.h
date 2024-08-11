// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from xarm_msgs:srv/GetSetModbusData.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__SRV__DETAIL__GET_SET_MODBUS_DATA__STRUCT_H_
#define XARM_MSGS__SRV__DETAIL__GET_SET_MODBUS_DATA__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'modbus_data'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in srv/GetSetModbusData in the package xarm_msgs.
typedef struct xarm_msgs__srv__GetSetModbusData_Request
{
  /// modbus data to send
  rosidl_runtime_c__uint8__Sequence modbus_data;
  /// Specify the length of modbus data bytes to be sent, the default(less than or equal to 0) is the size of modbus_data
  int16_t modbus_length;
  /// Specify the anticipated maximum respond data length in bytes
  int16_t ret_length;
  /// host id, 9: END RS485, 10: Controller RS485
  uint8_t host_id;
  /// whether to choose transparent transmission
  bool is_transparent_transmission;
  /// whether to use port 503 for communication
  /// if it is true, it will connect to 503 port for communication when it is used for the first time, which is generally only useful for transparent transmission
  bool use_503_port;
} xarm_msgs__srv__GetSetModbusData_Request;

// Struct for a sequence of xarm_msgs__srv__GetSetModbusData_Request.
typedef struct xarm_msgs__srv__GetSetModbusData_Request__Sequence
{
  xarm_msgs__srv__GetSetModbusData_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__GetSetModbusData_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'message'
#include "rosidl_runtime_c/string.h"
// Member 'ret_data'
// already included above
// #include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in srv/GetSetModbusData in the package xarm_msgs.
typedef struct xarm_msgs__srv__GetSetModbusData_Response
{
  int16_t ret;
  rosidl_runtime_c__String message;
  rosidl_runtime_c__uint8__Sequence ret_data;
} xarm_msgs__srv__GetSetModbusData_Response;

// Struct for a sequence of xarm_msgs__srv__GetSetModbusData_Response.
typedef struct xarm_msgs__srv__GetSetModbusData_Response__Sequence
{
  xarm_msgs__srv__GetSetModbusData_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xarm_msgs__srv__GetSetModbusData_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // XARM_MSGS__SRV__DETAIL__GET_SET_MODBUS_DATA__STRUCT_H_
