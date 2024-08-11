// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from xarm_msgs:srv/GetSetModbusData.idl
// generated code does not contain a copyright notice
#include "xarm_msgs/srv/detail/get_set_modbus_data__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"

// Include directives for member types
// Member `modbus_data`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
xarm_msgs__srv__GetSetModbusData_Request__init(xarm_msgs__srv__GetSetModbusData_Request * msg)
{
  if (!msg) {
    return false;
  }
  // modbus_data
  if (!rosidl_runtime_c__uint8__Sequence__init(&msg->modbus_data, 0)) {
    xarm_msgs__srv__GetSetModbusData_Request__fini(msg);
    return false;
  }
  // modbus_length
  msg->modbus_length = 0;
  // ret_length
  // host_id
  msg->host_id = 9;
  // is_transparent_transmission
  msg->is_transparent_transmission = false;
  // use_503_port
  msg->use_503_port = false;
  return true;
}

void
xarm_msgs__srv__GetSetModbusData_Request__fini(xarm_msgs__srv__GetSetModbusData_Request * msg)
{
  if (!msg) {
    return;
  }
  // modbus_data
  rosidl_runtime_c__uint8__Sequence__fini(&msg->modbus_data);
  // modbus_length
  // ret_length
  // host_id
  // is_transparent_transmission
  // use_503_port
}

bool
xarm_msgs__srv__GetSetModbusData_Request__are_equal(const xarm_msgs__srv__GetSetModbusData_Request * lhs, const xarm_msgs__srv__GetSetModbusData_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // modbus_data
  if (!rosidl_runtime_c__uint8__Sequence__are_equal(
      &(lhs->modbus_data), &(rhs->modbus_data)))
  {
    return false;
  }
  // modbus_length
  if (lhs->modbus_length != rhs->modbus_length) {
    return false;
  }
  // ret_length
  if (lhs->ret_length != rhs->ret_length) {
    return false;
  }
  // host_id
  if (lhs->host_id != rhs->host_id) {
    return false;
  }
  // is_transparent_transmission
  if (lhs->is_transparent_transmission != rhs->is_transparent_transmission) {
    return false;
  }
  // use_503_port
  if (lhs->use_503_port != rhs->use_503_port) {
    return false;
  }
  return true;
}

bool
xarm_msgs__srv__GetSetModbusData_Request__copy(
  const xarm_msgs__srv__GetSetModbusData_Request * input,
  xarm_msgs__srv__GetSetModbusData_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // modbus_data
  if (!rosidl_runtime_c__uint8__Sequence__copy(
      &(input->modbus_data), &(output->modbus_data)))
  {
    return false;
  }
  // modbus_length
  output->modbus_length = input->modbus_length;
  // ret_length
  output->ret_length = input->ret_length;
  // host_id
  output->host_id = input->host_id;
  // is_transparent_transmission
  output->is_transparent_transmission = input->is_transparent_transmission;
  // use_503_port
  output->use_503_port = input->use_503_port;
  return true;
}

