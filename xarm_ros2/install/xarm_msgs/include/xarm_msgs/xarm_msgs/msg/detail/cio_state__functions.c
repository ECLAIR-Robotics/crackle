// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from xarm_msgs:msg/CIOState.idl
// generated code does not contain a copyright notice
#include "xarm_msgs/msg/detail/cio_state__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"

bool
xarm_msgs__msg__CIOState__init(xarm_msgs__msg__CIOState * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    xarm_msgs__msg__CIOState__fini(msg);
    return false;
  }
  // state
  // code
  // input_digitals
  // output_digitals
  // input_analogs
  // output_analogs
  // input_conf
  // output_conf
  return true;
}

void
xarm_msgs__msg__CIOState__fini(xarm_msgs__msg__CIOState * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // state
  // code
  // input_digitals
  // output_digitals
  // input_analogs
  // output_analogs
  // input_conf
  // output_conf
}

bool
xarm_msgs__msg__CIOState__are_equal(const xarm_msgs__msg__CIOState * lhs, const xarm_msgs__msg__CIOState * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // state
  if (lhs->state != rhs->state) {
    return false;
  }
  // code
  if (lhs->code != rhs->code) {
    return false;
  }
  // input_digitals
  for (size_t i = 0; i < 2; ++i) {
    if (lhs->input_digitals[i] != rhs->input_digitals[i]) {
      return false;
    }
  }
  // output_digitals
  for (size_t i = 0; i < 2; ++i) {
    if (lhs->output_digitals[i] != rhs->output_digitals[i]) {
      return false;
    }
  }
  // input_analogs
  for (size_t i = 0; i < 2; ++i) {
    if (lhs->input_analogs[i] != rhs->input_analogs[i]) {
      return false;
    }
  }
  // output_analogs
  for (size_t i = 0; i < 2; ++i) {
    if (lhs->output_analogs[i] != rhs->output_analogs[i]) {
      return false;
    }
  }
  // input_conf
  for (size_t i = 0; i < 16; ++i) {
    if (lhs->input_conf[i] != rhs->input_conf[i]) {
      return false;
    }
  }
  // output_conf
  for (size_t i = 0; i < 16; ++i) {
    if (lhs->output_conf[i] != rhs->output_conf[i]) {
      return false;
    }
  }
  return true;
}

bool
xarm_msgs__msg__CIOState__copy(
  const xarm_msgs__msg__CIOState * input,
  xarm_msgs__msg__CIOState * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // state
  output->state = input->state;
  // code
  output->code = input->code;
  // input_digitals
  for (size_t i = 0; i < 2; ++i) {
    output->input_digitals[i] = input->input_digitals[i];
  }
  // output_digitals
  for (size_t i = 0; i < 2; ++i) {
    output->output_digitals[i] = input->output_digitals[i];
  }
  // input_analogs
  for (size_t i = 0; i < 2; ++i) {
    output->input_analogs[i] = input->input_analogs[i];
  }
  // output_analogs
  for (size_t i = 0; i < 2; ++i) {
    output->output_analogs[i] = input->output_analogs[i];
  }
  // input_conf
  for (size_t i = 0; i < 16; ++i) {
    output->input_conf[i] = input->input_conf[i];
  }
  // output_conf
  for (size_t i = 0; i < 16; ++i) {
    output->output_conf[i] = input->output_conf[i];
  }
  return true;
}

xarm_msgs__msg__CIOState *
xarm_msgs__msg__CIOState__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xarm_msgs__msg__CIOState * msg = (xarm_msgs__msg__CIOState *)allocator.allocate(sizeof(xarm_msgs__msg__CIOState), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(xarm_msgs__msg__CIOState));
  bool success = xarm_msgs__msg__CIOState__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
xarm_msgs__msg__CIOState__destroy(xarm_msgs__msg__CIOState * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    xarm_msgs__msg__CIOState__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
xarm_msgs__msg__CIOState__Sequence__init(xarm_msgs__msg__CIOState__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xarm_msgs__msg__CIOState * data = NULL;

  if (size) {
    data = (xarm_msgs__msg__CIOState *)allocator.zero_allocate(size, sizeof(xarm_msgs__msg__CIOState), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = xarm_msgs__msg__CIOState__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        xarm_msgs__msg__CIOState__fini(&data[i - 1]);
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
xarm_msgs__msg__CIOState__Sequence__fini(xarm_msgs__msg__CIOState__Sequence * array)
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
      xarm_msgs__msg__CIOState__fini(&array->data[i]);
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

xarm_msgs__msg__CIOState__Sequence *
xarm_msgs__msg__CIOState__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xarm_msgs__msg__CIOState__Sequence * array = (xarm_msgs__msg__CIOState__Sequence *)allocator.allocate(sizeof(xarm_msgs__msg__CIOState__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = xarm_msgs__msg__CIOState__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
xarm_msgs__msg__CIOState__Sequence__destroy(xarm_msgs__msg__CIOState__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    xarm_msgs__msg__CIOState__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
xarm_msgs__msg__CIOState__Sequence__are_equal(const xarm_msgs__msg__CIOState__Sequence * lhs, const xarm_msgs__msg__CIOState__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!xarm_msgs__msg__CIOState__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
xarm_msgs__msg__CIOState__Sequence__copy(
  const xarm_msgs__msg__CIOState__Sequence * input,
  xarm_msgs__msg__CIOState__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(xarm_msgs__msg__CIOState);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    xarm_msgs__msg__CIOState * data =
      (xarm_msgs__msg__CIOState *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!xarm_msgs__msg__CIOState__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          xarm_msgs__msg__CIOState__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!xarm_msgs__msg__CIOState__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
