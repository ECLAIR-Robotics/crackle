// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from xarm_msgs:srv/MoveVelocity.idl
// generated code does not contain a copyright notice
#include "xarm_msgs/srv/detail/move_velocity__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"

// Include directives for member types
// Member `speeds`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
xarm_msgs__srv__MoveVelocity_Request__init(xarm_msgs__srv__MoveVelocity_Request * msg)
{
  if (!msg) {
    return false;
  }
  // speeds
  if (!rosidl_runtime_c__float__Sequence__init(&msg->speeds, 0)) {
    xarm_msgs__srv__MoveVelocity_Request__fini(msg);
    return false;
  }
  // is_sync
  msg->is_sync = true;
  // is_tool_coord
  msg->is_tool_coord = false;
  // duration
  msg->duration = -1.0f;
  return true;
}

void
xarm_msgs__srv__MoveVelocity_Request__fini(xarm_msgs__srv__MoveVelocity_Request * msg)
{
  if (!msg) {
    return;
  }
  // speeds
  rosidl_runtime_c__float__Sequence__fini(&msg->speeds);
  // is_sync
  // is_tool_coord
  // duration
}

bool
xarm_msgs__srv__MoveVelocity_Request__are_equal(const xarm_msgs__srv__MoveVelocity_Request * lhs, const xarm_msgs__srv__MoveVelocity_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // speeds
  if (!rosidl_runtime_c__float__Sequence__are_equal(
      &(lhs->speeds), &(rhs->speeds)))
  {
    return false;
  }
  // is_sync
  if (lhs->is_sync != rhs->is_sync) {
    return false;
  }
  // is_tool_coord
  if (lhs->is_tool_coord != rhs->is_tool_coord) {
    return false;
  }
  // duration
  if (lhs->duration != rhs->duration) {
    return false;
  }
  return true;
}

bool
xarm_msgs__srv__MoveVelocity_Request__copy(
  const xarm_msgs__srv__MoveVelocity_Request * input,
  xarm_msgs__srv__MoveVelocity_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // speeds
  if (!rosidl_runtime_c__float__Sequence__copy(
      &(input->speeds), &(output->speeds)))
  {
    return false;
  }
  // is_sync
  output->is_sync = input->is_sync;
  // is_tool_coord
  output->is_tool_coord = input->is_tool_coord;
  // duration
  output->duration = input->duration;
  return true;
}

