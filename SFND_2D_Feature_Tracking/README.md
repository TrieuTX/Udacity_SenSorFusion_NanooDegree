# SFND 2D Feature Tracking

<img src="images/keypoints.png" width="820" height="248" />

The idea of the camera course is to build a collision detection system - that's the overall goal for the Final Project. As a preparation for this, you will now build the feature tracking part and test various detector / descriptor combinations to see which ones perform best. This mid-term project consists of four parts:

* First, you will focus on loading images, setting up data structures and putting everything into a ring buffer to optimize memory load. 
* Then, you will integrate several keypoint detectors such as HARRIS, FAST, BRISK and SIFT and compare them with regard to number of keypoints and speed. 
* In the next part, you will then focus on descriptor extraction and matching using brute force and also the FLANN approach we discussed in the previous lesson. 
* In the last part, once the code framework is complete, you will test the various algorithms in different combinations and compare them with regard to some performance measures. 

See the classroom instruction and code comments for more details on each of these parts. Once you are finished with this project, the keypoint matching part will be set up and you can proceed to the next lesson, where the focus is on integrating Lidar points and on object detection using deep-learning. 

## Dependencies for Running Locally
1. cmake >= 2.8
 * All OSes: [click here for installation instructions](https://cmake.org/install/)

2. make >= 4.1 (Linux, Mac), 3.81 (Windows)
 * Linux: make is installed by default on most Linux distros
 * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
 * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)

3. OpenCV >= 4.1
 * All OSes: refer to the [official instructions](https://docs.opencv.org/master/df/d65/tutorial_table_of_content_introduction.html)
 * This must be compiled from source using the `-D OPENCV_ENABLE_NONFREE=ON` cmake flag for testing the SIFT and SURF detectors. If using [homebrew](https://brew.sh/): `$> brew install --build-from-source opencv` will install required dependencies and compile opencv with the `opencv_contrib` module by default (no need to set `-DOPENCV_ENABLE_NONFREE=ON` manually). 
 * The OpenCV 4.1.0 source code can be found [here](https://github.com/opencv/opencv/tree/4.1.0)

4. gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using either [MinGW-w64](http://mingw-w64.org/doku.php/start) or [Microsoft's VCPKG, a C++ package manager](https://docs.microsoft.com/en-us/cpp/build/install-vcpkg?view=msvc-160&tabs=windows). VCPKG maintains its own binary distributions of OpenCV and many other packages. To see what packages are available, type `vcpkg search` at the command prompt. For example, once you've _VCPKG_ installed, you can install _OpenCV 4.1_ with the command:
```bash
c:\vcpkg> vcpkg install opencv4[nonfree,contrib]:x64-windows
```
Then, add *C:\vcpkg\installed\x64-windows\bin* and *C:\vcpkg\installed\x64-windows\debug\bin* to your user's _PATH_ variable. Also, set the _CMake Toolchain File_ to *c:\vcpkg\scripts\buildsystems\vcpkg.cmake*.


## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./2D_feature_tracking`.
#### **TASK MP.7**
Your seventh task is to count the number of keypoints on the preceding vehicle for all 10 images and take note of the distribution of their neighborhood size. Do this for all the detectors you have implemented.

| Detector      | Number of Keypoints        |
| ------------- | -------------------------- |
| **SHITOMASI** | 1179                       |
| **HARRIS**    | 248                        |
| **FAST**      | 1491                       |
| **BRISK**     | 2762                       |
| **ORB**       | 1161                       |
| **AKAZE**     | 1670                       |
| **SIFT**      | 1386                       |

#### **TASK MP.8**
Your eighth task is to count the number of matched keypoints for all 10 images using all possible combinations of detectors and descriptors. In the matching step, use the BF approach with the descriptor distance ratio set to 0.8.

| Detector\Descriptor | BRISK | BRIEF | ORB | FREAK | AKAZE | SIFT |
| --- | --- | --- |--- |--- |--- |--- |
| **SHITOMASI** | 767 | 944 |907|768|N/A|N/A|
| **HARRIS** | 142|173|160|144|N/A|N/A|
| **FAST** | 899 |1099|1081|878|N/A|N/A|
| **BRISK** | 1570 |1704|1510|1524|N/A|N/A|
| **ORB** | 751 |545|761|420|N/A|N/A|
| **AKAZE** | 1215 |1266|1186|1187|1259|N/A|
| **SIFT** | 592 |702|N/A|593|N/A|N/A|

#### **TASK MP.9**
Your ninth task is to log the time it takes for keypoint detection and descriptor extraction. The results must be entered into a spreadsheet and based on this information you will then suggest the TOP3 detector / descriptor combinations as the best choice for our purpose of detecting keypoints on vehicles. Finally, in a short text, please justify your recommendation based on your observations and on the data you collected.

| Detector\Descriptor | BRISK | BRIEF | ORB | FREAK | AKAZE | SIFT |
| --- | --- | --- |--- |--- |--- |--- |
| **SHITOMASI** | 144.301 |145.187 |146.714|508.543|N/A|N/A|
| **HARRIS** | 145.435|163.349|179.998|403.676| N/A |N/A|
| **FAST** | 24.1113 |15.6447|41.0496|324.599|N/A|N/A|
| **BRISK** | 373.145 |347.023|434.01|620.562|N/A|N/A|
| **ORB** | 82.7799|68.3624|163.302|348.231|N/A|N/A|
| **AKAZE** | 539.141 |511.956|682.993|786.982|1017.64|N/A|
| **SIFT** | 1100.64 |1122.2|N/A|1458.2|N/A|N/A|

#### **Top Three Detector/Descriptor Combinations:**
Detector/Descriptor  | Number of Matched Keypoints | Processing Time |
-------------------- | --------------------------- | --------------- |
**FAST+BRIEF**           | 1099 keypoints              | 15.6447 ms      |
**FAST+BRISK**           | 899 keypoints               | 24.1113 ms      |
**FAST+ORB**             | 1081 keypoints              | 41.0496 ms      |