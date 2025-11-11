"""
This implements the Finite state Machine (FSA) for Crackle that
decides what actions to take based on the current state and inputs.

"""

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

    async def main_loop(self):
        # Consume the latest asyncio task in the queue

        while True:
            if self._state == CrackleState.IDLE:
                # Create a task to run handle_idle without blocking
                print("State is IDLE")
                t = asyncio.create_task(self.handle_idle())
                await t
            elif self._state == CrackleState.LISTENING:
                print("State is LISTENING")
                time.sleep(3)  # Simulate listening time
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