xarm_msgs__srv__GetSetModbusData_Request *
xarm_msgs__srv__GetSetModbusData_Request__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xarm_msgs__srv__GetSetModbusData_Request * msg = (xarm_msgs__srv__GetSetModbusData_Request *)allocator.allocate(sizeof(xarm_msgs__srv__GetSetModbusData_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(xarm_msgs__srv__GetSetModbusData_Request));
  bool success = xarm_msgs__srv__GetSetModbusData_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
xarm_msgs__srv__GetSetModbusData_Request__destroy(xarm_msgs__srv__GetSetModbusData_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    xarm_msgs__srv__GetSetModbusData_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
xarm_msgs__srv__GetSetModbusData_Request__Sequence__init(xarm_msgs__srv__GetSetModbusData_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xarm_msgs__srv__GetSetModbusData_Request * data = NULL;

  if (size) {
    data = (xarm_msgs__srv__GetSetModbusData_Request *)allocator.zero_allocate(size, sizeof(xarm_msgs__srv__GetSetModbusData_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = xarm_msgs__srv__GetSetModbusData_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        xarm_msgs__srv__GetSetModbusData_Request__fini(&data[i - 1]);
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
xarm_msgs__srv__GetSetModbusData_Request__Sequence__fini(xarm_msgs__srv__GetSetModbusData_Request__Sequence * array)
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
      xarm_msgs__srv__GetSetModbusData_Request__fini(&array->data[i]);
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

xarm_msgs__srv__GetSetModbusData_Request__Sequence *
xarm_msgs__srv__GetSetModbusData_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xarm_msgs__srv__GetSetModbusData_Request__Sequence * array = (xarm_msgs__srv__GetSetModbusData_Request__Sequence *)allocator.allocate(sizeof(xarm_msgs__srv__GetSetModbusData_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = xarm_msgs__srv__GetSetModbusData_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
xarm_msgs__srv__GetSetModbusData_Request__Sequence__destroy(xarm_msgs__srv__GetSetModbusData_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    xarm_msgs__srv__GetSetModbusData_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
xarm_msgs__srv__GetSetModbusData_Request__Sequence__are_equal(const xarm_msgs__srv__GetSetModbusData_Request__Sequence * lhs, const xarm_msgs__srv__GetSetModbusData_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!xarm_msgs__srv__GetSetModbusData_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
xarm_msgs__srv__GetSetModbusData_Request__Sequence__copy(
  const xarm_msgs__srv__GetSetModbusData_Request__Sequence * input,
  xarm_msgs__srv__GetSetModbusData_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(xarm_msgs__srv__GetSetModbusData_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    xarm_msgs__srv__GetSetModbusData_Request * data =
      (xarm_msgs__srv__GetSetModbusData_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!xarm_msgs__srv__GetSetModbusData_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          xarm_msgs__srv__GetSetModbusData_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!xarm_msgs__srv__GetSetModbusData_Request__copy(
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
// Member `ret_data`
// already included above
// #include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
xarm_msgs__srv__GetSetModbusData_Response__init(xarm_msgs__srv__GetSetModbusData_Response * msg)
{
  if (!msg) {
    return false;
  }
  // ret
  // message
  if (!rosidl_runtime_c__String__init(&msg->message)) {
    xarm_msgs__srv__GetSetModbusData_Response__fini(msg);
    return false;
  }
  // ret_data
  if (!rosidl_runtime_c__uint8__Sequence__init(&msg->ret_data, 0)) {
    xarm_msgs__srv__GetSetModbusData_Response__fini(msg);
    return false;
  }
  return true;
}

void
xarm_msgs__srv__GetSetModbusData_Response__fini(xarm_msgs__srv__GetSetModbusData_Response * msg)
{
  if (!msg) {
    return;
  }
  // ret
  // message
  rosidl_runtime_c__String__fini(&msg->message);
  // ret_data
  rosidl_runtime_c__uint8__Sequence__fini(&msg->ret_data);
}

bool
xarm_msgs__srv__GetSetModbusData_Response__are_equal(const xarm_msgs__srv__GetSetModbusData_Response * lhs, const xarm_msgs__srv__GetSetModbusData_Response * rhs)
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
  // ret_data
  if (!rosidl_runtime_c__uint8__Sequence__are_equal(
      &(lhs->ret_data), &(rhs->ret_data)))
  {
    return false;
  }
  return true;
}

bool
xarm_msgs__srv__GetSetModbusData_Response__copy(
  const xarm_msgs__srv__GetSetModbusData_Response * input,
  xarm_msgs__srv__GetSetModbusData_Response * output)
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
  // ret_data
  if (!rosidl_runtime_c__uint8__Sequence__copy(
      &(input->ret_data), &(output->ret_data)))
  {
    return false;
  }
  return true;
}

xarm_msgs__srv__GetSetModbusData_Response *
xarm_msgs__srv__GetSetModbusData_Response__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xarm_msgs__srv__GetSetModbusData_Response * msg = (xarm_msgs__srv__GetSetModbusData_Response *)allocator.allocate(sizeof(xarm_msgs__srv__GetSetModbusData_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(xarm_msgs__srv__GetSetModbusData_Response));
  bool success = xarm_msgs__srv__GetSetModbusData_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
xarm_msgs__srv__GetSetModbusData_Response__destroy(xarm_msgs__srv__GetSetModbusData_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    xarm_msgs__srv__GetSetModbusData_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
xarm_msgs__srv__GetSetModbusData_Response__Sequence__init(xarm_msgs__srv__GetSetModbusData_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xarm_msgs__srv__GetSetModbusData_Response * data = NULL;

  if (size) {
    data = (xarm_msgs__srv__GetSetModbusData_Response *)allocator.zero_allocate(size, sizeof(xarm_msgs__srv__GetSetModbusData_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = xarm_msgs__srv__GetSetModbusData_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        xarm_msgs__srv__GetSetModbusData_Response__fini(&data[i - 1]);
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
xarm_msgs__srv__GetSetModbusData_Response__Sequence__fini(xarm_msgs__srv__GetSetModbusData_Response__Sequence * array)
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
      xarm_msgs__srv__GetSetModbusData_Response__fini(&array->data[i]);
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

xarm_msgs__srv__GetSetModbusData_Response__Sequence *
xarm_msgs__srv__GetSetModbusData_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xarm_msgs__srv__GetSetModbusData_Response__Sequence * array = (xarm_msgs__srv__GetSetModbusData_Response__Sequence *)allocator.allocate(sizeof(xarm_msgs__srv__GetSetModbusData_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = xarm_msgs__srv__GetSetModbusData_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
xarm_msgs__srv__GetSetModbusData_Response__Sequence__destroy(xarm_msgs__srv__GetSetModbusData_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    xarm_msgs__srv__GetSetModbusData_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
xarm_msgs__srv__GetSetModbusData_Response__Sequence__are_equal(const xarm_msgs__srv__GetSetModbusData_Response__Sequence * lhs, const xarm_msgs__srv__GetSetModbusData_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!xarm_msgs__srv__GetSetModbusData_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
xarm_msgs__srv__GetSetModbusData_Response__Sequence__copy(
  const xarm_msgs__srv__GetSetModbusData_Response__Sequence * input,
  xarm_msgs__srv__GetSetModbusData_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(xarm_msgs__srv__GetSetModbusData_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    xarm_msgs__srv__GetSetModbusData_Response * data =
      (xarm_msgs__srv__GetSetModbusData_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!xarm_msgs__srv__GetSetModbusData_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          xarm_msgs__srv__GetSetModbusData_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!xarm_msgs__srv__GetSetModbusData_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
