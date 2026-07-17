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
import numpy as np
import openwakeword
from openwakeword.model import Model
import multiprocessing
from typing import List, Dict, Any
import pyaudio
import os

# from planner import main_planner
import wave
from openai import OpenAI

openai_key = os.environ.get("OPENAI_API_KEY")

# openwakeword.utils.download_models()

FORMAT = pyaudio.paInt16
CHANNELS = 1
MODEL_RATE = 16000   # rate openwakeword and Whisper expect
CHUNK = 1280         # samples at MODEL_RATE (80 ms)

def _downsample(audio: np.ndarray, from_rate: int, to_rate: int) -> np.ndarray:
    if from_rate == to_rate:
        return audio
    import scipy.signal as sps
    from math import gcd
    g = gcd(from_rate, to_rate)
    return sps.resample_poly(audio, to_rate // g, from_rate // g).astype(np.int16)

ROS_ENABLED = os.environ.get("ROS_ENABLED", "false").lower() == "true"
print(f"ROS_ENABLED: {ROS_ENABLED}")

if ROS_ENABLED:
    from crackle_planning._api import PlannerAPI
    from crackle_planning._llm import GptAPI
    from crackle_planning.face_ui import ui_client
else:
    from _api import PlannerAPI
    from _llm import GptAPI
    from face_ui import ui_client

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

        self.planner_api = PlannerAPI(use_ros=ROS_ENABLED)
        self._running_threads = []
        self.handlers = {
            CrackleState.IDLE: [CrackleState.TASK],
            CrackleState.TASK: [CrackleState.IDLE, CrackleState.FAILURE],
            CrackleState.RESETTING: [CrackleState.IDLE],
            CrackleState.FAILURE: [CrackleState.RESETTING],
        }

        # Use TFLite as the inference framework
        self.INFERENCE_FRAMEWORK = "tflite"

        # Path to your custom TFLite model — prefer ROS workspace, fall back to repo location
        custom_model_path = os.path.join(
            os.environ.get("HOME", ""),
            "crackle_ws", "src", "crackle",
            "crackle_planning", "crackle_planning",
            "leeoh.tflite",
        )
        if not os.path.exists(custom_model_path):
            custom_model_path = os.path.join(os.path.dirname(__file__), "leeoh.tflite")
        print("custom_model_path:", custom_model_path)

        # Key you'll use in the prediction dict
        self.WAKEWORD_NAME = "leeoh"
        self.WAKEWORD_NAME = "leeoh"

        # Use PulseAudio (parec) for mic input — works reliably regardless of ALSA config
        import subprocess
        self._mic_rate = MODEL_RATE  # parec delivers at the rate we request
        self._mic_chunk = CHUNK      # 1280 samples = 80 ms at 16 kHz
        self._parec_proc = subprocess.Popen(
            ["parec", "--raw", "--format=s16le",
             f"--rate={MODEL_RATE}", "--channels=1"],
            stdout=subprocess.PIPE,
        )
        print(f"Mic: PulseAudio parec @ {self._mic_rate} Hz")

        # Load ONLY your custom wake word
        self._owwModel = Model(
            wakeword_models=[custom_model_path],
            inference_framework=self.INFERENCE_FRAMEWORK,
        )
        # self._n_models = len(self._owwModel.models.keys())

        
        self.gpt_api = GptAPI()

        #Stores the current command/prompt 
        self.current_command = ""

        #The context window of fsm 
        self.context_window = [] 

        print(f"Initialized FSM in state: {self._state}")


    def _add_task(self, coro: Any):
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
        ui_client.set_state("idle")
        async def scanning_thread(name: str, stop_event: asyncio.Event):
            while self._state == CrackleState.IDLE and not stop_event.is_set():
                print("Scanning for commands...")
                ui_client.set_state("idle")  # heartbeat so the pipeline UI can tell IDLE is alive, not stalled
                await asyncio.sleep(5)  # Scanning delay

        def _read_chunk():
            raw = self._parec_proc.stdout.read(self._mic_chunk * 2)  # *2 for int16 bytes
            return np.frombuffer(raw, dtype=np.int16)

        async def listening_thread(name: str, stop_event: asyncio.Event):
            frame = 0
            while self._state == CrackleState.IDLE:
                audio = _read_chunk()
                prediction = self._owwModel.predict(audio)
                score = prediction[self.WAKEWORD_NAME]
                if score > 0.01 or frame % 50 == 0:
                    print(f"[wake] score={score:.4f}  rms={int(np.sqrt(np.mean(audio.astype(np.float32)**2)))}")
                frame += 1

                if score > 0.1:
                    print(f"Wake word detected with score {score:.3f}")
                    self._state = CrackleState.LISTENING
                    ui_client.set_wakeword(True)
                    ui_client.set_state("listening")
                    wake_wall_time = time.time()
                    self.planner_api.look_at_sound_direction(wake_wall_time)
                    self._drain_mic_buffer()
                    stop_event.set()
                    break

        stop_event_scanning = asyncio.Event()
        loop = asyncio.get_event_loop()
        kb_thread = threading.Thread(
            target=self._keyboard_trigger,
            args=(stop_event_scanning, loop),
            daemon=True,
        )
        kb_thread.start()
        listen_task = asyncio.create_task(listening_thread("Listener", stop_event_scanning))
        scan_task = asyncio.create_task(scanning_thread("Scanner", stop_event_scanning))
        self._running_threads.extend([scan_task, listen_task])
        await asyncio.gather(scan_task, listen_task)

    def _keyboard_trigger(self, stop_event: asyncio.Event, loop: asyncio.AbstractEventLoop):
        """Poll stdin for a space bar press as an alternative to the wake word.
        Runs in a daemon thread alongside the OWW listening task."""
        import sys
        import select
        import tty
        import termios
        if not sys.stdin.isatty():
            return
        fd = sys.stdin.fileno()
        old = termios.tcgetattr(fd)
        try:
            tty.setcbreak(fd)  # single-char reads; keeps Ctrl+C working
            while self._state == CrackleState.IDLE and not stop_event.is_set():
                readable, _, _ = select.select([sys.stdin], [], [], 0.1)
                if readable:
                    ch = sys.stdin.read(1)
                    if ch == ' ':
                        print("\n[keyboard] Space bar — activating listen")
                        self._state = CrackleState.LISTENING
                        loop.call_soon_threadsafe(stop_event.set)
                        break
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old)

    def _drain_mic_buffer(self):
        """Non-blocking drain of any mic audio that accumulated in the pipe buffer.
        Also feeds silence to the wake word model to flush its sliding-window state,
        preventing false re-detection on the next IDLE pass."""
        import fcntl
        fd = self._parec_proc.stdout.fileno()
        flags = fcntl.fcntl(fd, fcntl.F_GETFL)
        fcntl.fcntl(fd, fcntl.F_SETFL, flags | os.O_NONBLOCK)
        try:
            drained = 0
            while True:
                try:
                    data = os.read(fd, self._mic_chunk * 2)
                    if not data:
                        break
                    drained += len(data)
                except BlockingIOError:
                    break
        finally:
            fcntl.fcntl(fd, fcntl.F_SETFL, flags)
        print(f"[mic] drained {drained // 2 / MODEL_RATE:.2f}s of stale audio")
        # OWW uses ~1.5s of context; feed silence to evict the wake word from its window
        silence = np.zeros(self._mic_chunk, dtype=np.int16)
        for _ in range(10):
            self._owwModel.predict(silence)

    async def handle_task(self):
        ui_client.set_state("thinking")
        print("Entering TASK state: Executing task...")
        print(f"Current command: {self.current_command}")

        # Tool calls are executed inside get_command via the agentic loop.
        response = self.gpt_api.get_command(self, self.current_command, self.planner_api)
        print(f"Response: {response}")

        # Surface the emotion + reply on the face UI (and drive the ROS face).
        if response.emotion is not None:
            ui_client.set_emotion(response.emotion)
            self.planner_api.set_emotion(response.emotion)
        if response.dialoge is not None:
            ui_client.set_response(response.dialoge)

        if response.dialoge is not None:
            if not response.tts_handled:
                self.gpt_api.speak_text_eleven_labs(response.dialoge)
                print("[INFO] Sound played")
            # Allow PulseAudio's DAC buffer to drain before reading the mic again.
            time.sleep(0.2)
            self._drain_mic_buffer()
        else:
            print("[ERROR] response dialogue was none")

        if response.continue_talking:
            print("Continuing conversation — returning to LISTENING state")
            self._state = CrackleState.LISTENING
            ui_client.set_state("listening")
            return

        print("TASK COMPLETED")
        self._state = CrackleState.IDLE
        ui_client.set_state("idle")
        
    async def handle_resetting(self):
        print("Entering RESETTING state: Resetting system...")

    async def handle_failure(self):
        print("Entering FAILURE state: Handling failure...")

    def record_output(
        self,
        max_seconds: float = 15.0,
        silence_duration: float = 0.4,
        silence_rms_threshold: float = 500.0,
        start_timeout: float = 5.0,
    ):
        """Record audio until the speaker pauses, or max_seconds is reached.

        Uses a simple RMS threshold as a voice activity detector: waits for the
        speaker to start, then stops after `silence_duration` seconds of
        continuous quiet. `start_timeout` bounds how long we wait for any
        speech to begin before giving up. Returns 16kHz PCM regardless of mic rate.
        """
        chunks_per_second = MODEL_RATE / self._mic_chunk
        max_frames = int(max_seconds * chunks_per_second)
        silence_frames_needed = int(silence_duration * chunks_per_second)
        start_timeout_frames = int(start_timeout * chunks_per_second)

        # Calibrate noise floor over ~0.3s so the VAD threshold adapts to the
        # mic gain / ambient noise level of whatever machine we're running on.
        calibration_frames = int(0.3 * chunks_per_second)
        noise_rmses = []
        for _ in range(calibration_frames):
            raw = self._parec_proc.stdout.read(self._mic_chunk * 2)
            s = np.frombuffer(raw, dtype=np.int16)
            noise_rmses.append(float(np.sqrt(np.mean(s.astype(np.float32) ** 2))))
        noise_floor = float(np.mean(noise_rmses))
        adaptive_threshold = max(silence_rms_threshold, noise_floor * 3.0)
        print(f"[VAD] noise_floor={noise_floor:.0f}  threshold={adaptive_threshold:.0f}")

        buf = []
        has_spoken = False
        silent_count = 0

        for frame_idx in range(max_frames):
            raw = self._parec_proc.stdout.read(self._mic_chunk * 2)
            samples = np.frombuffer(raw, dtype=np.int16)
            buf.append(samples)

            rms = float(np.sqrt(np.mean(samples.astype(np.float32) ** 2)))

            if rms > adaptive_threshold:
                has_spoken = True
                silent_count = 0
            else:
                if has_spoken:
                    silent_count += 1
                    if silent_count >= silence_frames_needed:
                        break
                elif frame_idx >= start_timeout_frames:
                    break

        return np.concatenate(buf)
    
    def _samples_to_wav_bytes(self, samples: np.ndarray, sample_rate: int = 16000) -> bytes:
        """Encode PCM samples as a WAV byte string (no disk I/O)."""
        import io
        buf = io.BytesIO()
        with wave.open(buf, "wb") as wf:
            wf.setnchannels(1)
            wf.setsampwidth(2)
            wf.setframerate(sample_rate)
            wf.writeframes(samples.tobytes())
        return buf.getvalue()

    def save_as_wav(self, samples: np.ndarray, sample_rate=16000, filename="capture.wav"):
        """Save audio to disk (kept for debugging)."""
        with wave.open(filename, "wb") as wf:
            wf.setnchannels(1)
            wf.setsampwidth(2)
            wf.setframerate(sample_rate)
            wf.writeframes(samples.tobytes())



    async def main_loop(self):
        # Consume the latest asyncio task in the queue

        while True:
            if self._state == CrackleState.IDLE:
                # Create a task to run handle_idle without blocking
                print("State is IDLE")
                t = asyncio.create_task(self.handle_idle())
                await t
            elif self._state == CrackleState.LISTENING:
                print("State is LISTEN")
                print("Crackle is Listening now...")
                # start listening for a command (call function)
                samples = self.record_output()
                print("Recording complete.")
                wav_bytes = self._samples_to_wav_bytes(samples)
                ui_client.emit_stage("api:whisper_stt", "loading")
                text = self.gpt_api.speech_to_text(wav_bytes)
                ui_client.emit_stage("api:whisper_stt", "done", text=text)
                print(f"Transcribed words: {text}")
                self.current_command = text
                ui_client.set_transcript(text)
                ui_client.set_state("thinking")
                self._state = CrackleState.TASK #start working on the task/just talk back

                # print("Crackle's response: ", responseWords)
                # print("Crackle's emotion: ", emotion)
                # self.gpt_api.speakText(responseWords, output_path="response.mp3")
                #action = self.gpt_api.get_command(text)
                #responseWords = action["dialogue"]
                #self.gpt_api.speak_text_eleven_labs(responseWords, output_path="response.mp3")
                # self.gpt_api.speak_text_eleven_labs()
                # playsound("response.mp3", block=True)
                #emotion = action["emotion"]
                #self.planner_api.set_emotion(emotion)
                #api = self.planner_api
                #exec(action["code"])
                # if "dance" in text.lower():
                #     print("Command recognized: Dance")
                #     # Execute dance action
                #     print("Crackle is dancing now!")
                #     self.planner_api.dance_dance()
                #     print("Crackle finished dancing.")

                # print(f"Transcribed text: {text}")

                # For simplicity, we transition back to IDLE after listening
                #self._state = CrackleState.IDLE
                
            elif self._state == CrackleState.TASK:
                print("State is TASK")
                t = asyncio.create_task(self.handle_task())
                await t

def main():
    fsm = CrackleFSM()
    #fsm._state = CrackleState.TASK
    print(f"Initial state: {fsm._state}")
    main_thread = threading.Thread(target=lambda: asyncio.run(fsm.main_loop()), daemon=True)
    main_thread.start()
    main_thread.join()

if __name__ == "__main__":
    print("Starting Crackle FSM...")
    main()