import ctypes
import dlib
import numpy as np




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

    # Load the shared C library
    lib = ctypes.CDLL('./yaw_calculations_avg.so')

    # Define the input and output types of the function
    lib.calculate_yaw_and_Q.argtypes = [ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float)]
    lib.calculate_yaw_and_Q.restype = None

    # Call the C function
    lib.calculate_yaw_and_Q(
        eye_points.ctypes.data_as(ctypes.POINTER(ctypes.c_float)),
        nose_points.ctypes.data_as(ctypes.POINTER(ctypes.c_float)),
        ctypes.byref(Q),
        ctypes.byref(yaw)
    )

    return Q.value, yaw.value

# The main function
def get_yaw_from_landmarks(landmarks) -> float:

    # Get the eye and nose points
    eye_points, nose_points = get_eye_nose_points(landmarks)
    
    # Get Q and Yaw from the C code
    Q, yaw = get_yaw_from_c(eye_points, nose_points)
    
    # Print the result
    print(f"Q: {Q}, Yaw: {yaw}")
    return yaw

