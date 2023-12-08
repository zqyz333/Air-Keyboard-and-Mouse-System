#pragma once
#include "Gesture.h"

// �������ϰ�������
class SwipeUpGesture : public Gesture
{
public:
    SwipeUpGesture(void);
    ~SwipeUpGesture(void);

    //************************************
    // Desc:      ������ơ����ϰ������ƣ���������⡣
    // Method:    CheckGesture
    // FullName:  SwipeUpGesture::CheckGesture
    // Access:    public 
    // Returns:   GestureParseResult
    // Qualifier:
    // Parameter: Joint * pJoints ������
    // Parameter: int nStepIndex ��ⲽ��
    //************************************
    GestureParseResult CheckGesture(Joint* pJoints, int nStepIndex);

private:
    //************************************
    // Desc:      ��ⲽ��1���������Ҽ���²ࡣ
    // Method:    Step1
    // FullName:  SwipeUpGesture::Step1
    // Access:    private 
    // Returns:   GestureParseResult �����
    // Qualifier:
    // Parameter: Joint * pJoints ������
    //************************************
    GestureParseResult Step1(Joint* pJoints);

    //************************************
    // Desc:      ��ⲽ��2���������Ҽ���Ҳࡣ
    // Method:    Step2
    // FullName:  SwipeUpGesture::Step2
    // Access:    private 
    // Returns:   GestureParseResult �����
    // Qualifier:
    // Parameter: Joint * pJoints ������
    //************************************
    GestureParseResult Step2(Joint* pJoints);

    //************************************
    // Desc:      ��ⲽ��3.����λ���Ҽ���ϲࡣ
    // Method:    Step3
    // FullName:  SwipeUpGesture::Step3
    // Access:    private 
    // Returns:   GestureParseResult �����
    // Qualifier:
    // Parameter: Joint * pJoints ������
    //************************************
    GestureParseResult Step3(Joint* pJoints);
};

