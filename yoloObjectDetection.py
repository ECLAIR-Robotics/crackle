import cv2
import time 
from ultralytics import YOLO

#Load the pretrained or segmented model
model = YOLO('yolov8n.pt');
#model = YOLO('yolov8n-seg.pt')

#Running inference for webcam or alternate source
#for webcam
#results = model(source = 0, show = True, conf = 0.4, save = True)
#if capturing alternate source
video_path = "/Users/kevinbabu/Documents/ECLAIR/crackle/pexels-rodnae-productions-7334739 (1080p).mp4"
cap = cv2.VideoCapture(video_path)
#video_path = 0

#Capture video properties
#cap = cv2.VideoCapture(video_path)
frame_width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
frame_height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
fps = cap.get(cv2.CAP_PROP_FPS)

# Define the codec and initialize VideoWriter
fourcc = cv2.VideoWriter_fourcc(*'mp4v')
output_video_path = "/Users/kevinbabu/Documents/ECLAIR/crackle/ObjectDetectionClips/pexels.mp4"
out = cv2.VideoWriter(output_video_path, fourcc, fps, (frame_width, frame_height))

#Run through webcam
while(cap.isOpened()):
    success, frame = cap.read()
    if not success:
        break

    start = time.perf_counter()

    # Run model
    results = model(frame)    
    annotated_frame = results[0].plot()

    # Write the processed frame to the output video file
    out.write(annotated_frame)
    # Write the processed frame to the output video file
    cv2.imshow("Yolov8 Inference", annotated_frame)

    #calculate total time
    end = time.perf_counter()
    total_time = end - start
    fps = 1 / total_time

    #Quit loop by pressing 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

#Release capture
cap.release()
out.release()
cv2.destroyAllWindows()