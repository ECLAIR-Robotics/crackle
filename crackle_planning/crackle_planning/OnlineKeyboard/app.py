from flask import Flask, render_template, request, redirect, url_for, jsonify, session
import pyautogui
import keyboard
import screen_brightness_control as sbc
import time
from functools import wraps
from crackle_planning.arm_planner import PlannerNode
from geometry_msgs.msg import Pose
import rclpy

app = Flask(__name__)
app.secret_key = "{replace with key}"  # Replace with a strong secret key

# Hardcoded password
PASSWORD = "password"

# brightness = sbc.get_brightness()

pyautogui.FAILSAFE = False


# Dictionary to keep track of which keys are toggled (held down)
toggle_keys = {
    "Shift": False,
    "Control": False,
    "Alt": False
}

DEFAULT_POSITION = Pose()   
DEFAULT_POSITION.position.x = 0.1
DEFAULT_POSITION.position.y = 0.1
DEFAULT_POSITION.position.z = 0.1
DEFAULT_POSITION.orientation.x = 1.0
DEFAULT_POSITION.orientation.y = 0.0
DEFAULT_POSITION.orientation.z = 0.0
DEFAULT_POSITION.orientation.w = 0.0

rclpy.init()

arm_api = PlannerNode()

current_pose = Pose()
target_pose = Pose()

def login_required(f):
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if 'auth_token' not in session:
            return redirect(url_for('login'))  # Redirect to login if unauthenticated
        return f(*args, **kwargs)
    return decorated_function

@app.route('/')
@login_required
def home():
    return render_template('index.html', toggle_keys=toggle_keys)

# Endpoint to serve login page
@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        password = request.form['password']
        if password == PASSWORD:
            session['auth_token'] = 'authenticated'  # Set a session token
            return redirect(url_for('home'))  # Redirect to the main page after login
        else:
            return "Invalid password", 401  # Return an error for invalid password
    return render_template('login.html')  # Serve the login page template

@app.route('/move_mouse', methods=['POST'])
@login_required
def move_mouse():

    global current_pose, target_pose

    data = request.json
    dx = data['dx']
    dy = data['dy']
    
    # Scale the movement (adjust these values as needed)
    scale_factor = 0.0005
    dx_scaled = float(dx * scale_factor)
    dy_scaled = float(dy * scale_factor * -1)
    
    # Move the mouse relative to its current position

    target_pose.position.x += dx_scaled
    target_pose.position.y += dy_scaled
    print(f"current_pose: {target_pose}")

    return jsonify({"status": "success"})

@app.route('/click_mouse', methods=['POST'])
@login_required
def click_mouse():
    global current_pose, target_pose
    plan_resp = arm_api.plan_to_pose(target_pose)
    resp = arm_api.execute_plan()
    if plan_resp.success and resp.success:
        current_pose = target_pose
    else:
        target_pose = current_pose
    return jsonify({"status": "success"})

# Route to handle key press events
@app.route('/press_key', methods=['POST'])
@login_required
def press_key():
    key = request.form['key']

    if key == 'Off':
        keyboard.press_and_release('win+r')
        time.sleep(0.5)  # Wait for the Run dialog to appear
        keyboard.write('shutdown /h')
        keyboard.press_and_release('enter')
        return redirect(url_for('home'))

    # Handle toggleable keys (Shift, Control, Escape)
    if key in toggle_keys:
        if toggle_keys[key]:
            keyboard.release(key.lower())
            toggle_keys[key] = False
        else:
            keyboard.press(key.lower())
            toggle_keys[key] = True
    else:
        keyboard.press_and_release(key.lower())
    
    return redirect(url_for('home'))

@app.route('/get_toggle_states', methods=['GET'])
@login_required
def get_toggle_states():
    return jsonify(toggle_keys)

# Media Controls
@app.route('/play_pause', methods=['POST'])
@login_required
def play_pause():
    pyautogui.press('playpause')  # Simulate Play/Pause media key
    return redirect(url_for('home'))

@app.route('/volume_up', methods=['POST'])
@login_required
def volume_up():
    delta = 0.01
    target_pose.position.z += delta

    return jsonify({"status": "success"})

@app.route('/volume_down', methods=['POST'])
@login_required
def volume_down():
    delta = -0.01
    target_pose.position.z += delta
    return jsonify({"status": "success"})

@app.route('/brightness_up', methods=['POST'])
@login_required
def brightness_up():
    sbc.set_brightness(sbc.get_brightness()[0] + 10)
    return redirect(url_for('home'))
 
@app.route('/brightness_down', methods=['POST'])
@login_required
def brightness_down():
    sbc.set_brightness(sbc.get_brightness()[0] - 10)
    return redirect(url_for('home'))

# Route to handle text input submission
@app.route('/submit_text', methods=['POST'])
@login_required
def submit_text():
    text = request.form['text_input']

    if text.upper() in [f"F{i}" for i in range(1, 13)]:
        # Simulate pressing the function key
        keyboard.press_and_release(text.lower())
    else:
        # If Control is toggled, simulate key combinations
        if toggle_keys["Control"]:
            for char in text:
                keyboard.press('ctrl')
                keyboard.press_and_release(char)
                keyboard.release('ctrl')
        else:
            for char in text:
                keyboard.write(char)
    
    
    
    return redirect(url_for('home'))


if __name__ == '__main__':
    print("Starting server")
    current_pose = DEFAULT_POSITION
    target_pose = DEFAULT_POSITION
    arm_api.plan_to_pose(current_pose)
    arm_api.execute_plan()
    print("Moving arm to default position")
    app.run(host='0.0.0.0', port=1338)
