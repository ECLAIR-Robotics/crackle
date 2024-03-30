from xarm.wrapper import XArmAPI

print("initializing")
arm = XArmAPI('192.168.1.166')  # Change the port accordingly
arm.motion_enable(enable=True)
arm.set_mode(0)
    # Connect to the arm
arm.connect()

    # Move to the specified coordinates
# arm.set_position(180, 50, 154, roll=4.3, pitch=180, yaw=0, wait=True, speed=50, mvacc=1000, relative=False, is_radian=False)
print(arm.get_gripper_position())
    # Disconnect from the arm