xarm_msgs__srv__MoveVelocity_Request *
xarm_msgs__srv__MoveVelocity_Request__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xarm_msgs__srv__MoveVelocity_Request * msg = (xarm_msgs__srv__MoveVelocity_Request *)allocator.allocate(sizeof(xarm_msgs__srv__MoveVelocity_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(xarm_msgs__srv__MoveVelocity_Request));
  bool success = xarm_msgs__srv__MoveVelocity_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
xarm_msgs__srv__MoveVelocity_Request__destroy(xarm_msgs__srv__MoveVelocity_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    xarm_msgs__srv__MoveVelocity_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
xarm_msgs__srv__MoveVelocity_Request__Sequence__init(xarm_msgs__srv__MoveVelocity_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xarm_msgs__srv__MoveVelocity_Request * data = NULL;

  if (size) {
    data = (xarm_msgs__srv__MoveVelocity_Request *)allocator.zero_allocate(size, sizeof(xarm_msgs__srv__MoveVelocity_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = xarm_msgs__srv__MoveVelocity_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        xarm_msgs__srv__MoveVelocity_Request__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
xarm_msgs__srv__MoveVelocity_Request__Sequence__fini(xarm_msgs__srv__MoveVelocity_Request__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      xarm_msgs__srv__MoveVelocity_Request__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

xarm_msgs__srv__MoveVelocity_Request__Sequence *
xarm_msgs__srv__MoveVelocity_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xarm_msgs__srv__MoveVelocity_Request__Sequence * array = (xarm_msgs__srv__MoveVelocity_Request__Sequence *)allocator.allocate(sizeof(xarm_msgs__srv__MoveVelocity_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = xarm_msgs__srv__MoveVelocity_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
xarm_msgs__srv__MoveVelocity_Request__Sequence__destroy(xarm_msgs__srv__MoveVelocity_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    xarm_msgs__srv__MoveVelocity_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
xarm_msgs__srv__MoveVelocity_Request__Sequence__are_equal(const xarm_msgs__srv__MoveVelocity_Request__Sequence * lhs, const xarm_msgs__srv__MoveVelocity_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!xarm_msgs__srv__MoveVelocity_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
xarm_msgs__srv__MoveVelocity_Request__Sequence__copy(
  const xarm_msgs__srv__MoveVelocity_Request__Sequence * input,
  xarm_msgs__srv__MoveVelocity_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(xarm_msgs__srv__MoveVelocity_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    xarm_msgs__srv__MoveVelocity_Request * data =
      (xarm_msgs__srv__MoveVelocity_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!xarm_msgs__srv__MoveVelocity_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          xarm_msgs__srv__MoveVelocity_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!xarm_msgs__srv__MoveVelocity_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `message`
#include "rosidl_runtime_c/string_functions.h"

bool
xarm_msgs__srv__MoveVelocity_Response__init(xarm_msgs__srv__MoveVelocity_Response * msg)
{
  if (!msg) {
    return false;
  }
  // ret
  // message
  if (!rosidl_runtime_c__String__init(&msg->message)) {
    xarm_msgs__srv__MoveVelocity_Response__fini(msg);
    return false;
  }
  return true;
}

void
xarm_msgs__srv__MoveVelocity_Response__fini(xarm_msgs__srv__MoveVelocity_Response * msg)
{
  if (!msg) {
    return;
  }
  // ret
  // message
  rosidl_runtime_c__String__fini(&msg->message);
}

bool
xarm_msgs__srv__MoveVelocity_Response__are_equal(const xarm_msgs__srv__MoveVelocity_Response * lhs, const xarm_msgs__srv__MoveVelocity_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // ret
  if (lhs->ret != rhs->ret) {
    return false;
  }
  // message
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->message), &(rhs->message)))
  {
    return false;
  }
  return true;
}

bool
xarm_msgs__srv__MoveVelocity_Response__copy(
  const xarm_msgs__srv__MoveVelocity_Response * input,
  xarm_msgs__srv__MoveVelocity_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // ret
  output->ret = input->ret;
  // message
  if (!rosidl_runtime_c__String__copy(
      &(input->message), &(output->message)))
  {
    return false;
  }
  return true;
}

xarm_msgs__srv__MoveVelocity_Response *
xarm_msgs__srv__MoveVelocity_Response__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xarm_msgs__srv__MoveVelocity_Response * msg = (xarm_msgs__srv__MoveVelocity_Response *)allocator.allocate(sizeof(xarm_msgs__srv__MoveVelocity_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(xarm_msgs__srv__MoveVelocity_Response));
  bool success = xarm_msgs__srv__MoveVelocity_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
xarm_msgs__srv__MoveVelocity_Response__destroy(xarm_msgs__srv__MoveVelocity_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    xarm_msgs__srv__MoveVelocity_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
xarm_msgs__srv__MoveVelocity_Response__Sequence__init(xarm_msgs__srv__MoveVelocity_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xarm_msgs__srv__MoveVelocity_Response * data = NULL;

  if (size) {
    data = (xarm_msgs__srv__MoveVelocity_Response *)allocator.zero_allocate(size, sizeof(xarm_msgs__srv__MoveVelocity_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = xarm_msgs__srv__MoveVelocity_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        xarm_msgs__srv__MoveVelocity_Response__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
xarm_msgs__srv__MoveVelocity_Response__Sequence__fini(xarm_msgs__srv__MoveVelocity_Response__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      xarm_msgs__srv__MoveVelocity_Response__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

xarm_msgs__srv__MoveVelocity_Response__Sequence *
xarm_msgs__srv__MoveVelocity_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xarm_msgs__srv__MoveVelocity_Response__Sequence * array = (xarm_msgs__srv__MoveVelocity_Response__Sequence *)allocator.allocate(sizeof(xarm_msgs__srv__MoveVelocity_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = xarm_msgs__srv__MoveVelocity_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
xarm_msgs__srv__MoveVelocity_Response__Sequence__destroy(xarm_msgs__srv__MoveVelocity_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    xarm_msgs__srv__MoveVelocity_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
xarm_msgs__srv__MoveVelocity_Response__Sequence__are_equal(const xarm_msgs__srv__MoveVelocity_Response__Sequence * lhs, const xarm_msgs__srv__MoveVelocity_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!xarm_msgs__srv__MoveVelocity_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
xarm_msgs__srv__MoveVelocity_Response__Sequence__copy(
  const xarm_msgs__srv__MoveVelocity_Response__Sequence * input,
  xarm_msgs__srv__MoveVelocity_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(xarm_msgs__srv__MoveVelocity_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    xarm_msgs__srv__MoveVelocity_Response * data =
      (xarm_msgs__srv__MoveVelocity_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!xarm_msgs__srv__MoveVelocity_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          xarm_msgs__srv__MoveVelocity_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!xarm_msgs__srv__MoveVelocity_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
