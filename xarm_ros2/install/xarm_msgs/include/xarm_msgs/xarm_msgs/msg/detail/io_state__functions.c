// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from xarm_msgs:msg/IOState.idl
// generated code does not contain a copyright notice
#include "xarm_msgs/msg/detail/io_state__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
xarm_msgs__msg__IOState__init(xarm_msgs__msg__IOState * msg)
{
  if (!msg) {
    return false;
  }
  // digital_1
  // digital_2
  // analog_1
  // analog_2
  return true;
}

void
xarm_msgs__msg__IOState__fini(xarm_msgs__msg__IOState * msg)
{
  if (!msg) {
    return;
  }
  // digital_1
  // digital_2
  // analog_1
  // analog_2
}

bool
xarm_msgs__msg__IOState__are_equal(const xarm_msgs__msg__IOState * lhs, const xarm_msgs__msg__IOState * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // digital_1
  if (lhs->digital_1 != rhs->digital_1) {
    return false;
  }
  // digital_2
  if (lhs->digital_2 != rhs->digital_2) {
    return false;
  }
  // analog_1
  if (lhs->analog_1 != rhs->analog_1) {
    return false;
  }
  // analog_2
  if (lhs->analog_2 != rhs->analog_2) {
    return false;
  }
  return true;
}

bool
xarm_msgs__msg__IOState__copy(
  const xarm_msgs__msg__IOState * input,
  xarm_msgs__msg__IOState * output)
{
  if (!input || !output) {
    return false;
  }
  // digital_1
  output->digital_1 = input->digital_1;
  // digital_2
  output->digital_2 = input->digital_2;
  // analog_1
  output->analog_1 = input->analog_1;
  // analog_2
  output->analog_2 = input->analog_2;
  return true;
}

xarm_msgs__msg__IOState *
xarm_msgs__msg__IOState__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xarm_msgs__msg__IOState * msg = (xarm_msgs__msg__IOState *)allocator.allocate(sizeof(xarm_msgs__msg__IOState), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(xarm_msgs__msg__IOState));
  bool success = xarm_msgs__msg__IOState__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
xarm_msgs__msg__IOState__destroy(xarm_msgs__msg__IOState * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    xarm_msgs__msg__IOState__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
xarm_msgs__msg__IOState__Sequence__init(xarm_msgs__msg__IOState__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xarm_msgs__msg__IOState * data = NULL;

  if (size) {
    data = (xarm_msgs__msg__IOState *)allocator.zero_allocate(size, sizeof(xarm_msgs__msg__IOState), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = xarm_msgs__msg__IOState__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        xarm_msgs__msg__IOState__fini(&data[i - 1]);
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
xarm_msgs__msg__IOState__Sequence__fini(xarm_msgs__msg__IOState__Sequence * array)
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
      xarm_msgs__msg__IOState__fini(&array->data[i]);
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

xarm_msgs__msg__IOState__Sequence *
xarm_msgs__msg__IOState__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xarm_msgs__msg__IOState__Sequence * array = (xarm_msgs__msg__IOState__Sequence *)allocator.allocate(sizeof(xarm_msgs__msg__IOState__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = xarm_msgs__msg__IOState__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
xarm_msgs__msg__IOState__Sequence__destroy(xarm_msgs__msg__IOState__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    xarm_msgs__msg__IOState__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
xarm_msgs__msg__IOState__Sequence__are_equal(const xarm_msgs__msg__IOState__Sequence * lhs, const xarm_msgs__msg__IOState__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!xarm_msgs__msg__IOState__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
xarm_msgs__msg__IOState__Sequence__copy(
  const xarm_msgs__msg__IOState__Sequence * input,
  xarm_msgs__msg__IOState__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(xarm_msgs__msg__IOState);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    xarm_msgs__msg__IOState * data =
      (xarm_msgs__msg__IOState *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!xarm_msgs__msg__IOState__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          xarm_msgs__msg__IOState__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!xarm_msgs__msg__IOState__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
