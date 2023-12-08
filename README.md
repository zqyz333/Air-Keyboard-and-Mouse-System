# Air-Keyboard-and-Mouse-System(空气键鼠系统)


## **brief introduction**:
Course Design for Intelligent Robotics（智能机器人课设）
We have completed three functions, namely: **motion detection analog keyboard AWSD input**, **air mouse**, and **air keyboard**. We merged the mouse and keyboard into an air keyboard and mouse system, which switches by detecting whether the left hand is clenched and raised over the left shoulder (left hand. X<left shoulder. X&&left hand Y>left shoulder. Y), and when the left hand is clenched and raised over the left shoulder, it switches to mouse control; Switch from left key down to keyboard control when clenching (releasing) with the left hand.

## The following are two compressed file source codes:
### 1. Simulate AWSD
New files added:

    SwipeDownGesture.h
    SwipeUpGesture.h
    SwipeDownGesture.cpp
    SwipeUpGesture.cpp

Change part of the code:

    SwipeRightGesture.cpp

New code added:

    KinectBody.cpp

### 2. Air keyboard and mouse system（Annotate the code section that simulates AWSD）
New files added:

    kalman.h
    kalman_filter.h
    SwipeDownGesture.h
    SwipeUpGesture.h
    SwipeDownGesture.cpp
    SwipeUpGesture.cpp

Change part of the code:

    SwipeRightGesture.cpp

New code added:

    KinectBody.cpp

## **Program operation method**: 
[Kinect SKD 2.0](https://www.microsoft.com/en-us/download/details.aspx?id=44561) needs to be installed, connect Kinect 2.0, start running the program, and the character can be used when standing within the shooting range of the camera. Pay attention to only one person as much as possible to reduce interference, and the mouse control range is the upper right quarter of the screen of the camera.

## **Usage**:
1. Swipe the left fist from the bottom of the left shoulder to the top of the left shoulder, activate the air mouse control: Right handed fist clenching simulation: Right click on the mouse, open the right hand to release the click button (to quickly double-click, quickly clench and open the fist twice)
2. Shake the left fist from the top of the left shoulder to the bottom of the left shoulder, activate the air keyboard control: the keyboard control records the Morse code and emits a low pitched beep by waving the hand left and right, and completes the coding by lifting it up over the right shoulder. Output the letter (swing the left hand to the right is 0, which is "·"; swing the right hand to the left is 1, which is "-") and emit a high pitched beep. If the Morse code does not match any letter, no letter will be output.

## **Note**: 
The mouse control adds Kalman filtering, which has a certain cumulative adaptation period (about 30 seconds). Use your right hand to control the mouse to move as much as possible towards the middle of the screen. Once the Kalman filter is stable, you can move the mouse slowly and steadily.
If you want to move the mouse quickly, you can annotate line 287 in KinectBody.cpp, uncomment line 288, and reduce the coefficient "500" in lines 253 and 254. There is no Kalman filter control at all, and the control speed will be very fast. However, there will be obvious random shaking when the mouse moves, especially after the right hand and body overlap.

