"""
This implements the Finite state Machine (FSA) for Crackle that
decides what actions to take based on the current state and inputs.

"""

from fileinput import filename
import time
from enum import Enum
import threading
import asyncio
import random
import sounddevice as sd
import numpy as np
import openwakeword
from openwakeword.model import Model
import multiprocessing
from typing import List, Dict
import pyaudio
import os
from crackle_planning._api import PlannerAPI
import wave
from openai import OpenAI
from crackle_planning._llm import GptAPI
from playsound import playsound
from crackle_planning._keys import openai_key

openwakeword.utils.download_models()

FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 16000
CHUNK = 1280

ROS_ENABLED = os.getenv("ROS_ENABLED", "false").lower() == "true"
print(f"ROS_ENABLED: {ROS_ENABLED}")

class CrackleState(Enum):
    IDLE = "idle"
    TASK = "task"
    LISTENING = "listening"
    RESETTING = "resetting"
    FAILURE = "failure"

class CrackleFSM:
    def __init__(self):
        self._state = CrackleState.IDLE
        self._tasks: asyncio.Queue = asyncio.Queue()
        self._event_loop = asyncio.get_event_loop()
        self._stop_event = asyncio.Event()
        self._model = Model()
        self.planner_api = PlannerAPI(use_ros=ROS_ENABLED)
        
        self._running_threads = []
        self.handlers: Dict[CrackleState, List[callable]] = {
            CrackleState.IDLE: [CrackleState.TASK],
            CrackleState.TASK: [CrackleState.IDLE, CrackleState.FAILURE],
            CrackleState.RESETTING: [CrackleState.IDLE],
            CrackleState.FAILURE: [CrackleState.RESETTING],
        }
        self.INFERENCE_FRAMEWORK = 'tflite'
        self._audio = pyaudio.PyAudio()
        self._mic_stream = self._audio.open(format=FORMAT, channels=CHANNELS, rate=RATE, frames_per_buffer=CHUNK, input=True)
        self._owwModel = Model(inference_framework=self.INFERENCE_FRAMEWORK)
        self._n_models = len(self._owwModel.models.keys())

        self.gpt_api = GptAPI(key=openai_key)  # Replace with your actual key
 
        print(f"Initialized FSM in state: {self._state}")

    def _add_task(self, coro: any):
        task = asyncio.create_task(coro)
        self._tasks.put(task)
        return task

    def interrupt_current_tasks(self):
        for t in self._running_threads:
            t.cancel()
        self._running_threads = []

    def look_at_sound_direction(self):
        if (ROS_ENABLED):
            # use ros interface to get latest sound direction
            pass
        else:
            print("ROS not enabled, cannot look at sound direction")
            pass

    async def handle_idle(self):
        # Create two threads: one for listening to commands and one for scanning
        # Listening thread interrupts the scanning thread and then processes the command
        print("Entering IDLE state: Scanning and Listening for commands...")
        self._mic_stream.start_stream()
        async def scanning_thread(name: str, stop_event: asyncio.Event):
            while self._state == CrackleState.IDLE and not stop_event.is_set():
                print("Scanning for commands...")
                # run command
                await asyncio.sleep(5)  # Scanning delay

        async def listening_thread(name: str, stop_event: asyncio.Event):
            while self._state == CrackleState.IDLE:
                audio = np.frombuffer(self._mic_stream.read(CHUNK), dtype=np.int16)
                prediction = self._owwModel.predict(audio)
                if prediction["hey_jarvis"] > 0.8:
                    self._state = CrackleState.LISTENING
                    wake_wall_time = time.time()  # seconds float

                    # Trigger immediately; ROS will wait up to ~0.5s for a fresh sample at/after this time
                    self.planner_api.look_at_sound_direction(wake_wall_time)

                    # Optionally flush model state
                    for _ in range(15):
                        audio = np.frombuffer(self._mic_stream.read(CHUNK), dtype=np.int16)
                        _ = self._owwModel.predict(audio)

                    stop_event.set()
                    break

        stop_event_scanning = asyncio.Event()
        scan_task = asyncio.create_task(scanning_thread("Scanner", stop_event_scanning))
        listen_task = asyncio.create_task(listening_thread("Listener", stop_event_scanning))
        self._running_threads.extend([scan_task, listen_task])
        await asyncio.gather(scan_task, listen_task)

    async def handle_task(self):
        # ...
        await asyncio.sleep(2)  # Simulate task execution time
        print("Entering TASK state: Executing task...")
        pass

    async def handle_resetting(self):
        # ...
        print("Entering RESETTING state: Resetting system...")
        await asyncio.sleep(2)  # Simulate resetting time
        pass

    async def handle_failure(self):
        # ...
        print("Entering FAILURE state: Handling failure...")
        await asyncio.sleep(2)  # Simulate failure handling time
        pass

    def record_output(self, seconds, samplerate=16000, chunk=1024):
        """records audio from the microphone for a set number of seconds"""
        frames = int(seconds * samplerate / chunk)
        buf = []

        for _ in range(frames):
            data = self._mic_stream.read(chunk, exception_on_overflow=False)
            buf.append(np.frombuffer(data, dtype=np.int16))

        return np.concatenate(buf)
    
    def save_as_wav(self, samples: np.ndarray, sample_rate=16000, filename="capture.wav"):
        """saves an audio sample as a wav file"""
        with wave.open(filename, "wb") as wf:
            wf.setnchannels(1)
            wf.setsampwidth(2)  # int16
            wf.setframerate(sample_rate)
            wf.writeframes(samples.tobytes())

        print(f"Saved WAV: {filename}")



    async def main_loop(self):
        # Consume the latest asyncio task in the queue

        while True:
            if self._state == CrackleState.IDLE:
                # Create a task to run handle_idle without blocking
                print("State is IDLE")
                t = asyncio.create_task(self.handle_idle())
                await t
            elif self._state == CrackleState.LISTENING:
                print("Crackle is Listening now...")
                # start listening for a command (call function)
                samples = self.record_output(5)          # record 5 seconds
                print("Recording complete.")
                self.save_as_wav(samples, 16000, "out.wav")  # save it
                transcribed_words = self.gpt_api.speech_to_text("out.wav")
                print(f"Transcribed words: {transcribed_words}")


                # Convert speech to text
                text = self.gpt_api.speech_to_text("out.wav")
                responseWords, emotion = self.gpt_api.parseTalkResponse(text)
                print("Crackle's response: ", responseWords)
                print("Crackle's emotion: ", emotion)
                # self.gpt_api.speakText(responseWords, output_path="response.mp3")
                self.gpt_api.speak_text_eleven_labs(responseWords, output_path="response.mp3")
                playsound("response.mp3", block=True)
                self.planner_api.set_emotion(emotion)
                if "dance" in text.lower():
                    print("Command recognized: Dance")
                    # Execute dance action
                    print("Crackle is dancing now!")
                    self.planner_api.dance_dance()
                    print("Crackle finished dancing.")

                print(f"Transcribed text: {text}")

                # For simplicity, we transition back to IDLE after listening
                self._state = CrackleState.IDLE


async def func1():
    print("Function 1 started")
    await asyncio.sleep(3)
    print("Function 1 completed")
async def func2():
    print("Function 2 started")
    await asyncio.sleep(1)
    print("Function 2 co1pleted")
async def func3():
    print("Function 3 started")
    await asyncio.sleep(3)
    print("Function 3 completed")

def main():
    fsm = CrackleFSM()
    print(f"Initial state: {fsm._state}")
    main_thread = threading.Thread(target=lambda: asyncio.run(fsm.main_loop()), daemon=True)
    main_thread.start()
    main_thread.join()

if __name__ == "__main__":
    print("Starting Crackle FSM...")
    main()