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

openwakeword.utils.download_models()

FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 16000
CHUNK = 1280


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

    def __wakeup_callback(self, indata, frames, time, status):
        if status:
            print(status)
        # Check if the wake word is detected
        if self._model.detect(indata):
            print("Wake word detected!")
            self._state = CrackleState.LISTENING
            self.interrupt_current_tasks()
            self._stop_event.set()

    async def handle_idle(self):
        # Create two threads: one for listening to commands and one for scanning
        # Listening thread interrupts the scanning thread and then processes the command
        print("Entering IDLE state: Scanning and Listening for commands...")
        time.sleep(1)  # Simulate setup time
        print("Setup complete, starting scanning and listening threads.")
        await asyncio.sleep(2)  # Simulate idle time

        async def scanning_thread(name: str, stop_event: asyncio.Event):
            while self._state == CrackleState.IDLE and not stop_event.is_set():
                print("Scanning for commands...")
                # run command
                await asyncio.sleep(5)  # Scanning delay

        async def listening_thread(name: str, stop_event: asyncio.Event):
            while self._state == CrackleState.IDLE:
                audio = np.frombuffer(self._mic_stream.read(CHUNK), dtype=np.int16)
                prediction = self._owwModel.predict(audio)
                print(prediction["hey_jarvis"])
                output_string_header = """
                Model Name         | Score | Wakeword Status
                --------------------------------------
                """
                for mdl in self._owwModel.prediction_buffer.keys():
                            # Add scores in formatted table
                            scores = list(self._owwModel.prediction_buffer[mdl])
                            curr_score = format(scores[-1], '.20f').replace("-", "")

                            output_string_header += f"""{mdl}{" "*(16 - len(mdl))}   | {curr_score[0:5]} | {"--"+" "*20 if scores[-1] <= 0.5 else "Wakeword Detected!"}
                            """
                # if prediction:
                #     print("Command received, interrupting scanning...")
                #     self._state = CrackleState.LISTENING
                #     stop_event.set()
                #     break

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