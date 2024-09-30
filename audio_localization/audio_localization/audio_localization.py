import rclpy
from rclpy.node import Node
from crackle_interfaces.msg import AudioData
import pyaudio
import numpy as np
from std_msgs.msg import String
import time

class audioLocalizationNode(Node):
    
    # Physics constants
    MIKE_DISTANCE = 0
    SPEED_OF_SOUND = 343000000

    # Mike constants
    SAMPLE_RATE = 44100  # Sample rate in Hz
    CHUNK = 1024  # Number of frames per buffer
    THRESHOLD = 0.01  # Threshold for detecting sound
    LEFT_MIKE_INDEX = 0
    RIGHT_MIKE_INDEX = 1
    WAIT_TIME = 0 #how often sound is sampled

    # time after which timeout has probably occured: after this point reset the test
    MAX_WAIT_TIME = 1000
        
    def __init__(self):
        super().__init__('audioLocalizationNode')
        self.publisher_ = self.create_publisher(String, 'topic', 10)
        timer_period = 0.5  # seconds
        self.timer = self.create_timer(timer_period, self.timer_callback)
        self.i = 0
        self.subscription = self.create_subscription(AudioData, 'mic_audio/right', self.getRightMikeData, 10)
        self.subscription = self.create_subscription(AudioData, 'mic_audio/left', self.getLeftMikeData, 10)
        #init the mikes as empty
        self.leftMike = microphoneData([], 0, 0)
        self.rightMike = microphoneData([], 0, 0)
        
    def getRightMikeData(self, msg):
        self.rightMike.setStartTime(msg.start_time)
        self.rightMike.setEndTime(msg.end_time)
        self.rightMike.setNewData(msg.data)
        main()
        
    def getLeftMikeData(self, msg):
        self.leftMike.setStartTime(msg.start_time)
        self.leftMike.setEndTime(msg.end_time)
        self.leftMike.setNewData(msg.data)
        
    #positive = right mike first, negative = left mike first
    def getDeltaTime(self, leftTime, rightTime):
        return rightTime - leftTime
    
    def getAngle(self, deltaTime):
        delta = (deltaTime * self.SPEED_OF_SOUND) / self.MIKE_DISTANCE
        angle1 = np.arctan2(delta, 1)
        angle2 = np.arctan2(-delta, 1)
        return np.degrees(angle1), np.degrees(angle2)
        
    def main(self):
        #if both are at threshold sounds on average over time frame
        if(self.rightMike.getAverageSound() > self.THRESHOLD and self.leftMike.getAverageSound() > self.THRESHOLD):
            #TODO publish the following
            #return the angle between based on time difference between highestSound of each side in time frame
            self.getAngle(self.getDeltaTime(self.leftMike.getTimeOfHighestSound(), self.rightMike.getTimeOfHighestSound()))
        time.sleep(self.WAIT_TIME)
        
    
        
class microphoneData():

    def __init__(self, audioData, t0, t1):
        self.audio = audioData # past second
        self.significantSounds = [
            {
                "magnitude": 78,
                "time_step": 5
            }
        ] # past second
        self.startTime = t0 # of audio data
        self.endTime = t1 # o

    def process_audio(audio_msg):
        # will process and add to audio and significantSounds
        return
        
    # uses some method to find a fair 'average' sound
    # TODO: find best / least sketchy way to get an avg sound (actual average or something else) 
    def getAverageSound(self):
        avg = 0
        for i in range(len(self.audio)):
            avg += self.audio[i]
        avg /= len(self.audio)
        
    #TODO: finish
    def getTimeOfHighestSound(self):
        #
        
        
    def getStartTime(self):
        return self.startTime

    def getEndTime(self):
        return self.endTime
    
    def setStartTime(self, t0):
        self.startTime = t0
        
    def setEndTime(self, t1):
        self.endTime = t1
        
    def setNewData(self, data):
        self.audio = data
        
    
def main(args=None):
    rclpy.init(args=args)
    node = audioLocalizationNode()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()
