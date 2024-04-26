from xarm.wrapper import XArmAPI

# Create an instance of the xArmAPI object
arm = XArmAPI('192.168.1.166')
# arm.motion_enable(enable=True)
arm.set_mode(0)
# Connect to the robot
arm.connect()

# print(arm.get_position())

# Set the target position
x, y, z = 100, 200, 300  # change these values to your desired position
arm.set_position(x, y, z)

# Move the arm to the target position
# arm.move_to(x, y, z)

#arm.reset(wait=True)
# Disconnect from the robot
arm.disconnect()

"""
 * pickup_piece( str -> name of the square) -> pick up a piece from the board
 * place_piece( str -> name of the square) -> place a piece on the board
 * move_piece( str -> name of the square, str -> name of the square) -> move a piece from one square to another
 * constants: size of the board, size of the squares, size of highest piece
"""