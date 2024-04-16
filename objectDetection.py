from ultralytics import YOLO
import time

# Load the pre-trained model
model = YOLO('yolov8n-seg.pt')

# Initialize the source, could be an image, video file, directory, or webcam
source = 0  # For webcam

# Performance Metrics Initialization
start_time = time.time()

roi_coords = (100, 100, 300, 300)

# Run inference and display results
results = model(source=source, show=True, conf=0.4, save=True)

# Calculate and display the inference time
end_time = time.time()
print(f"Inference time: {end_time - start_time:.2f} seconds")

# Accessing and printing specific metrics like precision or recall if needed
# This assumes model has been validated and these metrics are available
if hasattr(results, 'metrics'):
    print(f"Precision: {results.metrics.precision}")
    print(f"Recall: {results.metrics.recall}")
