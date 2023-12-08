# 空气键鼠系统(Air-Keyboard-and-Mouse-System)

## **简介**:
智能机器人课设
我们完成了三个功能，分别是：**动作检测模拟键盘AWSD输入**、**空气鼠标**和**空气键盘**，我们将鼠标和键盘合并成了空气键鼠系统，通过检测左手是否握拳且举过左肩来切换（左手.X<左肩.X&&左手Y>左肩.Y），左手握拳举过左肩则切换至鼠标控制；左手握拳（放开）从左键放下切换至键盘控制。

## 以下是两个压缩文件源代码：
### 1、模拟AWSD
新加文件：

    SwipeDownGesture.h
    SwipeUpGesture.h
    SwipeDownGesture.cpp
    SwipeUpGesture.cpp

改动部分代码：
    SwipeRightGesture.cpp

新加部分代码：
    KinectBody.cpp

### 2、空气键鼠系统（对模拟AWSD的代码部分进行注释）
新加文件：
    kalman.h
    kalman_filter.h
    SwipeDownGesture.h
    SwipeUpGesture.h

    SwipeDownGesture.cpp
    SwipeUpGesture.cpp

改动部分代码：
    SwipeRightGesture.cpp

新加部分代码：
    KinectBody.cpp

## **程序运行方法**：
需安装[Kinect SKD 2.0](https://www.microsoft.com/en-us/download/details.aspx?id=44561)，连接kinect2.0，开始运行程序，人物站在摄像机的拍摄范围内就可以使用，注意尽量只有一个人，减少干扰，鼠标控制范围是摄像机的右上1/4个屏幕。

## **使用方法**：
1、左手握拳从左肩下挥至左肩上，启动空气鼠标控制：右手握拳模拟鼠标右键点击一下，右手张开表示松开点击键（若要快速双击则需快速握拳张开两次）
2、左手握拳从左肩上挥至左肩下，启动空气键盘控制：键盘控制通过左右挥手实现摩斯编码记录并发出低音蜂鸣，右上向上抬起超过右肩实现完成编码，输出字母（左手向右挥动是0，即"·"；右手向左挥动是1，即"-"）并发出高音蜂鸣，若摩斯编码不符合任何一个字母，则不输出字母。

## **注意**：
鼠标控制添加了卡尔曼滤波，会有一定的累积适应期（大概30s），用右手控制鼠标尽力往屏幕中间移动，卡尔曼滤波器稳定了之后，就可以慢速稳定地移动鼠标了
若想快速移动鼠标，可以注释KinectBody.cpp中的第287行，解注释第288行，调小第253和254行的系数"500"，就完全没有卡尔曼滤波控制，控制速度会非常快，但鼠标移动会出现很明显的随机抖动，特别是在右手和身体重合了之后。