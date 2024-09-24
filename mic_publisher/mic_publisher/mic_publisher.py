import rclpy
from rclpy.node import Node
from crackle_interfaces.msg import AudioData
import pyaudio
import numpy as np

class MicPublisherNode(Node):
    def __init__(self):
        super().__init__('mic_publisher_node')

        self.left_publisher = self.create_publisher(AudioData, 'mic_audio/left', 10)
        self.right_publisher = self.create_publisher(AudioData, 'mic_audio/right', 10)

        audio = pyaudio.PyAudio()

        stream_left = audio.open(format=pyaudio.paInt16,
            channels=1,
            rate=44100,
            input=True,
            stream_callback=self.left_mic_callback,
            frames_per_buffer=2**13,
            input_device_index = 0)
        
        stream_right = audio.open(format=pyaudio.paInt16,
            channels=1,
            rate=44100,
            input=True,
            stream_callback=self.right_mic_callback,
            frames_per_buffer=2**13,
            input_device_index = 0)

    def left_mic_callback(self, input_data, frame_count, time_info, flags):
        return self.mic_callback(self.left_publisher, input_data, frame_count, time_info, flags)
    
    def right_mic_callback(self, input_data, frame_count, time_info, flags):
        return self.mic_callback(self.right_publisher, input_data, frame_count, time_info, flags)

    def mic_callback(self, publish_topic, input_data, frame_count, time_info, flags):
        audio_data = AudioData()
        audio_data.data = np.frombuffer(input_data, dtype=np.int32).tolist()
        audio_data.frames = frame_count
        audio_data.start_time = time_info["input_buffer_adc_time"]
        audio_data.end_time = time_info["current_time"]

        publish_topic.publish(audio_data)

        print("publishing")

        return input_data, pyaudio.paContinue

def main(args=None):
    rclpy.init(args=args)
    node = MicPublisherNode()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()
