# Face Yaw Calculation

The goal of this project is to detect the yaw (rotation angle) of a face from a given image. For testing purposes, I created a dataset of images organized into folders based on their respective yaw values. The images were generated using Blender.

You can download the dataset here:

[Download Dataset](https://drive.google.com/file/d/1qrEP8gNNz1FJivloYYybPYRdRCElaxuo/view?usp=sharing)


This project calculates the yaw of faces in images using facial landmarks. The project relies on a facial landmarks model (`shape_predictor_68_face_landmarks.dat`).

## Prerequisites

- **Operating System**: Linux or Windows with WSL
- **Python Libraries**: 
  - `dlib`
  - `numpy`
  - `ctypes`
- **C Shared Library**: `yaw_calculations_avg.so`
- **Dataset**: Images organized by yaw angles

## Setup

### 1. Install Required Libraries

To install the necessary Python dependencies:

```bash
pip install dlib numpy
```

### 2. Download and Extract the Landmark Predictor Model

Download the `shape_predictor_68_face_landmarks.dat` model:

```bash
# Download the file
wget http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2

# Extract the file
bzip2 -d shape_predictor_68_face_landmarks.dat.bz2
```

Ensure the `shape_predictor_68_face_landmarks.dat` file is accessible in your working directory, and update the path in the Python script if needed.

### 3. Download the Dataset

Download the dataset containing images organized by yaw angles from this link:

[Download Dataset](https://drive.google.com/file/d/1x5G2ZoaEReugbRIy9fWo8Hj_lI3apmml/view?usp=sharing)

After downloading, unzip it into your working directory:

```bash
unzip dataset.zip -d /path/to/extracted/dataset
```

### Dataset Structure

The dataset should be organized as follows:

```
DATASET/
├── -51/
│   ├── 1.png
│   ├── 2.png
│   └── ...
├── -29/
│   ├── 1.png
│   ├── 2.png
│   └── ...
└── ...
```

Each folder corresponds to a yaw angle, and the images are stored in those folders.

## Running the Script

### 1. Update the Paths in the Script

In the script `get_yaw.py`, ensure the following paths are correct:

- **Path to `shape_predictor_68_face_landmarks.dat`**:
  Update the path to this file in the script if necessary. By default, it's set to:
  ```python
  predictor = dlib.shape_predictor('../shape_predictor_68_face_landmarks.dat')
  ```

- **Path to an Image**:
  Update the `image_path` to point to an image from your dataset. Example:
  ```python
  image_path = './DATASET/-51/1.png'
  ```

### 2. Run the Script

Once the paths are updated, you can run the script:

```bash
python3 get_yaw.py
```

The script will detect the face in the image and print the calculated `Q` and yaw values.

## Notes

- Ensure the `yaw_calculations_avg.so` shared C library is in the same directory as the Python script. This is necessary for calculating yaw and `Q`.
- The script processes one image at a time. You can extend it to loop through all images in the dataset for batch processing.

## Troubleshooting

- **No face detected**: Make sure the face in the image is clear and not obstructed. Adjust the dataset images if needed.
- **Path issues**: Double-check that all paths to the model, images, and shared libraries are correct.

---

Feel free to clone and modify this repository for your needs. If you need to process multiple images, consider adding a loop over the dataset images to automate the process. Let me know if further improvements are required!

