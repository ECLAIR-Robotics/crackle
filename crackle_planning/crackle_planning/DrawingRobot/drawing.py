import tkinter as tk
from typing import List, Tuple
import numpy as np
import matplotlib.pyplot as plt
from scipy.spatial.distance import euclidean
from crackle_planning.arm_planner import PlannerNode
from geometry_msgs.msg import Pose
import rclpy
import rclpy.time
import time
from visualization_msgs.msg import Marker, MarkerArray

# Mock definitions for your pre-existing plan and execute functionalities.
def plan_path(points_3d: List[Tuple[float, float, float]]) -> List[Tuple[float, float, float]]:
    """
    Plan a path in 3D space for the robotic arm.
    In a real scenario, this function would handle IK, collision checks, etc.
    """
    # For demonstration, pretend the input points are already suitable as a path.
    return points_3d




class FreeDrawToRobot:
    def __init__(self, master=None):
        """
        Initialize the drawing application with a canvas and some control buttons.
        """
        self.master = master
        self.master.title("Free Draw to Robot")

        # Canvas size (in pixels)
        self.canvas_width = 600
        self.canvas_height = 600

        self.canvas_coords = [(0.087, 0.1592), (0.3188, 0.1592), (0.3188, -0.1027), (0.087, -0.1027)] # ABCD anti clockwise
        

        # Hardcoded dimensions/offsets for demonstration
        # In a real-world application, these would be calibrated carefully.
        self.PAPER_Z_HEIGHT = 0.0           # z-height of the paper plane
        self.END_EFFECTOR_OFFSET = 0.1064      # offset between pen tip and end-effector frame

        self.DEFAULT_POSITION = Pose()
        self.DEFAULT_POSITION.position.x = self.canvas_coords[0][0]
        self.DEFAULT_POSITION.position.y = self.canvas_coords[0][1]
        self.DEFAULT_POSITION.position.z = self.PAPER_Z_HEIGHT + self.END_EFFECTOR_OFFSET
        self.DEFAULT_POSITION.orientation.x = 1.0
        self.DEFAULT_POSITION.orientation.y = 0.0
        self.DEFAULT_POSITION.orientation.z = 0.0
        self.DEFAULT_POSITION.orientation.w = 0.0
        self.arm_api = PlannerNode()
        self.marker_publisher = self.arm_api.create_publisher(MarkerArray, '/path_visualizer_array', 10)
        self.arm_api.plan_to_pose(self.DEFAULT_POSITION)
        self.arm_api.execute_plan()

        self.SCALING_FACTOR = self.canvas_width / (self.canvas_coords[1][0] - self.canvas_coords[0][0]) # Ratio of widths assuming square 



        self.drawing_points = []  # will hold (x, y) in canvas space
        self.current_line = []    # will hold points for the line currently being drawn

        # Setup GUI
        self.canvas = tk.Canvas(self.master,
                                width=self.canvas_width,
                                height=self.canvas_height,
                                bg="white")
        self.canvas.pack()

        # Bind mouse events
        self.canvas.bind("<ButtonPress-1>", self.on_button_press)
        self.canvas.bind("<B1-Motion>", self.on_draw)
        self.canvas.bind("<ButtonRelease-1>", self.on_button_release)

        # Add a button to trigger the "Send to Robot" step
        self.send_button = tk.Button(self.master, text="Send to Robot", command=self.send_drawing_to_robot)
        self.send_button.pack(pady=10)

    def execute_path(self, path: List[Tuple[float, float, float]]) -> None:
        """
        Send the planned path to the arm controller for execution.
        In a real scenario, this might interface with actual robot control software.
        """
        print("Executing path on the robot...")


        # publish a series of markers to visualize the path

        marker_array = MarkerArray()
        i = 0
        for waypoint in path:
            marker = Marker()
            marker.header.frame_id = "link_base"
            
            marker.ns = "waypoints" + str(i)
            marker.id = i
            marker.type = Marker.SPHERE
            marker.action = Marker.ADD
            marker.pose.position.x = waypoint[0]
            marker.pose.position.y = waypoint[1]
            marker.pose.position.z = waypoint[2]
            marker.pose.orientation.x = 1.0
            marker.pose.orientation.y = 0.0
            marker.pose.orientation.z = 0.0
            marker.pose.orientation.w = 0.0
            marker.scale.x = 0.01
            marker.scale.y = 0.01
            marker.scale.z = 0.01
            marker.color.a = 1.0
            marker.color.r = 1.0
            marker.color.g = 0.0
            marker.color.b = 0.0
            marker_array.markers.append(marker)
            i+=1
        self.marker_publisher.publish(marker_array)

        prev_waypoint = path[0]
        for waypoint in path:

            # TODO: If the euclidean distance between two points is above a certain threshold
            # then 'pen up' and then pen down after you get to the point            

            pose = Pose()
            pose.position.x = waypoint[0]
            pose.position.y = waypoint[1]
            pose.position.z = waypoint[2]
            pose.orientation.x = 1.0
            pose.orientation.y = 0.0
            pose.orientation.z = 0.0
            pose.orientation.w = 0.0
            self.arm_api.plan_to_pose(pose)
            self.arm_api.execute_plan()
            time.sleep(0.5)
            print(f"Moving to {waypoint}")

    print("Execution complete.")
    def on_button_press(self, event):
        """
        Start a new line segment when the user presses the left mouse button.
        """
        # Begin a new line
        self.current_line = []
        # Record the first point of the line
        self.current_line.append((event.x, event.y))

    def on_draw(self, event):
        """
        Track the line as the user drags the mouse (left button still held).
        """
        # Get the last point
        x_prev, y_prev = self.current_line[-1]
        # Current point
        x_curr, y_curr = event.x, event.y

        # Draw a line segment on the canvas
        self.canvas.create_line(x_prev, y_prev, x_curr, y_curr, fill="black")

        # Append the current point to the line
        self.current_line.append((x_curr, y_curr))

    def on_button_release(self, event):
        """
        Finalize the line when the user releases the left mouse button.
        """
        # Add the final point
        self.current_line.append((event.x, event.y))
        # Store the line into the global drawing_points list
        # In a real scenario, you might store each line as a separate list
        # For simplicity, we’re flattening everything into `drawing_points`.
        self.drawing_points.extend(self.current_line)

    def rdp(self, points, epsilon = 0.5):
        """
            Ramer-Douglas-Peucker Algorithm
        """

        def recursive_rdp(pts, eps):
            if (len(pts) < 3):
                return pts
            start, end = np.array(pts[0]), np.array(pts[-1])
            dmax, index = 0, 0
            
            for i in range(1, len(pts) -1):
                d = np.abs(np.cross(end - start, start - pts[i])) / np.linalg.norm(end - start)
                if d > dmax:
                    index, dmax = i, d
            if dmax > eps: 
                left = recursive_rdp(pts[: index + 1], eps)
                right = recursive_rdp(pts[index: ], eps)
                return np.vstack((left[:-1], right))
            return np.array([start, end])
        
        return recursive_rdp(points, epsilon)
    
    def distance_based_filtered(self, points, min_dist=0.1):
        print(points)
        filtered = [points[0]]
        for p in points[1: ]:
            if euclidean(p, filtered[-1]) > min_dist:
                filtered.append(p)
        return np.array(filtered)
    
    def uniform_sampling(points, step = 2):
        return np.array(points[::step])

    def send_drawing_to_robot(self):
        """
        Convert the 2D drawing into robot coordinates, plan, and execute.
        """
        if not self.drawing_points:
            print("No drawing available to send to robot.")
            return

        # Convert 2D canvas coordinates into 3D robot coordinates
        # The simplest approach: (x_pixel, y_pixel) => (x_robot, y_robot, z_robot).
        # This is a placeholder for a real calibration/transform.
        points_3d = []
        print("Points: ", self.drawing_points)

        # rdp_result = self.rdp(self.drawing_points) 

        # points_np = np.array(self.drawing_points)
        # points_np = rdp_result

        points_np = self.distance_based_filtered(self.drawing_points, self.SCALING_FACTOR * 0.005)



        # plt.plot(points_np[: ,0], points_np[: , 1], 'bo-', label="Path")
        # plt.xlabel("X")
        # plt.ylabel("Y")
        # plt.title("Title")
        # plt.legend()
        # plt.grid(True)
        # plt.show()

        for x, y in self.drawing_points:
            # Convert from top-left pixel coordinates to a robot coordinate frame
            # e.g., assume top-left of the canvas is robot world (0,0),
            # and each pixel is scaled by PIXEL_TO_ROBOT_SCALE in millimeters.
            # Hardcode Z to the PAPER_Z_HEIGHT plus the pen offset if needed.
            x_robot = x / self.SCALING_FACTOR
            y_robot = y / self.SCALING_FACTOR
            z_robot = self.PAPER_Z_HEIGHT + self.END_EFFECTOR_OFFSET

            points_3d.append((x_robot, y_robot, z_robot))

        # Plan the path using the existing function
        path = plan_path(points_3d)

        # Execute the path
        self.execute_path(path)

        # (Optional) Clear the canvas/drawing points after sending to the robot
        self.clear_drawing()  # If you want to start fresh after each send

    def clear_drawing(self):
        """
        Clear the GUI canvas and any stored points.
        """
        self.canvas.delete("all")
        self.drawing_points.clear()

def main():
    root = tk.Tk()
    rclpy.init()
    app = FreeDrawToRobot(master=root)
    root.mainloop()

if __name__ == "__main__":
    main()
