import robotpy_apriltag
import cv2
import numpy as np
from typing import List, Tuple
import glob
import detect_apriltag

def main():
    print('Hi from pkg_photo.')
    april_tag_detector = detect_apriltag.AprilTags()
    print(april_tag_detector.WIDTH)


if __name__ == '__main__':
    main()
