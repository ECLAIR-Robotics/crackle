// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from arm_interface:action/UfArm.idl
// generated code does not contain a copyright notice

#ifndef ARM_INTERFACE__ACTION__DETAIL__UF_ARM__BUILDER_HPP_
#define ARM_INTERFACE__ACTION__DETAIL__UF_ARM__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "arm_interface/action/detail/uf_arm__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace arm_interface
{

namespace action
{

namespace builder
{

class Init_UfArm_Goal_pose
{
public:
  explicit Init_UfArm_Goal_pose(::arm_interface::action::UfArm_Goal & msg)
  : msg_(msg)
  {}
  ::arm_interface::action::UfArm_Goal pose(::arm_interface::action::UfArm_Goal::_pose_type arg)
  {
    msg_.pose = std::move(arg);
    return std::move(msg_);
  }

private:
  ::arm_interface::action::UfArm_Goal msg_;
};

class Init_UfArm_Goal_goal_pose
{
public:
  Init_UfArm_Goal_goal_pose()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_UfArm_Goal_pose goal_pose(::arm_interface::action::UfArm_Goal::_goal_pose_type arg)
  {
    msg_.goal_pose = std::move(arg);
    return Init_UfArm_Goal_pose(msg_);
  }

private:
  ::arm_interface::action::UfArm_Goal msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::arm_interface::action::UfArm_Goal>()
{
  return arm_interface::action::builder::Init_UfArm_Goal_goal_pose();
}

}  // namespace arm_interface


namespace arm_interface
{

namespace action
{

namespace builder
{

class Init_UfArm_Result_message
{
public:
  explicit Init_UfArm_Result_message(::arm_interface::action::UfArm_Result & msg)
  : msg_(msg)
  {}
  ::arm_interface::action::UfArm_Result message(::arm_interface::action::UfArm_Result::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::arm_interface::action::UfArm_Result msg_;
};

class Init_UfArm_Result_success
{
public:
  explicit Init_UfArm_Result_success(::arm_interface::action::UfArm_Result & msg)
  : msg_(msg)
  {}
  Init_UfArm_Result_message success(::arm_interface::action::UfArm_Result::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_UfArm_Result_message(msg_);
  }

private:
  ::arm_interface::action::UfArm_Result msg_;
};

class Init_UfArm_Result_final_pose
{
public:
  Init_UfArm_Result_final_pose()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_UfArm_Result_success final_pose(::arm_interface::action::UfArm_Result::_final_pose_type arg)
  {
    msg_.final_pose = std::move(arg);
    return Init_UfArm_Result_success(msg_);
  }

private:
  ::arm_interface::action::UfArm_Result msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::arm_interface::action::UfArm_Result>()
{
  return arm_interface::action::builder::Init_UfArm_Result_final_pose();
}

}  // namespace arm_interface


namespace arm_interface
{

namespace action
{

namespace builder
{

class Init_UfArm_Feedback_status
{
public:
  explicit Init_UfArm_Feedback_status(::arm_interface::action::UfArm_Feedback & msg)
  : msg_(msg)
  {}
  ::arm_interface::action::UfArm_Feedback status(::arm_interface::action::UfArm_Feedback::_status_type arg)
  {
    msg_.status = std::move(arg);
    return std::move(msg_);
  }

private:
  ::arm_interface::action::UfArm_Feedback msg_;
};

class Init_UfArm_Feedback_current_pose
{
public:
  Init_UfArm_Feedback_current_pose()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_UfArm_Feedback_status current_pose(::arm_interface::action::UfArm_Feedback::_current_pose_type arg)
  {
    msg_.current_pose = std::move(arg);
    return Init_UfArm_Feedback_status(msg_);
  }

private:
  ::arm_interface::action::UfArm_Feedback msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::arm_interface::action::UfArm_Feedback>()
{
  return arm_interface::action::builder::Init_UfArm_Feedback_current_pose();
}

}  // namespace arm_interface


namespace arm_interface
{

namespace action
{

namespace builder
{

class Init_UfArm_SendGoal_Request_goal
{
public:
  explicit Init_UfArm_SendGoal_Request_goal(::arm_interface::action::UfArm_SendGoal_Request & msg)
  : msg_(msg)
  {}
  ::arm_interface::action::UfArm_SendGoal_Request goal(::arm_interface::action::UfArm_SendGoal_Request::_goal_type arg)
  {
    msg_.goal = std::move(arg);
    return std::move(msg_);
  }

private:
  ::arm_interface::action::UfArm_SendGoal_Request msg_;
};

class Init_UfArm_SendGoal_Request_goal_id
{
public:
  Init_UfArm_SendGoal_Request_goal_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_UfArm_SendGoal_Request_goal goal_id(::arm_interface::action::UfArm_SendGoal_Request::_goal_id_type arg)
  {
    msg_.goal_id = std::move(arg);
    return Init_UfArm_SendGoal_Request_goal(msg_);
  }

private:
  ::arm_interface::action::UfArm_SendGoal_Request msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::arm_interface::action::UfArm_SendGoal_Request>()
{
  return arm_interface::action::builder::Init_UfArm_SendGoal_Request_goal_id();
}

}  // namespace arm_interface


namespace arm_interface
{

namespace action
{

namespace builder
{

class Init_UfArm_SendGoal_Response_stamp
{
public:
  explicit Init_UfArm_SendGoal_Response_stamp(::arm_interface::action::UfArm_SendGoal_Response & msg)
  : msg_(msg)
  {}
  ::arm_interface::action::UfArm_SendGoal_Response stamp(::arm_interface::action::UfArm_SendGoal_Response::_stamp_type arg)
  {
    msg_.stamp = std::move(arg);
    return std::move(msg_);
  }

private:
  ::arm_interface::action::UfArm_SendGoal_Response msg_;
};

class Init_UfArm_SendGoal_Response_accepted
{
public:
  Init_UfArm_SendGoal_Response_accepted()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_UfArm_SendGoal_Response_stamp accepted(::arm_interface::action::UfArm_SendGoal_Response::_accepted_type arg)
  {
    msg_.accepted = std::move(arg);
    return Init_UfArm_SendGoal_Response_stamp(msg_);
  }

private:
  ::arm_interface::action::UfArm_SendGoal_Response msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::arm_interface::action::UfArm_SendGoal_Response>()
{
  return arm_interface::action::builder::Init_UfArm_SendGoal_Response_accepted();
}

}  // namespace arm_interface


namespace arm_interface
{

namespace action
{

namespace builder
{

class Init_UfArm_GetResult_Request_goal_id
{
public:
  Init_UfArm_GetResult_Request_goal_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::arm_interface::action::UfArm_GetResult_Request goal_id(::arm_interface::action::UfArm_GetResult_Request::_goal_id_type arg)
  {
    msg_.goal_id = std::move(arg);
    return std::move(msg_);
  }

private:
  ::arm_interface::action::UfArm_GetResult_Request msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::arm_interface::action::UfArm_GetResult_Request>()
{
  return arm_interface::action::builder::Init_UfArm_GetResult_Request_goal_id();
}

}  // namespace arm_interface


namespace arm_interface
{

namespace action
{

namespace builder
{

class Init_UfArm_GetResult_Response_result
{
public:
  explicit Init_UfArm_GetResult_Response_result(::arm_interface::action::UfArm_GetResult_Response & msg)
  : msg_(msg)
  {}
  ::arm_interface::action::UfArm_GetResult_Response result(::arm_interface::action::UfArm_GetResult_Response::_result_type arg)
  {
    msg_.result = std::move(arg);
    return std::move(msg_);
  }

private:
  ::arm_interface::action::UfArm_GetResult_Response msg_;
};

class Init_UfArm_GetResult_Response_status
{
public:
  Init_UfArm_GetResult_Response_status()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_UfArm_GetResult_Response_result status(::arm_interface::action::UfArm_GetResult_Response::_status_type arg)
  {
    msg_.status = std::move(arg);
    return Init_UfArm_GetResult_Response_result(msg_);
  }

private:
  ::arm_interface::action::UfArm_GetResult_Response msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::arm_interface::action::UfArm_GetResult_Response>()
{
  return arm_interface::action::builder::Init_UfArm_GetResult_Response_status();
}

}  // namespace arm_interface


namespace arm_interface
{

namespace action
{

namespace builder
{

class Init_UfArm_FeedbackMessage_feedback
{
public:
  explicit Init_UfArm_FeedbackMessage_feedback(::arm_interface::action::UfArm_FeedbackMessage & msg)
  : msg_(msg)
  {}
  ::arm_interface::action::UfArm_FeedbackMessage feedback(::arm_interface::action::UfArm_FeedbackMessage::_feedback_type arg)
  {
    msg_.feedback = std::move(arg);
    return std::move(msg_);
  }

private:
  ::arm_interface::action::UfArm_FeedbackMessage msg_;
};

class Init_UfArm_FeedbackMessage_goal_id
{
public:
  Init_UfArm_FeedbackMessage_goal_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_UfArm_FeedbackMessage_feedback goal_id(::arm_interface::action::UfArm_FeedbackMessage::_goal_id_type arg)
  {
    msg_.goal_id = std::move(arg);
    return Init_UfArm_FeedbackMessage_feedback(msg_);
  }

private:
  ::arm_interface::action::UfArm_FeedbackMessage msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::arm_interface::action::UfArm_FeedbackMessage>()
{
  return arm_interface::action::builder::Init_UfArm_FeedbackMessage_goal_id();
}

}  // namespace arm_interface

#endif  // ARM_INTERFACE__ACTION__DETAIL__UF_ARM__BUILDER_HPP_
