"""
Simulated ROS interface for running and evaluating plans without the physical
robot or a live ROS stack.

Drop-in replacement for RosInterface — exposes the same method signatures but
maintains an in-memory world model so every action is logged and the final
state can be inspected or asserted against.

Usage:
    from sim_ros_interface import SimulatedRosInterface
    from _api import PlannerAPI

    sim = SimulatedRosInterface(
        objects=["water_bottle", "coffee_mug", "apple"],
        persons=["Yash"],
    )
    api = PlannerAPI(use_ros=False, sim=sim)

    api.pick_up("water bottle")
    api.place()

    sim.assert_nothing_held()
    sim.assert_object_in_scene("water_bottle")
    sim.print_summary()

To test failure handling, inject failures by method name:
    sim = SimulatedRosInterface(
        objects=["water_bottle"],
        fail_on={"call_pickup_service": True},
    )
"""

from dataclasses import dataclass
from typing import Any, Dict, List, Optional


@dataclass
class SimObject:
    name: str
    location: str = "table"


@dataclass
class SimAction:
    method: str
    args: Dict[str, Any]
    success: bool
    result: Any
    note: str = ""


class SimulatedRosInterface:

    def __init__(
        self,
        objects: Optional[List[str]] = None,
        persons: Optional[List[str]] = None,
        fail_on: Optional[Dict[str, bool]] = None,
    ):
        """
        Args:
            objects:  Names of objects present in the simulated scene at start.
            persons:  Names returned by recognize_person().
            fail_on:  Map of method name → True to force that call to fail,
                      e.g. {"call_pickup_service": True} to test error recovery.
        """
        self._initial_objects = list(objects or [])
        self._persons = list(persons or ["unknown_person"])
        self._fail_on: Dict[str, bool] = fail_on or {}
        self._reset_state()

    def _reset_state(self) -> None:
        self._scene_objects: Dict[str, SimObject] = {
            name: SimObject(name=name) for name in self._initial_objects
        }
        self._held_object: Optional[str] = None
        self._gripper_closed: bool = False
        self._emotion: str = "neutral"
        self._action_log: List[SimAction] = []

    def _log(
        self, method: str, args: dict, success: bool, result: Any, note: str = ""
    ) -> None:
        entry = SimAction(
            method=method, args=args, success=success, result=result, note=note
        )
        self._action_log.append(entry)
        status = "OK  " if success else "FAIL"
        arg_str = ", ".join(f"{k}={repr(v)}" for k, v in args.items())
        print(f"[sim] {status} | {method}({arg_str}){' — ' + note if note else ''}")

    def _should_fail(self, method: str) -> bool:
        return self._fail_on.get(method, False)

    # ---- RosInterface API ------------------------------------------------

    def get_scene_object_names(self) -> List[str]:
        """Return names of all objects currently in the simulated scene."""
        names = list(self._scene_objects.keys())
        self._log(
            "get_scene_object_names", {}, True, names,
            f"{len(names)} object(s): {names}",
        )
        return names

    def get_latest_color_image_jpeg_b64(self) -> Optional[str]:
        """No camera in simulation — idle scanning skips GPT-vision when None."""
        self._log("get_latest_color_image_jpeg_b64", {}, True, None, "no sim camera")
        return None

    def call_scan_service(self) -> bool:
        """Re-populate the scene with any initial objects not currently held."""
        if self._should_fail("call_scan_service"):
            self._log("call_scan_service", {}, False, False, "injected failure")
            return False
        for name in self._initial_objects:
            if name not in self._scene_objects and name != self._held_object:
                self._scene_objects[name] = SimObject(name=name)
        names = list(self._scene_objects.keys())
        self._log("call_scan_service", {}, True, True, f"scene: {names}")
        return True

    def call_find_objects(self, names: List[str]) -> List[str]:
        """Return which of the requested names exist in the simulated scene."""
        if self._should_fail("call_find_objects"):
            self._log("call_find_objects", {"names": names}, False, [], "injected failure")
            return []
        found = []
        for req in names:
            if req in self._scene_objects:
                found.append(req)
                continue
            # Substring match as fallback (mirrors the embedding matcher's intent)
            req_norm = req.lower().replace(" ", "_")
            for scene_name in self._scene_objects:
                scene_norm = scene_name.lower().replace(" ", "_")
                if req_norm in scene_norm or scene_norm in req_norm:
                    found.append(scene_name)
                    break
        note = f"found: {found}" if found else "no matches in scene"
        self._log("call_find_objects", {"names": names}, bool(found), found, note)
        return found

    def move_to_pose(self, pose, abort_event=None) -> bool:
        """Simulate an arm motion to the given pose."""
        if abort_event is not None and abort_event.is_set():
            self._log("move_to_pose", {"pose": repr(pose)}, False, False, "aborted before execution")
            return False
        if self._should_fail("move_to_pose"):
            self._log("move_to_pose", {"pose": repr(pose)}, False, False, "injected failure")
            return False
        self._log("move_to_pose", {"pose": repr(pose)}, True, True, "arm moved to pose")
        return True

    def stop_motion(self) -> None:
        """No-op in simulation — there is no executing trajectory to abort."""
        self._log("stop_motion", {}, True, None, "no-op (sim)")

    def call_pickup_service(self, object_name: str):
        """Pick up the named object: remove it from the scene and mark it as held."""
        if self._should_fail("call_pickup_service"):
            self._log(
                "call_pickup_service", {"object_name": object_name}, False, None,
                "injected failure",
            )
            return None
        if object_name not in self._scene_objects:
            self._log(
                "call_pickup_service", {"object_name": object_name}, False, None,
                f"'{object_name}' not in scene",
            )
            return None
        del self._scene_objects[object_name]
        self._held_object = object_name
        self._gripper_closed = True
        self._log(
            "call_pickup_service", {"object_name": object_name}, True, True,
            f"now holding '{object_name}'",
        )
        return True

    def clear_and_refresh_octomap(self, settle_time: float = 1.5) -> None:
        """No-op in simulation — state transitions happen via pickup/open_gripper."""
        self._log(
            "clear_and_refresh_octomap", {"settle_time": settle_time}, True, None,
            "OctoMap refreshed (sim no-op)",
        )

    def look_at_person(self, wake_word_time: Optional[float] = None) -> None:
        """Simulate turning the robot head toward the speaker."""
        self._log(
            "look_at_person", {"wake_word_time": wake_word_time}, True, None,
            "head turned toward speaker",
        )

    def dance(self):
        """Simulate the dance routine."""
        if self._should_fail("dance"):
            self._log("dance", {}, False, None, "injected failure")
            return None
        self._log("dance", {}, True, True, "dancing!")
        return True

    def set_emotion(self, emotion: str) -> None:
        """Update the displayed emotion."""
        self._emotion = emotion
        self._log("set_emotion", {"emotion": emotion}, True, None, f"emotion → '{emotion}'")

    def go_to_hold_pose(self) -> bool:
        """Simulate moving to the hold pose and opening the gripper to receive
        an object ("hold this for me")."""
        if self._should_fail("go_to_hold_pose"):
            self._log("go_to_hold_pose", {}, False, False, "injected failure")
            return False
        self._gripper_closed = False
        self._log("go_to_hold_pose", {}, True, True,
                  "moved to hold pose, gripper open to receive object")
        return True

    def close_gripper(self) -> None:
        """Close the simulated gripper."""
        self._gripper_closed = True
        self._log("close_gripper", {}, True, None, "gripper closed")

    def open_gripper(self) -> None:
        """Open the simulated gripper; places any held object back into the scene."""
        if self._held_object:
            placed = self._held_object
            self._scene_objects[placed] = SimObject(name=placed, location="placed")
            self._held_object = None
            note = f"gripper opened — '{placed}' placed in scene"
        else:
            note = "gripper opened (nothing held)"
        self._gripper_closed = False
        self._log("open_gripper", {}, True, None, note)

    def recognize_person(self) -> List[str]:
        """Return the configured list of recognized persons."""
        self._log(
            "recognize_person", {}, True, self._persons,
            f"recognized: {self._persons}",
        )
        return list(self._persons)

    # ---- Evaluation helpers -----------------------------------------------

    def get_action_log(self) -> List[SimAction]:
        """Return the full ordered log of every simulated action."""
        return list(self._action_log)

    def get_state(self) -> Dict[str, Any]:
        """Return a snapshot of the current simulated world state."""
        return {
            "scene_objects": list(self._scene_objects.keys()),
            "held_object": self._held_object,
            "gripper_closed": self._gripper_closed,
            "emotion": self._emotion,
            "actions_taken": len(self._action_log),
            "failed_actions": sum(1 for a in self._action_log if not a.success),
        }

    def reset(self) -> None:
        """Reset all state back to the initial configuration."""
        self._reset_state()

    # ---- Assertions -------------------------------------------------------

    def assert_object_held(self, name: str) -> None:
        assert self._held_object == name, (
            f"Expected to be holding '{name}', but holding '{self._held_object}'"
        )

    def assert_nothing_held(self) -> None:
        assert self._held_object is None, (
            f"Expected gripper empty, but holding '{self._held_object}'"
        )

    def assert_gripper_open(self) -> None:
        assert not self._gripper_closed, "Expected gripper open, but it is closed"

    def assert_gripper_closed(self) -> None:
        assert self._gripper_closed, "Expected gripper closed, but it is open"

    def assert_object_in_scene(self, name: str) -> None:
        assert name in self._scene_objects, (
            f"Expected '{name}' in scene, but it is not (held={self._held_object})"
        )

    def assert_object_not_in_scene(self, name: str) -> None:
        assert name not in self._scene_objects, (
            f"Expected '{name}' not in scene, but it is"
        )

    def assert_all_actions_succeeded(self) -> None:
        failed = [a for a in self._action_log if not a.success]
        assert not failed, (
            f"{len(failed)} action(s) failed: "
            + ", ".join(f"{a.method}({a.args})" for a in failed)
        )

    # ---- Display ----------------------------------------------------------

    def print_summary(self) -> None:
        """Print a human-readable summary of the full simulation run."""
        state = self.get_state()
        sep = "=" * 52
        print(f"\n{sep}")
        print("  Simulation Summary")
        print(sep)
        print(f"  Scene objects  : {state['scene_objects']}")
        print(f"  Held object    : {state['held_object']}")
        print(f"  Gripper        : {'closed' if state['gripper_closed'] else 'open'}")
        print(f"  Emotion        : {state['emotion']}")
        print(f"  Total actions  : {state['actions_taken']}")
        print(f"  Failed actions : {state['failed_actions']}")
        print()
        print("  Action log:")
        for i, action in enumerate(self._action_log, 1):
            status = "✓" if action.success else "✗"
            arg_str = ", ".join(f"{k}={repr(v)}" for k, v in action.args.items())
            line = f"    {i:2d}. [{status}] {action.method}({arg_str})"
            print(line)
            if action.note:
                print(f"          → {action.note}")
        print(f"{sep}\n")
