import math
from _keys import openai_key
from ros_interface import RosInterface
from _llm import GptAPI
#from playsound import playsound
import time
#import speech_recognition as sr
#import pyttsx3
from pathlib import Path
from scipy.spatial.transform import Rotation as R
import rclpy
from rclpy.node import Node
from rclpy.executors import MultiThreadedExecutor
from rclpy.callback_groups import ReentrantCallbackGroup, MutuallyExclusiveCallbackGroup

from std_msgs.msg import String
from geometry_msgs.msg import Vector3Stamped
from geometry_msgs.msg import Pose, PoseStamped 
#from xarm_msgs.srv import PlanExec, PlanPose, PlanSingleStraight

class PlannerNode(Node):
    def __init__(self):
        super().__init__('planner_node')

        # Initialize the ROS interface
        self.ros_interface = RosInterface(self)

        # where the response items will be stored
        self.emotion = "happy"
        self.wordResponse = ""

        self.r = sr.Recognizer() 
        
        self.current_emotion = ""
        self.api = GptAPI(key)
        self.api_conversation = GptAPI(key_audio)

        self.xarm_exec_plan = self.create_client(PlanExec, '/xarm_exec_plan')
        self.xarm_plan_pose = self.create_client(PlanPose, '/xarm_pose_plan')

        self.wait_for_service(self.xarm_exec_plan)
        self.wait_for_service(self.xarm_plan_pose)
    
        self.location_sub = self.create_subscription(
            Vector3Stamped,
            '/sound_direction_pub/vector',
            self.location_callback,
            10
        )

        self.emotion_pub = self.create_publisher(
            String,
            '/face/emotion',
            10
        )

        self.last_known_location = Vector3Stamped()
        self.valid_emotions = [ "happy", "sad", "angry", "bored", "not_impressed", "evil_idea", "flirty", "aww", "wtf"]
        # Timer to run the planner at a fixed rate
        self.timer = self.create_timer(0.1, self.timer_callback)

    def wait_for_service(self, client):
        while not client.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('service not available, waiting again...') 

    def location_callback(self, msg):
        # Process the location data
        self.get_logger().info(f"Received location: {msg.vector.x}, {msg.vector.y}, {msg.vector.z}")
        self.last_known_location = msg
        # You can add your planning logic here based on the location data
        # Example: self.ros_interface.pick_up(msg.vector.x, msg.vector.y, msg.vector.z) 

    def speakText(self, text):
        self.api.speakText(text)

    def talkBack(self, wordsIn):
        self.get_logger().info("generating words to talk back")
        response = self.api_conversation.parseTalkResponse(wordsIn)
        self.wordResponse = response[0]
        self.emotion = response[1]

    def plan_to_pose(self, pose : Pose):
        req = PlanPose.Request()
        req.target = pose
        future = self.xarm_plan_pose.call_async(req)
        print('Sent request')
        rclpy.spin_until_future_complete(self, future)
        print("future: ", future.result())
        return future.result() 

    def execute_plan(self):
        req = PlanExec.Request()
        future = self.xarm_exec_plan.call_async(req)
        future.add_done_callback(self.execute_done_callback)
        # rclpy.spin_until_future_complete(self, future)
        while future.done() == False:
            print(f"{future.done()}")
            rclpy.spin_once(self)
        print('Execution done')
        return future

    def look_at_audio(self):
        # This function will be called to look at the audio source
        self.get_logger().info('Looking at audio source...')
        target_vector = self.last_known_location.vector

        # project the vector to the x-y plane
        target_vector.z = 0


        magnitude  = 0.5 # how far away from the center of the robot

        # set pose to the intersection of the vector and the circle of radius 0.5 on x and y plane
        pose = Pose()
        pose.position.x = target_vector.x * magnitude 
        pose.position.y = target_vector.y * magnitude
        pose.position.z = 0.2

        # rotation around the z axis
        rot_z = math.atan2(target_vector.y, target_vector.x)
        rotation = R.from_euler('z', rot_z, degrees=False)
        quaternion = rotation.as_quat()

        # make it face away from the base
        pose.orientation.x = quaternion[0]
        pose.orientation.y = quaternion[1]
        pose.orientation.z = quaternion[2]
        pose.orientation.w = quaternion[3]
        self.get_logger().info(f"Looking at audio source at pose: {pose.position.x}, {pose.position.y}, {pose.position.z}")
        

        # Call the planning function to move the robot arm to the target pose
        self.plan_to_pose(pose)


        # You can add your planning logic here
        # Example: self.ros_interface.orient_gripper(0, 0, 0)

    def timer_callback(self):
        # This function will be called at a fixed rate
        # You can add your planning logic here
        self.get_logger().info('Planning...')

        text = input("Hello user how can I help you?")

        self.talkBack(text)

        self.speakText(self.wordResponse)
        emotion = self.emotion
        # remove single and double quotes from the emotion
        emotion = emotion.replace("'", "").replace('"', "").replace(".", "")
        emotion = emotion.replace(" ", "_")
        emotion = emotion.lower()
        self.get_logger().info(f"Emotion: {emotion}")
        if emotion in self.valid_emotions:
            self.emotion_pub.publish(String(data=emotion))
        playsound("/home/tanay/crackle_ws/speech.mp3")

def main_planner():
    rclpy.init()
    executor = MultiThreadedExecutor()
    planner = PlannerNode()
    executor.add_node(planner)
    planner.get_logger().info('Planner node started.')
    print('Hi from crackle_planning.')
    api=GptAPI(key)
    print('Works')
    prompt='Pick up the object'
    executor.spin()
    executor.shutdown()
    planner.destroy_node()
    rclpy.shutdown()
    # response=api.get_command(prompt)
    # print(response)


if __name__ == '__main__':
    main()
