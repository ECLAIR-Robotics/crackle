"""
This implements the Finite state Machine (FSA) for Crackle that
decides what actions to take based on the current state and inputs.

"""

from fileinput import filename
import time
from enum import Enum
import threading
import asyncio
import base64
import concurrent.futures
import traceback
import datetime
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
    from crackle_planning._config import load_config
else:
    from _api import PlannerAPI
    from _llm import GptAPI
    from face_ui import ui_client
    from _config import load_config

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

        # Idle-scan scene analysis ("func a"): GPT-vision + find_objects. Runs on a
        # single-worker executor so at most one analysis is ever in flight — we
        # must not load the vision model into memory twice. The future persists on
        # self so that if the user wakes us mid-analysis, that analysis keeps
        # running in the background while we handle the command.
        self._scan_executor = concurrent.futures.ThreadPoolExecutor(max_workers=1)
        self._pending_scan_future: concurrent.futures.Future | None = None
        self._idle_scan_image_dir = os.path.join(os.path.dirname(__file__), "idle_scans")
        # The active idle-sweep thread and its stop flag (started in handle_idle).
        self._scan_thread: threading.Thread | None = None
        self._scan_stop_event: threading.Event | None = None
        # Background thread that turns the arm to face the user after the wake
        # word, so listening/processing can start without waiting on the motion.
        self._look_thread: threading.Thread | None = None

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

        # Run the blocking move/capture/analyze sweep in a REAL daemon thread —
        # NOT an asyncio task. The wake-word listener below is an async coroutine
        # that makes blocking mic reads with no awaits, so it never yields the
        # event loop; anything scheduled as a coroutine (e.g. asyncio.to_thread)
        # would be starved and never start. Setting scan_stop_event, or leaving
        # IDLE, stops the sweep at its next checkpoint.
        #
        # Make sure a prior sweep has stopped before starting a new one so two
        # threads never command the arm at once. By the time we re-enter IDLE the
        # previous sweep has almost always already exited (it bails as soon as the
        # state left IDLE), so this join returns immediately in practice.
        if self._scan_thread is not None and self._scan_thread.is_alive():
            if self._scan_stop_event is not None:
                self._scan_stop_event.set()
            self._scan_thread.join(timeout=5.0)
            if self._scan_thread.is_alive():
                print("[idle-scan] previous sweep still finishing a move; starting new "
                      "sweep once it yields.")

        scan_stop_event = threading.Event()
        self._scan_stop_event = scan_stop_event
        # Idle scanning is opt-in via crackle_config.json — re-read each time we
        # enter IDLE so the flag can be flipped without a restart. When disabled
        # the arm just listens (no sweep thread is started).
        if load_config().get("idle_scanning_enabled", True):
            self._scan_thread = threading.Thread(
                target=self._run_idle_scan_loop,
                args=(scan_stop_event,),
                daemon=True,
            )
            self._scan_thread.start()
        else:
            self._scan_thread = None
            print("[idle-scan] disabled via config (idle_scanning_enabled=false) — "
                  "listening only.")

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
                    # Instantly abort any scan trajectory in flight and flag the
                    # sweep to stop, BEFORE looking at the sound. stop_motion frees
                    # the arm (and the manipulation node's service group) so the
                    # look_at motion below can run immediately instead of queuing
                    # behind the scan move.
                    self.planner_api.stop_motion()
                    scan_stop_event.set()  # stop the idle sweep; func-a keeps running
                    # Turn to face the user in the BACKGROUND so we can start
                    # listening for their command immediately. The look-at motion
                    # takes a second or two; blocking on it here meant a command
                    # spoken while the arm was still turning got missed. Recording
                    # (parec mic) and LLM processing don't need the arm, so they
                    # proceed in parallel; if the command needs the arm, that motion
                    # naturally serializes after the look on the manipulation node.
                    self._look_thread = threading.Thread(
                        target=self.planner_api.look_at_sound_direction,
                        args=(wake_wall_time,),
                        daemon=True,
                    )
                    self._look_thread.start()
                    self._drain_mic_buffer()
                    stop_event.set()
                    break

        stop_event_scanning = asyncio.Event()
        loop = asyncio.get_event_loop()
        kb_thread = threading.Thread(
            target=self._keyboard_trigger,
            args=(stop_event_scanning, loop, scan_stop_event),
            daemon=True,
        )
        kb_thread.start()
        listen_task = asyncio.create_task(listening_thread("Listener", stop_event_scanning))
        self._running_threads.append(listen_task)
        # Only await the listener. When it breaks (wake word / keyboard) we return
        # so main_loop can handle the command; the scan thread notices the state
        # change / scan_stop_event and exits on its own. We deliberately do NOT
        # join it here — a move or func-a analysis may still be finishing, and it's
        # a daemon that stops itself, so blocking here would only add latency.
        await listen_task

    def _keyboard_trigger(
        self,
        stop_event: asyncio.Event,
        loop: asyncio.AbstractEventLoop,
        scan_stop_event: "threading.Event | None" = None,
    ):
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
                        if scan_stop_event is not None:
                            scan_stop_event.set()
                        loop.call_soon_threadsafe(stop_event.set)
                        break
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old)

    # ----- Idle-state workspace scanning -------------------------------------

    def _idle_scan_interrupted(self, stop_event: "threading.Event") -> bool:
        """True once we should abandon the sweep (wake word / keyboard / left IDLE)."""
        return stop_event.is_set() or self._state != CrackleState.IDLE

    def _await_previous_scene_analysis(self):
        """Block until any in-flight scene analysis ("func a") finishes.

        This is the ONLY place idle scanning blocks: we refuse to launch a second
        GPT-vision + find_objects pass while one is still running, so the vision
        model is never loaded into memory twice.
        """
        fut = self._pending_scan_future
        if fut is not None and not fut.done():
            print("[idle-scan] previous scene analysis still running — waiting for it "
                  "to finish before scanning again...")
            try:
                fut.result()
            except Exception:
                traceback.print_exc()

    def _store_scan_image(self, image_b64: str):
        """Persist a captured frame to disk for later inspection/debugging."""
        try:
            os.makedirs(self._idle_scan_image_dir, exist_ok=True)
            ts = datetime.datetime.now().strftime("%Y%m%d_%H%M%S_%f")
            path = os.path.join(self._idle_scan_image_dir, f"scan_{ts}.jpg")
            with open(path, "wb") as f:
                f.write(base64.b64decode(image_b64))
            print(f"[idle-scan] stored capture: {path}")
        except Exception:
            traceback.print_exc()

    def _process_scene_capture(self, image_b64: str):
        """"func a": GPT-vision -> graspable object names -> find_objects.

        Runs on the single-worker scan executor (never concurrently with itself).
        find_objects publishes any detections to the MoveIt planning scene as
        collision objects. Never raises — failures are logged and swallowed so a
        bad frame can't kill the background worker.
        """
        try:
            objects = self.gpt_api.detect_graspable_objects(image_b64)
            if not objects:
                print("[idle-scan] no graspable objects in this view.")
                return
            found = self.planner_api.find_and_add_objects(objects)
            print(f"[idle-scan] added to planning scene: {found}")
        except Exception:
            traceback.print_exc()

    def _run_idle_scan_loop(self, stop_event: "threading.Event"):
        """Continuously sweep the configured idle-scan poses while IDLE.

        At each pose: move (blocking), capture + store a picture, then kick off
        scene analysis ("func a") in the background so it overlaps the move to the
        next pose. Before launching the next analysis we block only if the previous
        one is still running. Fully interruptable: we bail at every checkpoint once
        the wake word/keyboard fires, leaving any in-flight analysis to finish in
        the background.
        """
        poses = self.planner_api.load_idle_scan_poses()
        if not poses:
            print("[idle-scan] no idle_scan_poses configured — idling without scanning. "
                  "Populate idle_scan_poses.json to enable workspace scanning.")
            return

        print(f"[idle-scan] sweeping {len(poses)} pose(s) while idle...")
        while not self._idle_scan_interrupted(stop_event):
            for pose in poses:
                if self._idle_scan_interrupted(stop_event):
                    return

                # Pass the stop flag so the move bails between plan and execute if
                # the wake word fires during the (short) planning window; an
                # already-executing trajectory is aborted via stop_motion instead.
                moved = self.planner_api.move_to_scan_pose(pose, abort_event=stop_event)
                if self._idle_scan_interrupted(stop_event):
                    return
                if not moved:
                    print("[idle-scan] failed to reach a scan pose; skipping it.")
                    continue

                # Arm is settled — take and store a picture.
                image_b64 = self.planner_api.capture_scene_image()
                if image_b64 is None:
                    continue
                self._store_scan_image(image_b64)

                # Only block if a prior analysis is still in flight (single model).
                self._await_previous_scene_analysis()
                if self._idle_scan_interrupted(stop_event):
                    return

                # Launch func-a in the background; it overlaps the next move.
                self._pending_scan_future = self._scan_executor.submit(
                    self._process_scene_capture, image_b64
                )

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
                text = self.gpt_api.speech_to_text(wav_bytes)
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