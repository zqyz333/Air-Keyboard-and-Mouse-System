#pragma once
#include "Gesture.h"

// �������°�������
class SwipeDownGesture : public Gesture
{
public:
    SwipeDownGesture(void);
    ~SwipeDownGesture(void);

    //************************************
    // Desc:      ������ơ����°������ƣ���������⡣
    // Method:    CheckGesture
    // FullName:  SwipeDownGesture::CheckGesture
    // Access:    public 
    // Returns:   GestureParseResult
    // Qualifier:
    // Parameter: Joint * pJoints ������
    // Parameter: int nStepIndex ��ⲽ��
    //************************************
    GestureParseResult CheckGesture(Joint* pJoints, int nStepIndex);

private:
    //************************************
    // Desc:      ��ⲽ��1���������󣨼�+�Σ�/2֮�ϡ�
    // Method:    Step1
    // FullName:  SwipeDownGesture::Step1
    // Access:    private 
    // Returns:   GestureParseResult �����
    // Qualifier:
    // Parameter: Joint * pJoints ������
    //************************************
    GestureParseResult Step1(Joint* pJoints);

    //************************************
    // Desc:      ��ⲽ��2���������󣨼�+�Σ�/2֮�ϡ�
    // Method:    Step2
    // FullName:  SwipeDownGesture::Step2
    // Access:    private 
    // Returns:   GestureParseResult �����
    // Qualifier:
    // Parameter: Joint * pJoints ������
    //************************************
    GestureParseResult Step2(Joint* pJoints);

    //************************************
    // Desc:      ��ⲽ��3.�������󣨼�+�Σ�/2֮�¡�
    // Method:    Step3
    // FullName:  SwipeDownGesture::Step3
    // Access:    private 
    // Returns:   GestureParseResult �����
    // Qualifier:
    // Parameter: Joint * pJoints ������
    //************************************
    GestureParseResult Step3(Joint* pJoints);
};

