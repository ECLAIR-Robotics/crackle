import robotpy_apriltag
import cv2
import numpy as np

cap = cv2.VideoCapture(0)
tag = robotpy_apriltag.AprilTagDetector()
tag.addFamily("tag36h11", 3)

WIDTH = 640
HEIGHT = 480
while True:
    # Capture frame-by-frame
    ret, frame = cap.read()
    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    frame = cv2.resize(frame, (640, 480))

    l: robotpy_apriltag.AprilTagDetection = tag.detect(frame)
    if l is not None:
        for i in l:
            # print(i.getCenter())
            point = i.getCenter()
            x: int = int(point.x)
            y: int = int(point.y)

            point_tl = (int(i.getCorner(0).x), int(i.getCorner(0).y))
            point_br = (int(i.getCorner(2).x), int(i.getCorner(2).y))

            cv2.rectangle(frame, point_tl, point_br, (0, 0, 255), thickness=2)
            
            # points = i.getCorners()
            # rect = cv2.minAreaRect(points)
            # box = cv2.boxPoints(rect)
            
            # cv2.drawContours(frame, [box], 0, (0, 255, 0), 2)
            
            cv2.line(frame, (x, 0), (x, HEIGHT), (0, 255, 0), thickness=2)
            cv2.line(frame, (0, y), (WIDTH, y), (0, 255, 0), thickness=2)

    # Display the frame
    cv2.imshow("Camera Feed", frame)

    # Break the loop if 'q' key is pressed
    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

# Release the capture object and close the window
cap.release()
cv2.destroyAllWindows()

class AprilTage:
    def __init__(self) -> None:
        self.width = 640
        self.height = 480
        
        self.cap = cv2.VideoCapture(0)
        self.tag = robotpy_apriltag.AprilTagDetector()
        tag.addFamily("tag36h11", 3)
        
    def detector():
        while True:
             # Capture frame-by-frame
             ret, frame = cap.read()
             frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
             frame = cv2.resize(frame, (640, 480))

             l: robotpy_apriltag.AprilTagDetection = tag.detect(frame)
             if l is not None:
                for i in l:
                    # print(i.getCenter())
                    point = i.getCenter()
                    x: int = int(point.x)
                    y: int = int(point.y)

                    point_tl = (int(i.getCorner(0).x), int(i.getCorner(0).y))
                    point_br = (int(i.getCorner(2).x), int(i.getCorner(2).y))

                    cv2.rectangle(frame, point_tl, point_br, (0, 0, 255), thickness=2)
            
                     # points = i.getCorners()
                     # rect = cv2.minAreaRect(points)
                     # box = cv2.boxPoints(rect)
                        
                     # cv2.drawContours(frame, [box], 0, (0, 255, 0), 2)
                        
                    cv2.line(frame, (x, 0), (x, HEIGHT), (0, 255, 0), thickness=2)
                    cv2.line(frame, (0, y), (WIDTH, y), (0, 255, 0), thickness=2)

                    # Display the frame
                    cv2.imshow("Camera Feed", frame)

                    # Break the loop if 'q' key is pressed
                    if cv2.waitKey(1) & 0xFF == ord("q"):
                        break
        
    
