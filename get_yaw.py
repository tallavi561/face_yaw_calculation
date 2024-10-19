import ctypes
import dlib
import numpy as np
import time

# Load the shared C library
lib = ctypes.CDLL('./yaw_calculations_avg.so')

# Define the input and output types of the function
lib.calculate_yaw_and_Q.argtypes = [ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float)]
lib.calculate_yaw_and_Q.restype = None

# Function to get eye and nose Landmark points
def get_eye_nose_points(landmarks):
    # Right eye
    right_eye = [(landmarks.part(i).x, landmarks.part(i).y) for i in range(36, 42)]
    # Left eye
    left_eye = [(landmarks.part(i).x, landmarks.part(i).y) for i in range(42, 48)]
    # Nose
    nose = [(landmarks.part(i).x, landmarks.part(i).y) for i in range(27, 30)]

    # Convert the points to a one-dimensional float32 array
    eye_points = np.array(right_eye + left_eye, dtype=np.float32).flatten()
    nose_points = np.array(nose, dtype=np.float32).flatten()

    return eye_points, nose_points

# Function to get Q and Yaw from the C code
def get_yaw_from_c(eye_points, nose_points):
    # Variables to store the result
    Q = ctypes.c_float()
    yaw = ctypes.c_float()

    # Call the C function
    lib.calculate_yaw_and_Q(
        eye_points.ctypes.data_as(ctypes.POINTER(ctypes.c_float)),
        nose_points.ctypes.data_as(ctypes.POINTER(ctypes.c_float)),
        ctypes.byref(Q),
        ctypes.byref(yaw)
    )

    return Q.value, yaw.value

# The main function
def main(image_path):
    x0 = time.time()
    # Load the image and Landmark detector
    detector = dlib.get_frontal_face_detector()
    predictor = dlib.shape_predictor('../shape_predictor_68_face_landmarks.dat')
    x1 = time.time()
    print("detector and predictor: ", x1 - x0)
    
    # Load the image and detect faces
    img = dlib.load_rgb_image(image_path)
    faces = detector(img, 1)
    if len(faces) == 0:
        print("No face detected.")
        return

    # Detect the Landmarks on the first detected face
    landmarks = predictor(img, faces[0])
    x2 = time.time()
    print("Get landmarks: ", x2 - x1)
    
    # Get the eye and nose points
    eye_points, nose_points = get_eye_nose_points(landmarks)
    x3 = time.time()
    print("Get eyes and nose points: ", x3 - x2)
    
    # Get Q and Yaw from the C code
    Q, yaw = get_yaw_from_c(eye_points, nose_points)
    x4 = time.time()
    print("Get Q and yaw: ", x4 - x3)
    
    # Print the result
    print(f"Q: {Q}, Yaw: {yaw}")

if __name__ == "__main__":
    image_path = './DATASET/-51/1.png'  # Update with your image path
    main(image_path)
