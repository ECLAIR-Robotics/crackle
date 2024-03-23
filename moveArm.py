from xarm.wrapper import XArmAPI

print("initializing")
arm = XArmAPI('192.168.1.166')  # Change the port accordingly
arm.motion_enable(enable=True)
arm.set_mode(0)
    # Connect to the arm
arm.connect()

    # Move to the specified coordinates
arm.set_position(150, 0, 90)

    # Disconnect from the arm
arm.disconnect()