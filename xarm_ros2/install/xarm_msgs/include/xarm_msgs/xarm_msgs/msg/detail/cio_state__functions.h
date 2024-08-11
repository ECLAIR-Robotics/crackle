// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from xarm_msgs:msg/CIOState.idl
// generated code does not contain a copyright notice

#ifndef XARM_MSGS__MSG__DETAIL__CIO_STATE__FUNCTIONS_H_
#define XARM_MSGS__MSG__DETAIL__CIO_STATE__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "xarm_msgs/msg/rosidl_generator_c__visibility_control.h"

#include "xarm_msgs/msg/detail/cio_state__struct.h"

/// Initialize msg/CIOState message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * xarm_msgs__msg__CIOState
 * )) before or use
 * xarm_msgs__msg__CIOState__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_xarm_msgs
bool
xarm_msgs__msg__CIOState__init(xarm_msgs__msg__CIOState * msg);

/// Finalize msg/CIOState message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_xarm_msgs
void
xarm_msgs__msg__CIOState__fini(xarm_msgs__msg__CIOState * msg);

/// Create msg/CIOState message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * xarm_msgs__msg__CIOState__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_xarm_msgs
xarm_msgs__msg__CIOState *
xarm_msgs__msg__CIOState__create();

/// Destroy msg/CIOState message.
/**
 * It calls
 * xarm_msgs__msg__CIOState__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_xarm_msgs
void
xarm_msgs__msg__CIOState__destroy(xarm_msgs__msg__CIOState * msg);

/// Check for msg/CIOState message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_xarm_msgs
bool
xarm_msgs__msg__CIOState__are_equal(const xarm_msgs__msg__CIOState * lhs, const xarm_msgs__msg__CIOState * rhs);

/// Copy a msg/CIOState message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_xarm_msgs
bool
xarm_msgs__msg__CIOState__copy(
  const xarm_msgs__msg__CIOState * input,
  xarm_msgs__msg__CIOState * output);

/// Initialize array of msg/CIOState messages.
/**
 * It allocates the memory for the number of elements and calls
 * xarm_msgs__msg__CIOState__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_xarm_msgs
bool
xarm_msgs__msg__CIOState__Sequence__init(xarm_msgs__msg__CIOState__Sequence * array, size_t size);

/// Finalize array of msg/CIOState messages.
/**
 * It calls
 * xarm_msgs__msg__CIOState__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_xarm_msgs
void
xarm_msgs__msg__CIOState__Sequence__fini(xarm_msgs__msg__CIOState__Sequence * array);

/// Create array of msg/CIOState messages.
/**
 * It allocates the memory for the array and calls
 * xarm_msgs__msg__CIOState__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_xarm_msgs
xarm_msgs__msg__CIOState__Sequence *
xarm_msgs__msg__CIOState__Sequence__create(size_t size);

/// Destroy array of msg/CIOState messages.
/**
 * It calls
 * xarm_msgs__msg__CIOState__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_xarm_msgs
void
xarm_msgs__msg__CIOState__Sequence__destroy(xarm_msgs__msg__CIOState__Sequence * array);

/// Check for msg/CIOState message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_xarm_msgs
bool
xarm_msgs__msg__CIOState__Sequence__are_equal(const xarm_msgs__msg__CIOState__Sequence * lhs, const xarm_msgs__msg__CIOState__Sequence * rhs);

/// Copy an array of msg/CIOState messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_xarm_msgs
bool
xarm_msgs__msg__CIOState__Sequence__copy(
  const xarm_msgs__msg__CIOState__Sequence * input,
  xarm_msgs__msg__CIOState__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // XARM_MSGS__MSG__DETAIL__CIO_STATE__FUNCTIONS_H_
