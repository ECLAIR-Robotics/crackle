// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from arm_interface:action/UfArm.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "arm_interface/action/detail/uf_arm__rosidl_typesupport_introspection_c.h"
#include "arm_interface/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "arm_interface/action/detail/uf_arm__functions.h"
#include "arm_interface/action/detail/uf_arm__struct.h"


// Include directives for member types
// Member `goal_pose`
#include "geometry_msgs/msg/pose.h"
// Member `goal_pose`
#include "geometry_msgs/msg/detail/pose__rosidl_typesupport_introspection_c.h"
// Member `pose`
#include "rosidl_runtime_c/string_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void arm_interface__action__UfArm_Goal__rosidl_typesupport_introspection_c__UfArm_Goal_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  arm_interface__action__UfArm_Goal__init(message_memory);
}

void arm_interface__action__UfArm_Goal__rosidl_typesupport_introspection_c__UfArm_Goal_fini_function(void * message_memory)
{
  arm_interface__action__UfArm_Goal__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember arm_interface__action__UfArm_Goal__rosidl_typesupport_introspection_c__UfArm_Goal_message_member_array[2] = {
  {
    "goal_pose",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(arm_interface__action__UfArm_Goal, goal_pose),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "pose",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(arm_interface__action__UfArm_Goal, pose),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers arm_interface__action__UfArm_Goal__rosidl_typesupport_introspection_c__UfArm_Goal_message_members = {
  "arm_interface__action",  // message namespace
  "UfArm_Goal",  // message name
  2,  // number of fields
  sizeof(arm_interface__action__UfArm_Goal),
  arm_interface__action__UfArm_Goal__rosidl_typesupport_introspection_c__UfArm_Goal_message_member_array,  // message members
  arm_interface__action__UfArm_Goal__rosidl_typesupport_introspection_c__UfArm_Goal_init_function,  // function to initialize message memory (memory has to be allocated)
  arm_interface__action__UfArm_Goal__rosidl_typesupport_introspection_c__UfArm_Goal_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t arm_interface__action__UfArm_Goal__rosidl_typesupport_introspection_c__UfArm_Goal_message_type_support_handle = {
  0,
  &arm_interface__action__UfArm_Goal__rosidl_typesupport_introspection_c__UfArm_Goal_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_arm_interface
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arm_interface, action, UfArm_Goal)() {
  arm_interface__action__UfArm_Goal__rosidl_typesupport_introspection_c__UfArm_Goal_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, geometry_msgs, msg, Pose)();
  if (!arm_interface__action__UfArm_Goal__rosidl_typesupport_introspection_c__UfArm_Goal_message_type_support_handle.typesupport_identifier) {
    arm_interface__action__UfArm_Goal__rosidl_typesupport_introspection_c__UfArm_Goal_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &arm_interface__action__UfArm_Goal__rosidl_typesupport_introspection_c__UfArm_Goal_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "arm_interface/action/detail/uf_arm__rosidl_typesupport_introspection_c.h"
// already included above
// #include "arm_interface/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "arm_interface/action/detail/uf_arm__functions.h"
// already included above
// #include "arm_interface/action/detail/uf_arm__struct.h"


// Include directives for member types
// Member `final_pose`
// already included above
// #include "geometry_msgs/msg/pose.h"
// Member `final_pose`
// already included above
// #include "geometry_msgs/msg/detail/pose__rosidl_typesupport_introspection_c.h"
// Member `message`
// already included above
// #include "rosidl_runtime_c/string_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void arm_interface__action__UfArm_Result__rosidl_typesupport_introspection_c__UfArm_Result_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  arm_interface__action__UfArm_Result__init(message_memory);
}

void arm_interface__action__UfArm_Result__rosidl_typesupport_introspection_c__UfArm_Result_fini_function(void * message_memory)
{
  arm_interface__action__UfArm_Result__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember arm_interface__action__UfArm_Result__rosidl_typesupport_introspection_c__UfArm_Result_message_member_array[3] = {
  {
    "final_pose",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(arm_interface__action__UfArm_Result, final_pose),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "success",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(arm_interface__action__UfArm_Result, success),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "message",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(arm_interface__action__UfArm_Result, message),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers arm_interface__action__UfArm_Result__rosidl_typesupport_introspection_c__UfArm_Result_message_members = {
  "arm_interface__action",  // message namespace
  "UfArm_Result",  // message name
  3,  // number of fields
  sizeof(arm_interface__action__UfArm_Result),
  arm_interface__action__UfArm_Result__rosidl_typesupport_introspection_c__UfArm_Result_message_member_array,  // message members
  arm_interface__action__UfArm_Result__rosidl_typesupport_introspection_c__UfArm_Result_init_function,  // function to initialize message memory (memory has to be allocated)
  arm_interface__action__UfArm_Result__rosidl_typesupport_introspection_c__UfArm_Result_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t arm_interface__action__UfArm_Result__rosidl_typesupport_introspection_c__UfArm_Result_message_type_support_handle = {
  0,
  &arm_interface__action__UfArm_Result__rosidl_typesupport_introspection_c__UfArm_Result_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_arm_interface
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arm_interface, action, UfArm_Result)() {
  arm_interface__action__UfArm_Result__rosidl_typesupport_introspection_c__UfArm_Result_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, geometry_msgs, msg, Pose)();
  if (!arm_interface__action__UfArm_Result__rosidl_typesupport_introspection_c__UfArm_Result_message_type_support_handle.typesupport_identifier) {
    arm_interface__action__UfArm_Result__rosidl_typesupport_introspection_c__UfArm_Result_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &arm_interface__action__UfArm_Result__rosidl_typesupport_introspection_c__UfArm_Result_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "arm_interface/action/detail/uf_arm__rosidl_typesupport_introspection_c.h"
// already included above
// #include "arm_interface/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "arm_interface/action/detail/uf_arm__functions.h"
// already included above
// #include "arm_interface/action/detail/uf_arm__struct.h"


// Include directives for member types
// Member `current_pose`
// already included above
// #include "geometry_msgs/msg/pose.h"
// Member `current_pose`
// already included above
// #include "geometry_msgs/msg/detail/pose__rosidl_typesupport_introspection_c.h"
// Member `status`
#include "actionlib_msgs/msg/goal_status.h"
// Member `status`
#include "actionlib_msgs/msg/detail/goal_status__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void arm_interface__action__UfArm_Feedback__rosidl_typesupport_introspection_c__UfArm_Feedback_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  arm_interface__action__UfArm_Feedback__init(message_memory);
}

void arm_interface__action__UfArm_Feedback__rosidl_typesupport_introspection_c__UfArm_Feedback_fini_function(void * message_memory)
{
  arm_interface__action__UfArm_Feedback__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember arm_interface__action__UfArm_Feedback__rosidl_typesupport_introspection_c__UfArm_Feedback_message_member_array[2] = {
  {
    "current_pose",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(arm_interface__action__UfArm_Feedback, current_pose),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "status",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(arm_interface__action__UfArm_Feedback, status),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers arm_interface__action__UfArm_Feedback__rosidl_typesupport_introspection_c__UfArm_Feedback_message_members = {
  "arm_interface__action",  // message namespace
  "UfArm_Feedback",  // message name
  2,  // number of fields
  sizeof(arm_interface__action__UfArm_Feedback),
  arm_interface__action__UfArm_Feedback__rosidl_typesupport_introspection_c__UfArm_Feedback_message_member_array,  // message members
  arm_interface__action__UfArm_Feedback__rosidl_typesupport_introspection_c__UfArm_Feedback_init_function,  // function to initialize message memory (memory has to be allocated)
  arm_interface__action__UfArm_Feedback__rosidl_typesupport_introspection_c__UfArm_Feedback_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t arm_interface__action__UfArm_Feedback__rosidl_typesupport_introspection_c__UfArm_Feedback_message_type_support_handle = {
  0,
  &arm_interface__action__UfArm_Feedback__rosidl_typesupport_introspection_c__UfArm_Feedback_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_arm_interface
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arm_interface, action, UfArm_Feedback)() {
  arm_interface__action__UfArm_Feedback__rosidl_typesupport_introspection_c__UfArm_Feedback_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, geometry_msgs, msg, Pose)();
  arm_interface__action__UfArm_Feedback__rosidl_typesupport_introspection_c__UfArm_Feedback_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, actionlib_msgs, msg, GoalStatus)();
  if (!arm_interface__action__UfArm_Feedback__rosidl_typesupport_introspection_c__UfArm_Feedback_message_type_support_handle.typesupport_identifier) {
    arm_interface__action__UfArm_Feedback__rosidl_typesupport_introspection_c__UfArm_Feedback_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &arm_interface__action__UfArm_Feedback__rosidl_typesupport_introspection_c__UfArm_Feedback_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "arm_interface/action/detail/uf_arm__rosidl_typesupport_introspection_c.h"
// already included above
// #include "arm_interface/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "arm_interface/action/detail/uf_arm__functions.h"
// already included above
// #include "arm_interface/action/detail/uf_arm__struct.h"


// Include directives for member types
// Member `goal_id`
#include "unique_identifier_msgs/msg/uuid.h"
// Member `goal_id`
#include "unique_identifier_msgs/msg/detail/uuid__rosidl_typesupport_introspection_c.h"
// Member `goal`
#include "arm_interface/action/uf_arm.h"
// Member `goal`
// already included above
// #include "arm_interface/action/detail/uf_arm__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void arm_interface__action__UfArm_SendGoal_Request__rosidl_typesupport_introspection_c__UfArm_SendGoal_Request_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  arm_interface__action__UfArm_SendGoal_Request__init(message_memory);
}

void arm_interface__action__UfArm_SendGoal_Request__rosidl_typesupport_introspection_c__UfArm_SendGoal_Request_fini_function(void * message_memory)
{
  arm_interface__action__UfArm_SendGoal_Request__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember arm_interface__action__UfArm_SendGoal_Request__rosidl_typesupport_introspection_c__UfArm_SendGoal_Request_message_member_array[2] = {
  {
    "goal_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(arm_interface__action__UfArm_SendGoal_Request, goal_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "goal",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(arm_interface__action__UfArm_SendGoal_Request, goal),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers arm_interface__action__UfArm_SendGoal_Request__rosidl_typesupport_introspection_c__UfArm_SendGoal_Request_message_members = {
  "arm_interface__action",  // message namespace
  "UfArm_SendGoal_Request",  // message name
  2,  // number of fields
  sizeof(arm_interface__action__UfArm_SendGoal_Request),
  arm_interface__action__UfArm_SendGoal_Request__rosidl_typesupport_introspection_c__UfArm_SendGoal_Request_message_member_array,  // message members
  arm_interface__action__UfArm_SendGoal_Request__rosidl_typesupport_introspection_c__UfArm_SendGoal_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  arm_interface__action__UfArm_SendGoal_Request__rosidl_typesupport_introspection_c__UfArm_SendGoal_Request_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t arm_interface__action__UfArm_SendGoal_Request__rosidl_typesupport_introspection_c__UfArm_SendGoal_Request_message_type_support_handle = {
  0,
  &arm_interface__action__UfArm_SendGoal_Request__rosidl_typesupport_introspection_c__UfArm_SendGoal_Request_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_arm_interface
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arm_interface, action, UfArm_SendGoal_Request)() {
  arm_interface__action__UfArm_SendGoal_Request__rosidl_typesupport_introspection_c__UfArm_SendGoal_Request_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, unique_identifier_msgs, msg, UUID)();
  arm_interface__action__UfArm_SendGoal_Request__rosidl_typesupport_introspection_c__UfArm_SendGoal_Request_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arm_interface, action, UfArm_Goal)();
  if (!arm_interface__action__UfArm_SendGoal_Request__rosidl_typesupport_introspection_c__UfArm_SendGoal_Request_message_type_support_handle.typesupport_identifier) {
    arm_interface__action__UfArm_SendGoal_Request__rosidl_typesupport_introspection_c__UfArm_SendGoal_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &arm_interface__action__UfArm_SendGoal_Request__rosidl_typesupport_introspection_c__UfArm_SendGoal_Request_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "arm_interface/action/detail/uf_arm__rosidl_typesupport_introspection_c.h"
// already included above
// #include "arm_interface/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "arm_interface/action/detail/uf_arm__functions.h"
// already included above
// #include "arm_interface/action/detail/uf_arm__struct.h"


// Include directives for member types
// Member `stamp`
#include "builtin_interfaces/msg/time.h"
// Member `stamp`
#include "builtin_interfaces/msg/detail/time__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void arm_interface__action__UfArm_SendGoal_Response__rosidl_typesupport_introspection_c__UfArm_SendGoal_Response_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  arm_interface__action__UfArm_SendGoal_Response__init(message_memory);
}

void arm_interface__action__UfArm_SendGoal_Response__rosidl_typesupport_introspection_c__UfArm_SendGoal_Response_fini_function(void * message_memory)
{
  arm_interface__action__UfArm_SendGoal_Response__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember arm_interface__action__UfArm_SendGoal_Response__rosidl_typesupport_introspection_c__UfArm_SendGoal_Response_message_member_array[2] = {
  {
    "accepted",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(arm_interface__action__UfArm_SendGoal_Response, accepted),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "stamp",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(arm_interface__action__UfArm_SendGoal_Response, stamp),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers arm_interface__action__UfArm_SendGoal_Response__rosidl_typesupport_introspection_c__UfArm_SendGoal_Response_message_members = {
  "arm_interface__action",  // message namespace
  "UfArm_SendGoal_Response",  // message name
  2,  // number of fields
  sizeof(arm_interface__action__UfArm_SendGoal_Response),
  arm_interface__action__UfArm_SendGoal_Response__rosidl_typesupport_introspection_c__UfArm_SendGoal_Response_message_member_array,  // message members
  arm_interface__action__UfArm_SendGoal_Response__rosidl_typesupport_introspection_c__UfArm_SendGoal_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  arm_interface__action__UfArm_SendGoal_Response__rosidl_typesupport_introspection_c__UfArm_SendGoal_Response_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t arm_interface__action__UfArm_SendGoal_Response__rosidl_typesupport_introspection_c__UfArm_SendGoal_Response_message_type_support_handle = {
  0,
  &arm_interface__action__UfArm_SendGoal_Response__rosidl_typesupport_introspection_c__UfArm_SendGoal_Response_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_arm_interface
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arm_interface, action, UfArm_SendGoal_Response)() {
  arm_interface__action__UfArm_SendGoal_Response__rosidl_typesupport_introspection_c__UfArm_SendGoal_Response_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, builtin_interfaces, msg, Time)();
  if (!arm_interface__action__UfArm_SendGoal_Response__rosidl_typesupport_introspection_c__UfArm_SendGoal_Response_message_type_support_handle.typesupport_identifier) {
    arm_interface__action__UfArm_SendGoal_Response__rosidl_typesupport_introspection_c__UfArm_SendGoal_Response_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &arm_interface__action__UfArm_SendGoal_Response__rosidl_typesupport_introspection_c__UfArm_SendGoal_Response_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "arm_interface/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "arm_interface/action/detail/uf_arm__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/service_introspection.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers arm_interface__action__detail__uf_arm__rosidl_typesupport_introspection_c__UfArm_SendGoal_service_members = {
  "arm_interface__action",  // service namespace
  "UfArm_SendGoal",  // service name
  // these two fields are initialized below on the first access
  NULL,  // request message
  // arm_interface__action__detail__uf_arm__rosidl_typesupport_introspection_c__UfArm_SendGoal_Request_message_type_support_handle,
  NULL  // response message
  // arm_interface__action__detail__uf_arm__rosidl_typesupport_introspection_c__UfArm_SendGoal_Response_message_type_support_handle
};

static rosidl_service_type_support_t arm_interface__action__detail__uf_arm__rosidl_typesupport_introspection_c__UfArm_SendGoal_service_type_support_handle = {
  0,
  &arm_interface__action__detail__uf_arm__rosidl_typesupport_introspection_c__UfArm_SendGoal_service_members,
  get_service_typesupport_handle_function,
};

// Forward declaration of request/response type support functions
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arm_interface, action, UfArm_SendGoal_Request)();

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arm_interface, action, UfArm_SendGoal_Response)();

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_arm_interface
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arm_interface, action, UfArm_SendGoal)() {
  if (!arm_interface__action__detail__uf_arm__rosidl_typesupport_introspection_c__UfArm_SendGoal_service_type_support_handle.typesupport_identifier) {
    arm_interface__action__detail__uf_arm__rosidl_typesupport_introspection_c__UfArm_SendGoal_service_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  rosidl_typesupport_introspection_c__ServiceMembers * service_members =
    (rosidl_typesupport_introspection_c__ServiceMembers *)arm_interface__action__detail__uf_arm__rosidl_typesupport_introspection_c__UfArm_SendGoal_service_type_support_handle.data;

  if (!service_members->request_members_) {
    service_members->request_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arm_interface, action, UfArm_SendGoal_Request)()->data;
  }
  if (!service_members->response_members_) {
    service_members->response_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arm_interface, action, UfArm_SendGoal_Response)()->data;
  }

  return &arm_interface__action__detail__uf_arm__rosidl_typesupport_introspection_c__UfArm_SendGoal_service_type_support_handle;
}

// already included above
// #include <stddef.h>
// already included above
// #include "arm_interface/action/detail/uf_arm__rosidl_typesupport_introspection_c.h"
// already included above
// #include "arm_interface/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "arm_interface/action/detail/uf_arm__functions.h"
// already included above
// #include "arm_interface/action/detail/uf_arm__struct.h"


// Include directives for member types
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/uuid.h"
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void arm_interface__action__UfArm_GetResult_Request__rosidl_typesupport_introspection_c__UfArm_GetResult_Request_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  arm_interface__action__UfArm_GetResult_Request__init(message_memory);
}

void arm_interface__action__UfArm_GetResult_Request__rosidl_typesupport_introspection_c__UfArm_GetResult_Request_fini_function(void * message_memory)
{
  arm_interface__action__UfArm_GetResult_Request__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember arm_interface__action__UfArm_GetResult_Request__rosidl_typesupport_introspection_c__UfArm_GetResult_Request_message_member_array[1] = {
  {
    "goal_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(arm_interface__action__UfArm_GetResult_Request, goal_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers arm_interface__action__UfArm_GetResult_Request__rosidl_typesupport_introspection_c__UfArm_GetResult_Request_message_members = {
  "arm_interface__action",  // message namespace
  "UfArm_GetResult_Request",  // message name
  1,  // number of fields
  sizeof(arm_interface__action__UfArm_GetResult_Request),
  arm_interface__action__UfArm_GetResult_Request__rosidl_typesupport_introspection_c__UfArm_GetResult_Request_message_member_array,  // message members
  arm_interface__action__UfArm_GetResult_Request__rosidl_typesupport_introspection_c__UfArm_GetResult_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  arm_interface__action__UfArm_GetResult_Request__rosidl_typesupport_introspection_c__UfArm_GetResult_Request_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t arm_interface__action__UfArm_GetResult_Request__rosidl_typesupport_introspection_c__UfArm_GetResult_Request_message_type_support_handle = {
  0,
  &arm_interface__action__UfArm_GetResult_Request__rosidl_typesupport_introspection_c__UfArm_GetResult_Request_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_arm_interface
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arm_interface, action, UfArm_GetResult_Request)() {
  arm_interface__action__UfArm_GetResult_Request__rosidl_typesupport_introspection_c__UfArm_GetResult_Request_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, unique_identifier_msgs, msg, UUID)();
  if (!arm_interface__action__UfArm_GetResult_Request__rosidl_typesupport_introspection_c__UfArm_GetResult_Request_message_type_support_handle.typesupport_identifier) {
    arm_interface__action__UfArm_GetResult_Request__rosidl_typesupport_introspection_c__UfArm_GetResult_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &arm_interface__action__UfArm_GetResult_Request__rosidl_typesupport_introspection_c__UfArm_GetResult_Request_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "arm_interface/action/detail/uf_arm__rosidl_typesupport_introspection_c.h"
// already included above
// #include "arm_interface/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "arm_interface/action/detail/uf_arm__functions.h"
// already included above
// #include "arm_interface/action/detail/uf_arm__struct.h"


// Include directives for member types
// Member `result`
// already included above
// #include "arm_interface/action/uf_arm.h"
// Member `result`
// already included above
// #include "arm_interface/action/detail/uf_arm__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void arm_interface__action__UfArm_GetResult_Response__rosidl_typesupport_introspection_c__UfArm_GetResult_Response_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  arm_interface__action__UfArm_GetResult_Response__init(message_memory);
}

void arm_interface__action__UfArm_GetResult_Response__rosidl_typesupport_introspection_c__UfArm_GetResult_Response_fini_function(void * message_memory)
{
  arm_interface__action__UfArm_GetResult_Response__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember arm_interface__action__UfArm_GetResult_Response__rosidl_typesupport_introspection_c__UfArm_GetResult_Response_message_member_array[2] = {
  {
    "status",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(arm_interface__action__UfArm_GetResult_Response, status),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "result",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(arm_interface__action__UfArm_GetResult_Response, result),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers arm_interface__action__UfArm_GetResult_Response__rosidl_typesupport_introspection_c__UfArm_GetResult_Response_message_members = {
  "arm_interface__action",  // message namespace
  "UfArm_GetResult_Response",  // message name
  2,  // number of fields
  sizeof(arm_interface__action__UfArm_GetResult_Response),
  arm_interface__action__UfArm_GetResult_Response__rosidl_typesupport_introspection_c__UfArm_GetResult_Response_message_member_array,  // message members
  arm_interface__action__UfArm_GetResult_Response__rosidl_typesupport_introspection_c__UfArm_GetResult_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  arm_interface__action__UfArm_GetResult_Response__rosidl_typesupport_introspection_c__UfArm_GetResult_Response_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t arm_interface__action__UfArm_GetResult_Response__rosidl_typesupport_introspection_c__UfArm_GetResult_Response_message_type_support_handle = {
  0,
  &arm_interface__action__UfArm_GetResult_Response__rosidl_typesupport_introspection_c__UfArm_GetResult_Response_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_arm_interface
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arm_interface, action, UfArm_GetResult_Response)() {
  arm_interface__action__UfArm_GetResult_Response__rosidl_typesupport_introspection_c__UfArm_GetResult_Response_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arm_interface, action, UfArm_Result)();
  if (!arm_interface__action__UfArm_GetResult_Response__rosidl_typesupport_introspection_c__UfArm_GetResult_Response_message_type_support_handle.typesupport_identifier) {
    arm_interface__action__UfArm_GetResult_Response__rosidl_typesupport_introspection_c__UfArm_GetResult_Response_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &arm_interface__action__UfArm_GetResult_Response__rosidl_typesupport_introspection_c__UfArm_GetResult_Response_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "arm_interface/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "arm_interface/action/detail/uf_arm__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/service_introspection.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers arm_interface__action__detail__uf_arm__rosidl_typesupport_introspection_c__UfArm_GetResult_service_members = {
  "arm_interface__action",  // service namespace
  "UfArm_GetResult",  // service name
  // these two fields are initialized below on the first access
  NULL,  // request message
  // arm_interface__action__detail__uf_arm__rosidl_typesupport_introspection_c__UfArm_GetResult_Request_message_type_support_handle,
  NULL  // response message
  // arm_interface__action__detail__uf_arm__rosidl_typesupport_introspection_c__UfArm_GetResult_Response_message_type_support_handle
};

static rosidl_service_type_support_t arm_interface__action__detail__uf_arm__rosidl_typesupport_introspection_c__UfArm_GetResult_service_type_support_handle = {
  0,
  &arm_interface__action__detail__uf_arm__rosidl_typesupport_introspection_c__UfArm_GetResult_service_members,
  get_service_typesupport_handle_function,
};

// Forward declaration of request/response type support functions
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arm_interface, action, UfArm_GetResult_Request)();

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arm_interface, action, UfArm_GetResult_Response)();

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_arm_interface
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arm_interface, action, UfArm_GetResult)() {
  if (!arm_interface__action__detail__uf_arm__rosidl_typesupport_introspection_c__UfArm_GetResult_service_type_support_handle.typesupport_identifier) {
    arm_interface__action__detail__uf_arm__rosidl_typesupport_introspection_c__UfArm_GetResult_service_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  rosidl_typesupport_introspection_c__ServiceMembers * service_members =
    (rosidl_typesupport_introspection_c__ServiceMembers *)arm_interface__action__detail__uf_arm__rosidl_typesupport_introspection_c__UfArm_GetResult_service_type_support_handle.data;

  if (!service_members->request_members_) {
    service_members->request_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arm_interface, action, UfArm_GetResult_Request)()->data;
  }
  if (!service_members->response_members_) {
    service_members->response_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arm_interface, action, UfArm_GetResult_Response)()->data;
  }

  return &arm_interface__action__detail__uf_arm__rosidl_typesupport_introspection_c__UfArm_GetResult_service_type_support_handle;
}

// already included above
// #include <stddef.h>
// already included above
// #include "arm_interface/action/detail/uf_arm__rosidl_typesupport_introspection_c.h"
// already included above
// #include "arm_interface/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "arm_interface/action/detail/uf_arm__functions.h"
// already included above
// #include "arm_interface/action/detail/uf_arm__struct.h"


// Include directives for member types
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/uuid.h"
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__rosidl_typesupport_introspection_c.h"
// Member `feedback`
// already included above
// #include "arm_interface/action/uf_arm.h"
// Member `feedback`
// already included above
// #include "arm_interface/action/detail/uf_arm__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void arm_interface__action__UfArm_FeedbackMessage__rosidl_typesupport_introspection_c__UfArm_FeedbackMessage_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  arm_interface__action__UfArm_FeedbackMessage__init(message_memory);
}

void arm_interface__action__UfArm_FeedbackMessage__rosidl_typesupport_introspection_c__UfArm_FeedbackMessage_fini_function(void * message_memory)
{
  arm_interface__action__UfArm_FeedbackMessage__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember arm_interface__action__UfArm_FeedbackMessage__rosidl_typesupport_introspection_c__UfArm_FeedbackMessage_message_member_array[2] = {
  {
    "goal_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(arm_interface__action__UfArm_FeedbackMessage, goal_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "feedback",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(arm_interface__action__UfArm_FeedbackMessage, feedback),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers arm_interface__action__UfArm_FeedbackMessage__rosidl_typesupport_introspection_c__UfArm_FeedbackMessage_message_members = {
  "arm_interface__action",  // message namespace
  "UfArm_FeedbackMessage",  // message name
  2,  // number of fields
  sizeof(arm_interface__action__UfArm_FeedbackMessage),
  arm_interface__action__UfArm_FeedbackMessage__rosidl_typesupport_introspection_c__UfArm_FeedbackMessage_message_member_array,  // message members
  arm_interface__action__UfArm_FeedbackMessage__rosidl_typesupport_introspection_c__UfArm_FeedbackMessage_init_function,  // function to initialize message memory (memory has to be allocated)
  arm_interface__action__UfArm_FeedbackMessage__rosidl_typesupport_introspection_c__UfArm_FeedbackMessage_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t arm_interface__action__UfArm_FeedbackMessage__rosidl_typesupport_introspection_c__UfArm_FeedbackMessage_message_type_support_handle = {
  0,
  &arm_interface__action__UfArm_FeedbackMessage__rosidl_typesupport_introspection_c__UfArm_FeedbackMessage_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_arm_interface
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arm_interface, action, UfArm_FeedbackMessage)() {
  arm_interface__action__UfArm_FeedbackMessage__rosidl_typesupport_introspection_c__UfArm_FeedbackMessage_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, unique_identifier_msgs, msg, UUID)();
  arm_interface__action__UfArm_FeedbackMessage__rosidl_typesupport_introspection_c__UfArm_FeedbackMessage_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, arm_interface, action, UfArm_Feedback)();
  if (!arm_interface__action__UfArm_FeedbackMessage__rosidl_typesupport_introspection_c__UfArm_FeedbackMessage_message_type_support_handle.typesupport_identifier) {
    arm_interface__action__UfArm_FeedbackMessage__rosidl_typesupport_introspection_c__UfArm_FeedbackMessage_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &arm_interface__action__UfArm_FeedbackMessage__rosidl_typesupport_introspection_c__UfArm_FeedbackMessage_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
