// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from arm_interface:action/UfArm.idl
// generated code does not contain a copyright notice
#include "arm_interface/action/detail/uf_arm__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `goal_pose`
#include "geometry_msgs/msg/detail/pose__functions.h"
// Member `pose`
#include "rosidl_runtime_c/string_functions.h"

bool
arm_interface__action__UfArm_Goal__init(arm_interface__action__UfArm_Goal * msg)
{
  if (!msg) {
    return false;
  }
  // goal_pose
  if (!geometry_msgs__msg__Pose__init(&msg->goal_pose)) {
    arm_interface__action__UfArm_Goal__fini(msg);
    return false;
  }
  // pose
  if (!rosidl_runtime_c__String__init(&msg->pose)) {
    arm_interface__action__UfArm_Goal__fini(msg);
    return false;
  }
  return true;
}

void
arm_interface__action__UfArm_Goal__fini(arm_interface__action__UfArm_Goal * msg)
{
  if (!msg) {
    return;
  }
  // goal_pose
  geometry_msgs__msg__Pose__fini(&msg->goal_pose);
  // pose
  rosidl_runtime_c__String__fini(&msg->pose);
}

bool
arm_interface__action__UfArm_Goal__are_equal(const arm_interface__action__UfArm_Goal * lhs, const arm_interface__action__UfArm_Goal * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // goal_pose
  if (!geometry_msgs__msg__Pose__are_equal(
      &(lhs->goal_pose), &(rhs->goal_pose)))
  {
    return false;
  }
  // pose
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->pose), &(rhs->pose)))
  {
    return false;
  }
  return true;
}

bool
arm_interface__action__UfArm_Goal__copy(
  const arm_interface__action__UfArm_Goal * input,
  arm_interface__action__UfArm_Goal * output)
{
  if (!input || !output) {
    return false;
  }
  // goal_pose
  if (!geometry_msgs__msg__Pose__copy(
      &(input->goal_pose), &(output->goal_pose)))
  {
    return false;
  }
  // pose
  if (!rosidl_runtime_c__String__copy(
      &(input->pose), &(output->pose)))
  {
    return false;
  }
  return true;
}

arm_interface__action__UfArm_Goal *
arm_interface__action__UfArm_Goal__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_Goal * msg = (arm_interface__action__UfArm_Goal *)allocator.allocate(sizeof(arm_interface__action__UfArm_Goal), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(arm_interface__action__UfArm_Goal));
  bool success = arm_interface__action__UfArm_Goal__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
