"""
This implements the Finite state Machine (FSA) for Crackle that
decides what actions to take based on the current state and inputs.

"""

from enum import Enum
import threading
import asyncio
import multiprocessing
from typing import List, Dict

class CrackleState(Enum):
    IDLE = "idle"
    TASK = "task"
    LISTENING = "listening"
    RESETTING = "resetting"
    FAILURE = "failure"

class CrackleFSM:
    def __init__(self):
        self.state = CrackleState.IDLE
        self._tasks : List[asyncio.Task] = []
        self.handlers : Dict[CrackleState, List[callable]] = {CrackleState.IDLE: [CrackleState.TASK],
         CrackleState.TASK: [CrackleState.IDLE, CrackleState.FAILURE],
         CrackleState.RESETTING: [CrackleState.IDLE],
         CrackleState.FAILURE: [CrackleState.RESETTING]}
        print(f"Initialized FSM in state: {self.state}")

    def _add_task(self, coro : any):
        task = asyncio.create_task(coro)
        self._tasks.append(task)
        return task 

    def handle_idle(self):
        # Create two threads: one for listening to commands and one for scanning
        # Listening thread interrupts the scanning thread and then processes the command
        async def scanning_thread(name: str, stop_event: asyncio.Event):
            while self.state == CrackleState.IDLE:
                print("Scanning for commands...")
                # run command 
                asyncio.sleep(1)  # Scanning delay
        async def listening_thread(name: str, stop_event: asyncio.Event):
            while self.state == CrackleState.IDLE:
                print("Listening for commands...")
                command_received = True  # Placeholder
                if command_received:
                    print("Command received, interrupting scanning...")
                    self.state = CrackleState.TASK
                    break
                asyncio.sleep(1)  # Listening delay
        pass

    def main_loop(self):
        while True:
            if self.state == CrackleState.IDLE:
                self.handle_idle()
            elif self.state == CrackleState.TASK:
                self.handle_task()
            elif self.state == CrackleState.RESETTING:
                self.handle_resetting()
            elif self.state == CrackleState.FAILURE:
                self.handle_failure()

def main():
    fsm = CrackleFSM()
    asyncio.create_task(fsm.main_loop())
     
    print(f"Initial state: {fsm.state}")
    asyncio.run(task)

if __name__ == "__main__":
    print("Starting Crackle FSM...")
    main()