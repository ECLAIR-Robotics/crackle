from ultralytics import YOLO
import cv2
from ultralytics.utils.plotting import Annotator

model = YOLO('yolov8n.pt')
cap = cv2.VideoCapture(0)
cap.set(3, 640)
cap.set(4, 480)

# Initialize a list to store bounding boxes
bounding_boxes = []

while True:
    _, img = cap.read()
    results = model.predict(img)

    for r in results:
        annotator = Annotator(img)
        boxes = r.boxes

        for box in boxes:
            b = box.xyxy[0]  # get box coordinates
            c = box.cls
            # Append bounding box coordinates and class to the list
            bounding_boxes.append((b, model.names[int(c)]))
            annotator.box_label(b, model.names[int(c)])
            # Now `bounding_boxes` contains all the bounding boxes and their classes
            print(bounding_boxes)


    img = annotator.result()  
    cv2.imshow('YOLO V8 Detection', img)

    if cv2.waitKey(1) & 0xFF == ord(' '):
        break

    

cap.release()
cv2.destroyAllWindows()