arm_interface__action__UfArm_Goal__destroy(arm_interface__action__UfArm_Goal * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    arm_interface__action__UfArm_Goal__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
arm_interface__action__UfArm_Goal__Sequence__init(arm_interface__action__UfArm_Goal__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_Goal * data = NULL;

  if (size) {
    data = (arm_interface__action__UfArm_Goal *)allocator.zero_allocate(size, sizeof(arm_interface__action__UfArm_Goal), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = arm_interface__action__UfArm_Goal__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        arm_interface__action__UfArm_Goal__fini(&data[i - 1]);
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
arm_interface__action__UfArm_Goal__Sequence__fini(arm_interface__action__UfArm_Goal__Sequence * array)
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
      arm_interface__action__UfArm_Goal__fini(&array->data[i]);
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

arm_interface__action__UfArm_Goal__Sequence *
arm_interface__action__UfArm_Goal__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_Goal__Sequence * array = (arm_interface__action__UfArm_Goal__Sequence *)allocator.allocate(sizeof(arm_interface__action__UfArm_Goal__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = arm_interface__action__UfArm_Goal__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
arm_interface__action__UfArm_Goal__Sequence__destroy(arm_interface__action__UfArm_Goal__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    arm_interface__action__UfArm_Goal__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
arm_interface__action__UfArm_Goal__Sequence__are_equal(const arm_interface__action__UfArm_Goal__Sequence * lhs, const arm_interface__action__UfArm_Goal__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!arm_interface__action__UfArm_Goal__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
arm_interface__action__UfArm_Goal__Sequence__copy(
  const arm_interface__action__UfArm_Goal__Sequence * input,
  arm_interface__action__UfArm_Goal__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(arm_interface__action__UfArm_Goal);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    arm_interface__action__UfArm_Goal * data =
      (arm_interface__action__UfArm_Goal *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!arm_interface__action__UfArm_Goal__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          arm_interface__action__UfArm_Goal__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!arm_interface__action__UfArm_Goal__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `final_pose`
// already included above
// #include "geometry_msgs/msg/detail/pose__functions.h"
// Member `message`
// already included above
// #include "rosidl_runtime_c/string_functions.h"

bool
arm_interface__action__UfArm_Result__init(arm_interface__action__UfArm_Result * msg)
{
  if (!msg) {
    return false;
  }
  // final_pose
  if (!geometry_msgs__msg__Pose__init(&msg->final_pose)) {
    arm_interface__action__UfArm_Result__fini(msg);
    return false;
  }
  // success
  // message
  if (!rosidl_runtime_c__String__init(&msg->message)) {
    arm_interface__action__UfArm_Result__fini(msg);
    return false;
  }
  return true;
}

void
arm_interface__action__UfArm_Result__fini(arm_interface__action__UfArm_Result * msg)
{
  if (!msg) {
    return;
  }
  // final_pose
  geometry_msgs__msg__Pose__fini(&msg->final_pose);
  // success
  // message
  rosidl_runtime_c__String__fini(&msg->message);
}

bool
arm_interface__action__UfArm_Result__are_equal(const arm_interface__action__UfArm_Result * lhs, const arm_interface__action__UfArm_Result * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // final_pose
  if (!geometry_msgs__msg__Pose__are_equal(
      &(lhs->final_pose), &(rhs->final_pose)))
  {
    return false;
  }
  // success
  if (lhs->success != rhs->success) {
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
arm_interface__action__UfArm_Result__copy(
  const arm_interface__action__UfArm_Result * input,
  arm_interface__action__UfArm_Result * output)
{
  if (!input || !output) {
    return false;
  }
  // final_pose
  if (!geometry_msgs__msg__Pose__copy(
      &(input->final_pose), &(output->final_pose)))
  {
    return false;
  }
  // success
  output->success = input->success;
  // message
  if (!rosidl_runtime_c__String__copy(
      &(input->message), &(output->message)))
  {
    return false;
  }
  return true;
}

arm_interface__action__UfArm_Result *
arm_interface__action__UfArm_Result__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_Result * msg = (arm_interface__action__UfArm_Result *)allocator.allocate(sizeof(arm_interface__action__UfArm_Result), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(arm_interface__action__UfArm_Result));
  bool success = arm_interface__action__UfArm_Result__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
arm_interface__action__UfArm_Result__destroy(arm_interface__action__UfArm_Result * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    arm_interface__action__UfArm_Result__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
arm_interface__action__UfArm_Result__Sequence__init(arm_interface__action__UfArm_Result__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_Result * data = NULL;

  if (size) {
    data = (arm_interface__action__UfArm_Result *)allocator.zero_allocate(size, sizeof(arm_interface__action__UfArm_Result), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = arm_interface__action__UfArm_Result__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        arm_interface__action__UfArm_Result__fini(&data[i - 1]);
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
arm_interface__action__UfArm_Result__Sequence__fini(arm_interface__action__UfArm_Result__Sequence * array)
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
      arm_interface__action__UfArm_Result__fini(&array->data[i]);
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

arm_interface__action__UfArm_Result__Sequence *
arm_interface__action__UfArm_Result__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_Result__Sequence * array = (arm_interface__action__UfArm_Result__Sequence *)allocator.allocate(sizeof(arm_interface__action__UfArm_Result__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = arm_interface__action__UfArm_Result__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
arm_interface__action__UfArm_Result__Sequence__destroy(arm_interface__action__UfArm_Result__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    arm_interface__action__UfArm_Result__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
arm_interface__action__UfArm_Result__Sequence__are_equal(const arm_interface__action__UfArm_Result__Sequence * lhs, const arm_interface__action__UfArm_Result__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!arm_interface__action__UfArm_Result__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
arm_interface__action__UfArm_Result__Sequence__copy(
  const arm_interface__action__UfArm_Result__Sequence * input,
  arm_interface__action__UfArm_Result__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(arm_interface__action__UfArm_Result);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    arm_interface__action__UfArm_Result * data =
      (arm_interface__action__UfArm_Result *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!arm_interface__action__UfArm_Result__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          arm_interface__action__UfArm_Result__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!arm_interface__action__UfArm_Result__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `current_pose`
// already included above
// #include "geometry_msgs/msg/detail/pose__functions.h"
// Member `status`
#include "actionlib_msgs/msg/detail/goal_status__functions.h"

bool
arm_interface__action__UfArm_Feedback__init(arm_interface__action__UfArm_Feedback * msg)
{
  if (!msg) {
    return false;
  }
  // current_pose
  if (!geometry_msgs__msg__Pose__init(&msg->current_pose)) {
    arm_interface__action__UfArm_Feedback__fini(msg);
    return false;
  }
  // status
  if (!actionlib_msgs__msg__GoalStatus__init(&msg->status)) {
    arm_interface__action__UfArm_Feedback__fini(msg);
    return false;
  }
  return true;
}

void
arm_interface__action__UfArm_Feedback__fini(arm_interface__action__UfArm_Feedback * msg)
{
  if (!msg) {
    return;
  }
  // current_pose
  geometry_msgs__msg__Pose__fini(&msg->current_pose);
  // status
  actionlib_msgs__msg__GoalStatus__fini(&msg->status);
}

bool
arm_interface__action__UfArm_Feedback__are_equal(const arm_interface__action__UfArm_Feedback * lhs, const arm_interface__action__UfArm_Feedback * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // current_pose
  if (!geometry_msgs__msg__Pose__are_equal(
      &(lhs->current_pose), &(rhs->current_pose)))
  {
    return false;
  }
  // status
  if (!actionlib_msgs__msg__GoalStatus__are_equal(
      &(lhs->status), &(rhs->status)))
  {
    return false;
  }
  return true;
}

bool
arm_interface__action__UfArm_Feedback__copy(
  const arm_interface__action__UfArm_Feedback * input,
  arm_interface__action__UfArm_Feedback * output)
{
  if (!input || !output) {
    return false;
  }
  // current_pose
  if (!geometry_msgs__msg__Pose__copy(
      &(input->current_pose), &(output->current_pose)))
  {
    return false;
  }
  // status
  if (!actionlib_msgs__msg__GoalStatus__copy(
      &(input->status), &(output->status)))
  {
    return false;
  }
  return true;
}

arm_interface__action__UfArm_Feedback *
arm_interface__action__UfArm_Feedback__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_Feedback * msg = (arm_interface__action__UfArm_Feedback *)allocator.allocate(sizeof(arm_interface__action__UfArm_Feedback), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(arm_interface__action__UfArm_Feedback));
  bool success = arm_interface__action__UfArm_Feedback__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
arm_interface__action__UfArm_Feedback__destroy(arm_interface__action__UfArm_Feedback * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    arm_interface__action__UfArm_Feedback__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
arm_interface__action__UfArm_Feedback__Sequence__init(arm_interface__action__UfArm_Feedback__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_Feedback * data = NULL;

  if (size) {
    data = (arm_interface__action__UfArm_Feedback *)allocator.zero_allocate(size, sizeof(arm_interface__action__UfArm_Feedback), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = arm_interface__action__UfArm_Feedback__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        arm_interface__action__UfArm_Feedback__fini(&data[i - 1]);
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
arm_interface__action__UfArm_Feedback__Sequence__fini(arm_interface__action__UfArm_Feedback__Sequence * array)
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
      arm_interface__action__UfArm_Feedback__fini(&array->data[i]);
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

arm_interface__action__UfArm_Feedback__Sequence *
arm_interface__action__UfArm_Feedback__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_Feedback__Sequence * array = (arm_interface__action__UfArm_Feedback__Sequence *)allocator.allocate(sizeof(arm_interface__action__UfArm_Feedback__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = arm_interface__action__UfArm_Feedback__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
arm_interface__action__UfArm_Feedback__Sequence__destroy(arm_interface__action__UfArm_Feedback__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    arm_interface__action__UfArm_Feedback__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
arm_interface__action__UfArm_Feedback__Sequence__are_equal(const arm_interface__action__UfArm_Feedback__Sequence * lhs, const arm_interface__action__UfArm_Feedback__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!arm_interface__action__UfArm_Feedback__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
arm_interface__action__UfArm_Feedback__Sequence__copy(
  const arm_interface__action__UfArm_Feedback__Sequence * input,
  arm_interface__action__UfArm_Feedback__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(arm_interface__action__UfArm_Feedback);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    arm_interface__action__UfArm_Feedback * data =
      (arm_interface__action__UfArm_Feedback *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!arm_interface__action__UfArm_Feedback__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          arm_interface__action__UfArm_Feedback__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!arm_interface__action__UfArm_Feedback__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `goal_id`
#include "unique_identifier_msgs/msg/detail/uuid__functions.h"
// Member `goal`
// already included above
// #include "arm_interface/action/detail/uf_arm__functions.h"

bool
arm_interface__action__UfArm_SendGoal_Request__init(arm_interface__action__UfArm_SendGoal_Request * msg)
{
  if (!msg) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__init(&msg->goal_id)) {
    arm_interface__action__UfArm_SendGoal_Request__fini(msg);
    return false;
  }
  // goal
  if (!arm_interface__action__UfArm_Goal__init(&msg->goal)) {
    arm_interface__action__UfArm_SendGoal_Request__fini(msg);
    return false;
  }
  return true;
}

void
arm_interface__action__UfArm_SendGoal_Request__fini(arm_interface__action__UfArm_SendGoal_Request * msg)
{
  if (!msg) {
    return;
  }
  // goal_id
  unique_identifier_msgs__msg__UUID__fini(&msg->goal_id);
  // goal
  arm_interface__action__UfArm_Goal__fini(&msg->goal);
}

bool
arm_interface__action__UfArm_SendGoal_Request__are_equal(const arm_interface__action__UfArm_SendGoal_Request * lhs, const arm_interface__action__UfArm_SendGoal_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__are_equal(
      &(lhs->goal_id), &(rhs->goal_id)))
  {
    return false;
  }
  // goal
  if (!arm_interface__action__UfArm_Goal__are_equal(
      &(lhs->goal), &(rhs->goal)))
  {
    return false;
  }
  return true;
}

bool
arm_interface__action__UfArm_SendGoal_Request__copy(
  const arm_interface__action__UfArm_SendGoal_Request * input,
  arm_interface__action__UfArm_SendGoal_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__copy(
      &(input->goal_id), &(output->goal_id)))
  {
    return false;
  }
  // goal
  if (!arm_interface__action__UfArm_Goal__copy(
      &(input->goal), &(output->goal)))
  {
    return false;
  }
  return true;
}

arm_interface__action__UfArm_SendGoal_Request *
arm_interface__action__UfArm_SendGoal_Request__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_SendGoal_Request * msg = (arm_interface__action__UfArm_SendGoal_Request *)allocator.allocate(sizeof(arm_interface__action__UfArm_SendGoal_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(arm_interface__action__UfArm_SendGoal_Request));
  bool success = arm_interface__action__UfArm_SendGoal_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
arm_interface__action__UfArm_SendGoal_Request__destroy(arm_interface__action__UfArm_SendGoal_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    arm_interface__action__UfArm_SendGoal_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
arm_interface__action__UfArm_SendGoal_Request__Sequence__init(arm_interface__action__UfArm_SendGoal_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_SendGoal_Request * data = NULL;

  if (size) {
    data = (arm_interface__action__UfArm_SendGoal_Request *)allocator.zero_allocate(size, sizeof(arm_interface__action__UfArm_SendGoal_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = arm_interface__action__UfArm_SendGoal_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        arm_interface__action__UfArm_SendGoal_Request__fini(&data[i - 1]);
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
arm_interface__action__UfArm_SendGoal_Request__Sequence__fini(arm_interface__action__UfArm_SendGoal_Request__Sequence * array)
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
      arm_interface__action__UfArm_SendGoal_Request__fini(&array->data[i]);
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

arm_interface__action__UfArm_SendGoal_Request__Sequence *
arm_interface__action__UfArm_SendGoal_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_SendGoal_Request__Sequence * array = (arm_interface__action__UfArm_SendGoal_Request__Sequence *)allocator.allocate(sizeof(arm_interface__action__UfArm_SendGoal_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = arm_interface__action__UfArm_SendGoal_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
arm_interface__action__UfArm_SendGoal_Request__Sequence__destroy(arm_interface__action__UfArm_SendGoal_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    arm_interface__action__UfArm_SendGoal_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
arm_interface__action__UfArm_SendGoal_Request__Sequence__are_equal(const arm_interface__action__UfArm_SendGoal_Request__Sequence * lhs, const arm_interface__action__UfArm_SendGoal_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!arm_interface__action__UfArm_SendGoal_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
arm_interface__action__UfArm_SendGoal_Request__Sequence__copy(
  const arm_interface__action__UfArm_SendGoal_Request__Sequence * input,
  arm_interface__action__UfArm_SendGoal_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(arm_interface__action__UfArm_SendGoal_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    arm_interface__action__UfArm_SendGoal_Request * data =
      (arm_interface__action__UfArm_SendGoal_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!arm_interface__action__UfArm_SendGoal_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          arm_interface__action__UfArm_SendGoal_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!arm_interface__action__UfArm_SendGoal_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `stamp`
#include "builtin_interfaces/msg/detail/time__functions.h"

bool
arm_interface__action__UfArm_SendGoal_Response__init(arm_interface__action__UfArm_SendGoal_Response * msg)
{
  if (!msg) {
    return false;
  }
  // accepted
  // stamp
  if (!builtin_interfaces__msg__Time__init(&msg->stamp)) {
    arm_interface__action__UfArm_SendGoal_Response__fini(msg);
    return false;
  }
  return true;
}

void
arm_interface__action__UfArm_SendGoal_Response__fini(arm_interface__action__UfArm_SendGoal_Response * msg)
{
  if (!msg) {
    return;
  }
  // accepted
  // stamp
  builtin_interfaces__msg__Time__fini(&msg->stamp);
}

bool
arm_interface__action__UfArm_SendGoal_Response__are_equal(const arm_interface__action__UfArm_SendGoal_Response * lhs, const arm_interface__action__UfArm_SendGoal_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // accepted
  if (lhs->accepted != rhs->accepted) {
    return false;
  }
  // stamp
  if (!builtin_interfaces__msg__Time__are_equal(
      &(lhs->stamp), &(rhs->stamp)))
  {
    return false;
  }
  return true;
}

bool
arm_interface__action__UfArm_SendGoal_Response__copy(
  const arm_interface__action__UfArm_SendGoal_Response * input,
  arm_interface__action__UfArm_SendGoal_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // accepted
  output->accepted = input->accepted;
  // stamp
  if (!builtin_interfaces__msg__Time__copy(
      &(input->stamp), &(output->stamp)))
  {
    return false;
  }
  return true;
}

arm_interface__action__UfArm_SendGoal_Response *
arm_interface__action__UfArm_SendGoal_Response__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_SendGoal_Response * msg = (arm_interface__action__UfArm_SendGoal_Response *)allocator.allocate(sizeof(arm_interface__action__UfArm_SendGoal_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(arm_interface__action__UfArm_SendGoal_Response));
  bool success = arm_interface__action__UfArm_SendGoal_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
arm_interface__action__UfArm_SendGoal_Response__destroy(arm_interface__action__UfArm_SendGoal_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    arm_interface__action__UfArm_SendGoal_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
arm_interface__action__UfArm_SendGoal_Response__Sequence__init(arm_interface__action__UfArm_SendGoal_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_SendGoal_Response * data = NULL;

  if (size) {
    data = (arm_interface__action__UfArm_SendGoal_Response *)allocator.zero_allocate(size, sizeof(arm_interface__action__UfArm_SendGoal_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = arm_interface__action__UfArm_SendGoal_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        arm_interface__action__UfArm_SendGoal_Response__fini(&data[i - 1]);
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
arm_interface__action__UfArm_SendGoal_Response__Sequence__fini(arm_interface__action__UfArm_SendGoal_Response__Sequence * array)
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
      arm_interface__action__UfArm_SendGoal_Response__fini(&array->data[i]);
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

arm_interface__action__UfArm_SendGoal_Response__Sequence *
arm_interface__action__UfArm_SendGoal_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_SendGoal_Response__Sequence * array = (arm_interface__action__UfArm_SendGoal_Response__Sequence *)allocator.allocate(sizeof(arm_interface__action__UfArm_SendGoal_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = arm_interface__action__UfArm_SendGoal_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
arm_interface__action__UfArm_SendGoal_Response__Sequence__destroy(arm_interface__action__UfArm_SendGoal_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    arm_interface__action__UfArm_SendGoal_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
arm_interface__action__UfArm_SendGoal_Response__Sequence__are_equal(const arm_interface__action__UfArm_SendGoal_Response__Sequence * lhs, const arm_interface__action__UfArm_SendGoal_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!arm_interface__action__UfArm_SendGoal_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
arm_interface__action__UfArm_SendGoal_Response__Sequence__copy(
  const arm_interface__action__UfArm_SendGoal_Response__Sequence * input,
  arm_interface__action__UfArm_SendGoal_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(arm_interface__action__UfArm_SendGoal_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    arm_interface__action__UfArm_SendGoal_Response * data =
      (arm_interface__action__UfArm_SendGoal_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!arm_interface__action__UfArm_SendGoal_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          arm_interface__action__UfArm_SendGoal_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!arm_interface__action__UfArm_SendGoal_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__functions.h"

bool
arm_interface__action__UfArm_GetResult_Request__init(arm_interface__action__UfArm_GetResult_Request * msg)
{
  if (!msg) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__init(&msg->goal_id)) {
    arm_interface__action__UfArm_GetResult_Request__fini(msg);
    return false;
  }
  return true;
}

void
arm_interface__action__UfArm_GetResult_Request__fini(arm_interface__action__UfArm_GetResult_Request * msg)
{
  if (!msg) {
    return;
  }
  // goal_id
  unique_identifier_msgs__msg__UUID__fini(&msg->goal_id);
}

bool
arm_interface__action__UfArm_GetResult_Request__are_equal(const arm_interface__action__UfArm_GetResult_Request * lhs, const arm_interface__action__UfArm_GetResult_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__are_equal(
      &(lhs->goal_id), &(rhs->goal_id)))
  {
    return false;
  }
  return true;
}

bool
arm_interface__action__UfArm_GetResult_Request__copy(
  const arm_interface__action__UfArm_GetResult_Request * input,
  arm_interface__action__UfArm_GetResult_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__copy(
      &(input->goal_id), &(output->goal_id)))
  {
    return false;
  }
  return true;
}

arm_interface__action__UfArm_GetResult_Request *
arm_interface__action__UfArm_GetResult_Request__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_GetResult_Request * msg = (arm_interface__action__UfArm_GetResult_Request *)allocator.allocate(sizeof(arm_interface__action__UfArm_GetResult_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(arm_interface__action__UfArm_GetResult_Request));
  bool success = arm_interface__action__UfArm_GetResult_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
arm_interface__action__UfArm_GetResult_Request__destroy(arm_interface__action__UfArm_GetResult_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    arm_interface__action__UfArm_GetResult_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
arm_interface__action__UfArm_GetResult_Request__Sequence__init(arm_interface__action__UfArm_GetResult_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_GetResult_Request * data = NULL;

  if (size) {
    data = (arm_interface__action__UfArm_GetResult_Request *)allocator.zero_allocate(size, sizeof(arm_interface__action__UfArm_GetResult_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = arm_interface__action__UfArm_GetResult_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        arm_interface__action__UfArm_GetResult_Request__fini(&data[i - 1]);
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
arm_interface__action__UfArm_GetResult_Request__Sequence__fini(arm_interface__action__UfArm_GetResult_Request__Sequence * array)
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
      arm_interface__action__UfArm_GetResult_Request__fini(&array->data[i]);
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

arm_interface__action__UfArm_GetResult_Request__Sequence *
arm_interface__action__UfArm_GetResult_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_GetResult_Request__Sequence * array = (arm_interface__action__UfArm_GetResult_Request__Sequence *)allocator.allocate(sizeof(arm_interface__action__UfArm_GetResult_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = arm_interface__action__UfArm_GetResult_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
arm_interface__action__UfArm_GetResult_Request__Sequence__destroy(arm_interface__action__UfArm_GetResult_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    arm_interface__action__UfArm_GetResult_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
arm_interface__action__UfArm_GetResult_Request__Sequence__are_equal(const arm_interface__action__UfArm_GetResult_Request__Sequence * lhs, const arm_interface__action__UfArm_GetResult_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!arm_interface__action__UfArm_GetResult_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
arm_interface__action__UfArm_GetResult_Request__Sequence__copy(
  const arm_interface__action__UfArm_GetResult_Request__Sequence * input,
  arm_interface__action__UfArm_GetResult_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(arm_interface__action__UfArm_GetResult_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    arm_interface__action__UfArm_GetResult_Request * data =
      (arm_interface__action__UfArm_GetResult_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!arm_interface__action__UfArm_GetResult_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          arm_interface__action__UfArm_GetResult_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!arm_interface__action__UfArm_GetResult_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `result`
// already included above
// #include "arm_interface/action/detail/uf_arm__functions.h"

bool
arm_interface__action__UfArm_GetResult_Response__init(arm_interface__action__UfArm_GetResult_Response * msg)
{
  if (!msg) {
    return false;
  }
  // status
  // result
  if (!arm_interface__action__UfArm_Result__init(&msg->result)) {
    arm_interface__action__UfArm_GetResult_Response__fini(msg);
    return false;
  }
  return true;
}

void
arm_interface__action__UfArm_GetResult_Response__fini(arm_interface__action__UfArm_GetResult_Response * msg)
{
  if (!msg) {
    return;
  }
  // status
  // result
  arm_interface__action__UfArm_Result__fini(&msg->result);
}

bool
arm_interface__action__UfArm_GetResult_Response__are_equal(const arm_interface__action__UfArm_GetResult_Response * lhs, const arm_interface__action__UfArm_GetResult_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // status
  if (lhs->status != rhs->status) {
    return false;
  }
  // result
  if (!arm_interface__action__UfArm_Result__are_equal(
      &(lhs->result), &(rhs->result)))
  {
    return false;
  }
  return true;
}

bool
arm_interface__action__UfArm_GetResult_Response__copy(
  const arm_interface__action__UfArm_GetResult_Response * input,
  arm_interface__action__UfArm_GetResult_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // status
  output->status = input->status;
  // result
  if (!arm_interface__action__UfArm_Result__copy(
      &(input->result), &(output->result)))
  {
    return false;
  }
  return true;
}

arm_interface__action__UfArm_GetResult_Response *
arm_interface__action__UfArm_GetResult_Response__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_GetResult_Response * msg = (arm_interface__action__UfArm_GetResult_Response *)allocator.allocate(sizeof(arm_interface__action__UfArm_GetResult_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(arm_interface__action__UfArm_GetResult_Response));
  bool success = arm_interface__action__UfArm_GetResult_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
arm_interface__action__UfArm_GetResult_Response__destroy(arm_interface__action__UfArm_GetResult_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    arm_interface__action__UfArm_GetResult_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
arm_interface__action__UfArm_GetResult_Response__Sequence__init(arm_interface__action__UfArm_GetResult_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_GetResult_Response * data = NULL;

  if (size) {
    data = (arm_interface__action__UfArm_GetResult_Response *)allocator.zero_allocate(size, sizeof(arm_interface__action__UfArm_GetResult_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = arm_interface__action__UfArm_GetResult_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        arm_interface__action__UfArm_GetResult_Response__fini(&data[i - 1]);
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
arm_interface__action__UfArm_GetResult_Response__Sequence__fini(arm_interface__action__UfArm_GetResult_Response__Sequence * array)
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
      arm_interface__action__UfArm_GetResult_Response__fini(&array->data[i]);
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

arm_interface__action__UfArm_GetResult_Response__Sequence *
arm_interface__action__UfArm_GetResult_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_GetResult_Response__Sequence * array = (arm_interface__action__UfArm_GetResult_Response__Sequence *)allocator.allocate(sizeof(arm_interface__action__UfArm_GetResult_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = arm_interface__action__UfArm_GetResult_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
arm_interface__action__UfArm_GetResult_Response__Sequence__destroy(arm_interface__action__UfArm_GetResult_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    arm_interface__action__UfArm_GetResult_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
arm_interface__action__UfArm_GetResult_Response__Sequence__are_equal(const arm_interface__action__UfArm_GetResult_Response__Sequence * lhs, const arm_interface__action__UfArm_GetResult_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!arm_interface__action__UfArm_GetResult_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
arm_interface__action__UfArm_GetResult_Response__Sequence__copy(
  const arm_interface__action__UfArm_GetResult_Response__Sequence * input,
  arm_interface__action__UfArm_GetResult_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(arm_interface__action__UfArm_GetResult_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    arm_interface__action__UfArm_GetResult_Response * data =
      (arm_interface__action__UfArm_GetResult_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!arm_interface__action__UfArm_GetResult_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          arm_interface__action__UfArm_GetResult_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!arm_interface__action__UfArm_GetResult_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__functions.h"
// Member `feedback`
// already included above
// #include "arm_interface/action/detail/uf_arm__functions.h"

bool
arm_interface__action__UfArm_FeedbackMessage__init(arm_interface__action__UfArm_FeedbackMessage * msg)
{
  if (!msg) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__init(&msg->goal_id)) {
    arm_interface__action__UfArm_FeedbackMessage__fini(msg);
    return false;
  }
  // feedback
  if (!arm_interface__action__UfArm_Feedback__init(&msg->feedback)) {
    arm_interface__action__UfArm_FeedbackMessage__fini(msg);
    return false;
  }
  return true;
}

void
arm_interface__action__UfArm_FeedbackMessage__fini(arm_interface__action__UfArm_FeedbackMessage * msg)
{
  if (!msg) {
    return;
  }
  // goal_id
  unique_identifier_msgs__msg__UUID__fini(&msg->goal_id);
  // feedback
  arm_interface__action__UfArm_Feedback__fini(&msg->feedback);
}

bool
arm_interface__action__UfArm_FeedbackMessage__are_equal(const arm_interface__action__UfArm_FeedbackMessage * lhs, const arm_interface__action__UfArm_FeedbackMessage * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__are_equal(
      &(lhs->goal_id), &(rhs->goal_id)))
  {
    return false;
  }
  // feedback
  if (!arm_interface__action__UfArm_Feedback__are_equal(
      &(lhs->feedback), &(rhs->feedback)))
  {
    return false;
  }
  return true;
}

bool
arm_interface__action__UfArm_FeedbackMessage__copy(
  const arm_interface__action__UfArm_FeedbackMessage * input,
  arm_interface__action__UfArm_FeedbackMessage * output)
{
  if (!input || !output) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__copy(
      &(input->goal_id), &(output->goal_id)))
  {
    return false;
  }
  // feedback
  if (!arm_interface__action__UfArm_Feedback__copy(
      &(input->feedback), &(output->feedback)))
  {
    return false;
  }
  return true;
}

arm_interface__action__UfArm_FeedbackMessage *
arm_interface__action__UfArm_FeedbackMessage__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_FeedbackMessage * msg = (arm_interface__action__UfArm_FeedbackMessage *)allocator.allocate(sizeof(arm_interface__action__UfArm_FeedbackMessage), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(arm_interface__action__UfArm_FeedbackMessage));
  bool success = arm_interface__action__UfArm_FeedbackMessage__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
arm_interface__action__UfArm_FeedbackMessage__destroy(arm_interface__action__UfArm_FeedbackMessage * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    arm_interface__action__UfArm_FeedbackMessage__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
arm_interface__action__UfArm_FeedbackMessage__Sequence__init(arm_interface__action__UfArm_FeedbackMessage__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_FeedbackMessage * data = NULL;

  if (size) {
    data = (arm_interface__action__UfArm_FeedbackMessage *)allocator.zero_allocate(size, sizeof(arm_interface__action__UfArm_FeedbackMessage), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = arm_interface__action__UfArm_FeedbackMessage__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        arm_interface__action__UfArm_FeedbackMessage__fini(&data[i - 1]);
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
arm_interface__action__UfArm_FeedbackMessage__Sequence__fini(arm_interface__action__UfArm_FeedbackMessage__Sequence * array)
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
      arm_interface__action__UfArm_FeedbackMessage__fini(&array->data[i]);
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

arm_interface__action__UfArm_FeedbackMessage__Sequence *
arm_interface__action__UfArm_FeedbackMessage__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_interface__action__UfArm_FeedbackMessage__Sequence * array = (arm_interface__action__UfArm_FeedbackMessage__Sequence *)allocator.allocate(sizeof(arm_interface__action__UfArm_FeedbackMessage__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = arm_interface__action__UfArm_FeedbackMessage__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
arm_interface__action__UfArm_FeedbackMessage__Sequence__destroy(arm_interface__action__UfArm_FeedbackMessage__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    arm_interface__action__UfArm_FeedbackMessage__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
arm_interface__action__UfArm_FeedbackMessage__Sequence__are_equal(const arm_interface__action__UfArm_FeedbackMessage__Sequence * lhs, const arm_interface__action__UfArm_FeedbackMessage__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!arm_interface__action__UfArm_FeedbackMessage__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
arm_interface__action__UfArm_FeedbackMessage__Sequence__copy(
  const arm_interface__action__UfArm_FeedbackMessage__Sequence * input,
  arm_interface__action__UfArm_FeedbackMessage__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(arm_interface__action__UfArm_FeedbackMessage);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    arm_interface__action__UfArm_FeedbackMessage * data =
      (arm_interface__action__UfArm_FeedbackMessage *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!arm_interface__action__UfArm_FeedbackMessage__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          arm_interface__action__UfArm_FeedbackMessage__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!arm_interface__action__UfArm_FeedbackMessage__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
