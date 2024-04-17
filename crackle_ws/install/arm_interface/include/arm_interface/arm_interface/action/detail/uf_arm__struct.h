// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from arm_interface:action/UfArm.idl
// generated code does not contain a copyright notice

#ifndef ARM_INTERFACE__ACTION__DETAIL__UF_ARM__STRUCT_H_
#define ARM_INTERFACE__ACTION__DETAIL__UF_ARM__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'goal_pose'
#include "geometry_msgs/msg/detail/pose__struct.h"
// Member 'pose'
#include "rosidl_runtime_c/string.h"

/// Struct defined in action/UfArm in the package arm_interface.
typedef struct arm_interface__action__UfArm_Goal
{
  geometry_msgs__msg__Pose goal_pose;
  rosidl_runtime_c__String pose;
} arm_interface__action__UfArm_Goal;

// Struct for a sequence of arm_interface__action__UfArm_Goal.
typedef struct arm_interface__action__UfArm_Goal__Sequence
{
  arm_interface__action__UfArm_Goal * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} arm_interface__action__UfArm_Goal__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'final_pose'
// already included above
// #include "geometry_msgs/msg/detail/pose__struct.h"
// Member 'message'
// already included above
// #include "rosidl_runtime_c/string.h"

/// Struct defined in action/UfArm in the package arm_interface.
typedef struct arm_interface__action__UfArm_Result
{
  geometry_msgs__msg__Pose final_pose;
  bool success;
  rosidl_runtime_c__String message;
} arm_interface__action__UfArm_Result;

// Struct for a sequence of arm_interface__action__UfArm_Result.
typedef struct arm_interface__action__UfArm_Result__Sequence
{
  arm_interface__action__UfArm_Result * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} arm_interface__action__UfArm_Result__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'current_pose'
// already included above
// #include "geometry_msgs/msg/detail/pose__struct.h"
// Member 'status'
#include "actionlib_msgs/msg/detail/goal_status__struct.h"

/// Struct defined in action/UfArm in the package arm_interface.
typedef struct arm_interface__action__UfArm_Feedback
{
  geometry_msgs__msg__Pose current_pose;
  actionlib_msgs__msg__GoalStatus status;
} arm_interface__action__UfArm_Feedback;

// Struct for a sequence of arm_interface__action__UfArm_Feedback.
typedef struct arm_interface__action__UfArm_Feedback__Sequence
{
  arm_interface__action__UfArm_Feedback * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} arm_interface__action__UfArm_Feedback__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'goal_id'
#include "unique_identifier_msgs/msg/detail/uuid__struct.h"
// Member 'goal'
#include "arm_interface/action/detail/uf_arm__struct.h"

/// Struct defined in action/UfArm in the package arm_interface.
typedef struct arm_interface__action__UfArm_SendGoal_Request
{
  unique_identifier_msgs__msg__UUID goal_id;
  arm_interface__action__UfArm_Goal goal;
} arm_interface__action__UfArm_SendGoal_Request;

// Struct for a sequence of arm_interface__action__UfArm_SendGoal_Request.
typedef struct arm_interface__action__UfArm_SendGoal_Request__Sequence
{
  arm_interface__action__UfArm_SendGoal_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} arm_interface__action__UfArm_SendGoal_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'stamp'
#include "builtin_interfaces/msg/detail/time__struct.h"

/// Struct defined in action/UfArm in the package arm_interface.
typedef struct arm_interface__action__UfArm_SendGoal_Response
{
  bool accepted;
  builtin_interfaces__msg__Time stamp;
} arm_interface__action__UfArm_SendGoal_Response;

// Struct for a sequence of arm_interface__action__UfArm_SendGoal_Response.
typedef struct arm_interface__action__UfArm_SendGoal_Response__Sequence
{
  arm_interface__action__UfArm_SendGoal_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} arm_interface__action__UfArm_SendGoal_Response__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'goal_id'
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__struct.h"

/// Struct defined in action/UfArm in the package arm_interface.
typedef struct arm_interface__action__UfArm_GetResult_Request
{
  unique_identifier_msgs__msg__UUID goal_id;
} arm_interface__action__UfArm_GetResult_Request;

// Struct for a sequence of arm_interface__action__UfArm_GetResult_Request.
typedef struct arm_interface__action__UfArm_GetResult_Request__Sequence
{
  arm_interface__action__UfArm_GetResult_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} arm_interface__action__UfArm_GetResult_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'result'
// already included above
// #include "arm_interface/action/detail/uf_arm__struct.h"

/// Struct defined in action/UfArm in the package arm_interface.
typedef struct arm_interface__action__UfArm_GetResult_Response
{
  int8_t status;
  arm_interface__action__UfArm_Result result;
} arm_interface__action__UfArm_GetResult_Response;

// Struct for a sequence of arm_interface__action__UfArm_GetResult_Response.
typedef struct arm_interface__action__UfArm_GetResult_Response__Sequence
{
  arm_interface__action__UfArm_GetResult_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} arm_interface__action__UfArm_GetResult_Response__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'goal_id'
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__struct.h"
// Member 'feedback'
// already included above
// #include "arm_interface/action/detail/uf_arm__struct.h"

/// Struct defined in action/UfArm in the package arm_interface.
typedef struct arm_interface__action__UfArm_FeedbackMessage
{
  unique_identifier_msgs__msg__UUID goal_id;
  arm_interface__action__UfArm_Feedback feedback;
} arm_interface__action__UfArm_FeedbackMessage;

// Struct for a sequence of arm_interface__action__UfArm_FeedbackMessage.
typedef struct arm_interface__action__UfArm_FeedbackMessage__Sequence
{
  arm_interface__action__UfArm_FeedbackMessage * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} arm_interface__action__UfArm_FeedbackMessage__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ARM_INTERFACE__ACTION__DETAIL__UF_ARM__STRUCT_H_
