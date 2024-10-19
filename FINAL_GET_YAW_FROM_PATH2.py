import ctypes
import dlib
import numpy as np
import time

# טען את ספריית ה-C המשותפת
lib = ctypes.CDLL('./yaw_calculations_avg.so')

# הגדר את סוגי הקלט והפלט של הפונקציה
lib.calculate_yaw_and_Q.argtypes = [ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float)]
lib.calculate_yaw_and_Q.restype = None

# פונקציה לקבלת נקודות Landmark של עיניים ואף
def get_eye_nose_points(landmarks):
    # עין ימין
    right_eye = [(landmarks.part(i).x, landmarks.part(i).y) for i in range(36, 42)]
    # עין שמאל
    left_eye = [(landmarks.part(i).x, landmarks.part(i).y) for i in range(42, 48)]
    # אף
    nose = [(landmarks.part(i).x, landmarks.part(i).y) for i in range(27, 30)]

    # המרת הנקודות למערך אחד ממדי של float32
    eye_points = np.array(right_eye + left_eye, dtype=np.float32).flatten()
    nose_points = np.array(nose, dtype=np.float32).flatten()

    return eye_points, nose_points

# פונקציה לקבלת Q ו-Yaw מה-C
def get_yaw_from_c(eye_points, nose_points):
    # משתנים לאחסון התוצאה
    Q = ctypes.c_float()
    yaw = ctypes.c_float()

    # קריאה לפונקציית ה-C
    lib.calculate_yaw_and_Q(
        eye_points.ctypes.data_as(ctypes.POINTER(ctypes.c_float)),
        nose_points.ctypes.data_as(ctypes.POINTER(ctypes.c_float)),
        ctypes.byref(Q),
        ctypes.byref(yaw)
    )

    return Q.value, yaw.value

# הפונקציה הראשית
def main(image_path):
    x0 =  time.time()
    # טען את התמונה והסריקה של Landmark
    detector = dlib.get_frontal_face_detector()
    predictor = dlib.shape_predictor('../shape_predictor_68_face_landmarks.dat')
    x1 = time.time()
    print("detector and predictor: ", x1 - x0)
    # קריאה לתמונה ואיתור פנים
    img = dlib.load_rgb_image(image_path)
    faces = detector(img, 1)
    if len(faces) == 0:
        print("No face detected.")
        return

    # איתור ה-Landmarks בפנים הראשונות שזוהו
    landmarks = predictor(img, faces[0])
    x2 = time.time()
    print("Get landmarks: ", x2 - x1)
    # קבלת הנקודות של העיניים והאף
    eye_points, nose_points = get_eye_nose_points(landmarks)
    x3 = time.time()
    print("Get eyes and nose points: ", x3 - x2)
    # קבלת Q ו-Yaw מה-C
    Q, yaw = get_yaw_from_c(eye_points, nose_points)
    x4 = time.time()
    print("Get Q and yaw: ", x4 - x3)
    # הדפסת התוצאה
    print(f"Q: {Q}, Yaw: {yaw}")

if __name__ == "__main__":
    image_path = '../NEW/-51/1.png' # עדכן את הנתיב לתמונה שלך
    main(image_path)
