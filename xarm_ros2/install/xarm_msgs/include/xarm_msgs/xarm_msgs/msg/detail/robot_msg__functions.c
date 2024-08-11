// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from xarm_msgs:msg/RobotMsg.idl
// generated code does not contain a copyright notice
#include "xarm_msgs/msg/detail/robot_msg__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `angle`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
xarm_msgs__msg__RobotMsg__init(xarm_msgs__msg__RobotMsg * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    xarm_msgs__msg__RobotMsg__fini(msg);
    return false;
  }
  // state
  // mode
  // cmdnum
  // mt_brake
  // mt_able
  // err
  // warn
  // angle
  if (!rosidl_runtime_c__float__Sequence__init(&msg->angle, 0)) {
    xarm_msgs__msg__RobotMsg__fini(msg);
    return false;
  }
  // pose
  // offset
  return true;
}

void
xarm_msgs__msg__RobotMsg__fini(xarm_msgs__msg__RobotMsg * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // state
  // mode
  // cmdnum
  // mt_brake
  // mt_able
  // err
  // warn
  // angle
  rosidl_runtime_c__float__Sequence__fini(&msg->angle);
  // pose
  // offset
}

bool
xarm_msgs__msg__RobotMsg__are_equal(const xarm_msgs__msg__RobotMsg * lhs, const xarm_msgs__msg__RobotMsg * rhs)
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
  // mode
  if (lhs->mode != rhs->mode) {
    return false;
  }
  // cmdnum
  if (lhs->cmdnum != rhs->cmdnum) {
    return false;
  }
  // mt_brake
  if (lhs->mt_brake != rhs->mt_brake) {
    return false;
  }
  // mt_able
  if (lhs->mt_able != rhs->mt_able) {
    return false;
  }
  // err
  if (lhs->err != rhs->err) {
    return false;
  }
  // warn
  if (lhs->warn != rhs->warn) {
    return false;
  }
  // angle
  if (!rosidl_runtime_c__float__Sequence__are_equal(
      &(lhs->angle), &(rhs->angle)))
  {
    return false;
  }
  // pose
  for (size_t i = 0; i < 6; ++i) {
    if (lhs->pose[i] != rhs->pose[i]) {
      return false;
    }
  }
  // offset
  for (size_t i = 0; i < 6; ++i) {
    if (lhs->offset[i] != rhs->offset[i]) {
      return false;
    }
  }
  return true;
}

bool
xarm_msgs__msg__RobotMsg__copy(
  const xarm_msgs__msg__RobotMsg * input,
  xarm_msgs__msg__RobotMsg * output)
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
  // mode
  output->mode = input->mode;
  // cmdnum
  output->cmdnum = input->cmdnum;
  // mt_brake
  output->mt_brake = input->mt_brake;
  // mt_able
  output->mt_able = input->mt_able;
  // err
  output->err = input->err;
  // warn
  output->warn = input->warn;
  // angle
  if (!rosidl_runtime_c__float__Sequence__copy(
      &(input->angle), &(output->angle)))
  {
    return false;
  }
  // pose
  for (size_t i = 0; i < 6; ++i) {
    output->pose[i] = input->pose[i];
  }
  // offset
  for (size_t i = 0; i < 6; ++i) {
    output->offset[i] = input->offset[i];
  }
  return true;
}

xarm_msgs__msg__RobotMsg *
xarm_msgs__msg__RobotMsg__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xarm_msgs__msg__RobotMsg * msg = (xarm_msgs__msg__RobotMsg *)allocator.allocate(sizeof(xarm_msgs__msg__RobotMsg), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(xarm_msgs__msg__RobotMsg));
  bool success = xarm_msgs__msg__RobotMsg__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
xarm_msgs__msg__RobotMsg__destroy(xarm_msgs__msg__RobotMsg * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    xarm_msgs__msg__RobotMsg__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
xarm_msgs__msg__RobotMsg__Sequence__init(xarm_msgs__msg__RobotMsg__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xarm_msgs__msg__RobotMsg * data = NULL;

  if (size) {
    data = (xarm_msgs__msg__RobotMsg *)allocator.zero_allocate(size, sizeof(xarm_msgs__msg__RobotMsg), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = xarm_msgs__msg__RobotMsg__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        xarm_msgs__msg__RobotMsg__fini(&data[i - 1]);
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
xarm_msgs__msg__RobotMsg__Sequence__fini(xarm_msgs__msg__RobotMsg__Sequence * array)
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
      xarm_msgs__msg__RobotMsg__fini(&array->data[i]);
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

xarm_msgs__msg__RobotMsg__Sequence *
xarm_msgs__msg__RobotMsg__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xarm_msgs__msg__RobotMsg__Sequence * array = (xarm_msgs__msg__RobotMsg__Sequence *)allocator.allocate(sizeof(xarm_msgs__msg__RobotMsg__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = xarm_msgs__msg__RobotMsg__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
xarm_msgs__msg__RobotMsg__Sequence__destroy(xarm_msgs__msg__RobotMsg__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    xarm_msgs__msg__RobotMsg__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
xarm_msgs__msg__RobotMsg__Sequence__are_equal(const xarm_msgs__msg__RobotMsg__Sequence * lhs, const xarm_msgs__msg__RobotMsg__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!xarm_msgs__msg__RobotMsg__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
xarm_msgs__msg__RobotMsg__Sequence__copy(
  const xarm_msgs__msg__RobotMsg__Sequence * input,
  xarm_msgs__msg__RobotMsg__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(xarm_msgs__msg__RobotMsg);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    xarm_msgs__msg__RobotMsg * data =
      (xarm_msgs__msg__RobotMsg *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!xarm_msgs__msg__RobotMsg__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          xarm_msgs__msg__RobotMsg__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!xarm_msgs__msg__RobotMsg__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
