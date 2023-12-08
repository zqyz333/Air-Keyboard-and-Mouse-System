#pragma once
#include "Gesture.h"

// 左手向下摆手手势
class SwipeDownGesture : public Gesture
{
public:
    SwipeDownGesture(void);
    ~SwipeDownGesture(void);

    //************************************
    // Desc:      检测手势。向下摆手手势，分三步检测。
    // Method:    CheckGesture
    // FullName:  SwipeDownGesture::CheckGesture
    // Access:    public 
    // Returns:   GestureParseResult
    // Qualifier:
    // Parameter: Joint * pJoints 骨骼点
    // Parameter: int nStepIndex 检测步骤
    //************************************
    GestureParseResult CheckGesture(Joint* pJoints, int nStepIndex);

private:
    //************************************
    // Desc:      检测步骤1。左手在左（肩+臀）/2之上。
    // Method:    Step1
    // FullName:  SwipeDownGesture::Step1
    // Access:    private 
    // Returns:   GestureParseResult 检测结果
    // Qualifier:
    // Parameter: Joint * pJoints 骨骼点
    //************************************
    GestureParseResult Step1(Joint* pJoints);

    //************************************
    // Desc:      检测步骤2。左手在左（肩+臀）/2之上。
    // Method:    Step2
    // FullName:  SwipeDownGesture::Step2
    // Access:    private 
    // Returns:   GestureParseResult 检测结果
    // Qualifier:
    // Parameter: Joint * pJoints 骨骼点
    //************************************
    GestureParseResult Step2(Joint* pJoints);

    //************************************
    // Desc:      检测步骤3.左手在左（肩+臀）/2之下。
    // Method:    Step3
    // FullName:  SwipeDownGesture::Step3
    // Access:    private 
    // Returns:   GestureParseResult 检测结果
    // Qualifier:
    // Parameter: Joint * pJoints 骨骼点
    //************************************
    GestureParseResult Step3(Joint* pJoints);
};

