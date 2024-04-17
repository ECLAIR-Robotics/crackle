# imports 
import numpy as np 
import cv2 as cv 
import glob 
import robotpy_apriltag
  
# termination criteria 
criteria = (cv.TERM_CRITERIA_EPS + cv.TERM_CRITERIA_MAX_ITER, 30, 0.001) 

# Real world coordinates of circular grid 
obj3d = np.zeros((44, 3), np.float32) 
# As the actual circle size is not required, 
# the z-coordinate is zero and the x and y coordinates are random numbers. 
a = [0, 36, 72, 108, 144, 180, 216, 252, 288, 324, 360] 
b = [0, 72, 144, 216, 36, 108, 180, 252] 
for i in range(0, 44): 
    obj3d[i] = (a[i // 4], (b[i % 8]), 0) 
    # print(objp[i]) 
  
# Vector to store 3D points 
obj_points = [] 
# Vector to store 2D points 
img_points = [] 
  
# Extracting path of individual image stored in a given directory 
# images = glob.glob('./Images/*.png') 
cap = cv.VideoCapture(0)
tag = robotpy_apriltag.AprilTagDetector()
tag.addFamily("tag36h11", 3)
while True: 
    # Loading image 
    ret, img = cap.read() 
    img = cv.resize(img, (640, 480))
    
    # Conversion to grayscale image 
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY) 
    l = tag.detect(gray)
    # To find the position of circles in the grid pattern 
    corners = [(0, 0), (0, 0), (0, 0), (0, 0)]
    for i in l: 
        for j in range(len(corners)):
                            corners[j] = (int(i.getCorner(j).x), int(i.getCorner(j).y))
    print(corners)                    
    ret = True
  
    # If true is returned,  
    # then 3D and 2D vector points are updated and corner is drawn on image 
    if ret == True: 
        obj_points.append(obj3d) 
  
        # corners2 = cv.cornerSubPix(gray, corners, (11, 11), (-1, -1), criteria) 
        # In case of circular grids,  
        # the cornerSubPix() is not always needed, so alternative method is: 
        # corners2 = corners 
        img_points.append(corners) 
  
        # Drawing the corners, saving and displaying the image 
        # cv.drawChessboardCorners(img, (4, 11), np.float32(corners), ret) 
        print("img_points: ", img_points)
        """Camera calibration:  
        Passing the value of known 3D points (obj points) and the corresponding pixel coordinates  
        of the detected corners (img points)"""
        ret, camera_mat, distortion, rotation_vecs, translation_vecs = cv.calibrateCamera( 
            obj_points, img_points[0], gray.shape[::-1], None, None) 
  
        print("Error in projection : \n", ret) 
        print("\nCamera matrix : \n", camera_mat) 
        print("\nDistortion coefficients : \n", distortion) 
        print("\nRotation vector : \n", rotation_vecs) 
        print("\nTranslation vector : \n", translation_vecs)
        # cv.imwrite('output.jpg', img) #To save corner-drawn image 
        
    cv.imshow('img', img) 
    if cv.waitKey(1) & 0xFF == ord("q"):
        break
        
# Release the capture object and close the window
cap.release()
cv.destroyAllWindows()
  
