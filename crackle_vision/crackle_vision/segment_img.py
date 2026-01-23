# segment_img.py
# modified 3/31/2025
# next time: call this function in the code!! getting close!! then test

# Uses SAM, Segment anything to find the point to pick up an object
# from https://github.com/facebookresearch/segment-anything
# guide: https://blog.roboflow.com/how-to-use-segment-anything-model-sam/

# You will need:
# pip install git+https://github.com/facebookresearch/segment-anything.git
# https://dl.fbaipublicfiles.com/segment_anything/sam_vit_h_4b8939.pth
# ^ It's BIG! 2.5 GB! ^ this downloads a 'model checkpoint', needed to run sam.

import torch
from segment_anything import sam_model_registry
import os
from os import access, R_OK
from os.path import isfile
import cv2
from segment_anything import SamPredictor
import numpy as np
from cv_bridge import CvBridge

def get_object_points(image_msg, bounding_box):
  """
  Create a mask for the object in the bounding box using SAM. returns a 
  list of all these points.
  
  Args:
      image_msg (sensor_msgs.msg.Image): ROS Image message.
      bounding_box (tuple): Bounding box (x, y, width, height).
  
  Returns:
      np.ndarray: Mask for the object in the bounding box. A list of (x, y) pairs.
  """
  # setup the model - find where the model is stored

  HOME = "/home/tanay/crackle_ws/src/crackle/crackle_vision/"

  bridge = CvBridge()

  DEVICE = torch.device('cuda:0' if torch.cuda.is_available() else 'cpu')
  MODEL_TYPE = "vit_h"
  CHECKPOINT_PATH = os.path.join(HOME, "data/sam_vit_h_4b8939.pth")
  assert isfile(CHECKPOINT_PATH) and access(CHECKPOINT_PATH, R_OK), \
        f"File {CHECKPOINT_PATH} doesn't exist or isn't readable"

  # open the model, code from https://blog.roboflow.com/how-to-use-segment-anything-model-sam/
  sam = sam_model_registry[MODEL_TYPE](checkpoint=CHECKPOINT_PATH)
  sam.to(device=DEVICE)
  print("done opening model")
  
  # Convert ROS Image message to OpenCV image
  # np_arr = np.frombuffer(image_msg.data, np.uint8)
  # cv_image = cv2.imdecode(np_arr, cv2.IMREAD_COLOR)

  cv_image = bridge.imgmsg_to_cv2(image_msg, desired_encoding="bgr8")

  print("cv image: ", cv_image)
  # Set the image for the predictor
  mask_generator = SamPredictor(sam) # use SamPredictor because it takes in a bounding box
  print("done creating mask generator")
  print(mask_generator)

  mask_generator.set_image(cv_image)
  
  # Create the mask
  x, y, width, height = bounding_box # Get the bounding box coordinates
  masks = mask_generator.predict(box=np.array([x, y, x + width, y + height]))
  print("masks: ", masks)  

  # Ensure there's only one mask (from one object)
  mask = masks[0]
  # assert len(masks) == 1, "Expected exactly one mask, but found multiple. This means there was multiple objects detected within the bounding box" 

  # Convert a mask to a list of all points
  # Get the coordinates of all non-zero pixels (the pixels that are part of the object)
  points = np.column_stack(np.where(mask > 0))
  
  # Convert to list of (x, y) pairs
  points = [tuple(point) for point in points]
#   print("end of get_object_points, resulting array is size: ", points.length, " first elem is", points[0])




  return points

def get_center(points):
  """
  Calculate the center of mass of a list of points.
  
  Args:
      points (list): List of points (x, y).
  
  Returns:
      tuple: Centroid point (x, y).
  """
  points_array = np.array(points)
  centroid = points_array.mean(axis=0)
  return tuple(centroid)
