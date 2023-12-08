#pragma once
#include "Gesture.h"

// 右手向上摆手手势
class SwipeUpGesture : public Gesture
{
public:
    SwipeUpGesture(void);
    ~SwipeUpGesture(void);

    //************************************
    // Desc:      检测手势。向上摆手手势，分三步检测。
    // Method:    CheckGesture
    // FullName:  SwipeUpGesture::CheckGesture
    // Access:    public 
    // Returns:   GestureParseResult
    // Qualifier:
    // Parameter: Joint * pJoints 骨骼点
    // Parameter: int nStepIndex 检测步骤
    //************************************
    GestureParseResult CheckGesture(Joint* pJoints, int nStepIndex);

private:
    //************************************
    // Desc:      检测步骤1。右手在右肩膀下侧。
    // Method:    Step1
    // FullName:  SwipeUpGesture::Step1
    // Access:    private 
    // Returns:   GestureParseResult 检测结果
    // Qualifier:
    // Parameter: Joint * pJoints 骨骼点
    //************************************
    GestureParseResult Step1(Joint* pJoints);

    //************************************
    // Desc:      检测步骤2。右手在右肩膀右侧。
    // Method:    Step2
    // FullName:  SwipeUpGesture::Step2
    // Access:    private 
    // Returns:   GestureParseResult 检测结果
    // Qualifier:
    // Parameter: Joint * pJoints 骨骼点
    //************************************
    GestureParseResult Step2(Joint* pJoints);

    //************************************
    // Desc:      检测步骤3.右手位于右肩膀上侧。
    // Method:    Step3
    // FullName:  SwipeUpGesture::Step3
    // Access:    private 
    // Returns:   GestureParseResult 检测结果
    // Qualifier:
    // Parameter: Joint * pJoints 骨骼点
    //************************************
    GestureParseResult Step3(Joint* pJoints);
};

