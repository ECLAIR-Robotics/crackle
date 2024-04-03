import sys
import os

sys.path.append(os.path.join(os.path.dirname(__file__), '..'))

from xarm.wrapper import XArmAPI

"""
 * pickup_piece( str -> name of the square) -> pick up a piece from the board
 * place_piece( str -> name of the square) -> place a piece on the board
 * move_piece( str -> name of the square, str -> name of the square) -> move a piece from one square to another
 * constants: size of the board, size of the squares, size of highest piece
"""
if __name__ == "__main__":
    
# Create an instance of the xArmAPI object
    arm = XArmAPI('192.168.1.166')
    arm.motion_enable(enable=True)
    arm.set_mode(0)
    # Connect to the robot
    arm.connect()

    print(arm.get_position())

    # Move the arm to the target position
    # arm.move_to(x, y, z)

    #arm.reset(wait=True)
    # Disconnect from the robot
    arm.disconnect()