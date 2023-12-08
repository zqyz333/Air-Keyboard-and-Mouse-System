#pragma once
#include "Gesture.h"

/// <summary>
/// �������Ұ�������
/// </summary>
class SwipeRightGesture : public Gesture
{
public:
    SwipeRightGesture(void);
    ~SwipeRightGesture(void);

    //************************************
    // Desc:      ������ơ����Ұ������ƣ���������⡣
    // Method:    CheckGesture
    // FullName:  SwipeRightGesture::CheckGesture
    // Access:    public 
    // Returns:   GestureParseResult
    // Qualifier:
    // Parameter: Joint * pJoints ������
    // Parameter: int nStepIndex ��ⲽ��
    //************************************
    GestureParseResult CheckGesture(Joint* pJoints, int nStepIndex);
private:
    //************************************
    // Desc:      ��ⲽ��1��������������ࡣ
    // Method:    Step1
    // FullName:  SwipeRightGesture::Step1
    // Access:    private 
    // Returns:   GestureParseResult �����
    // Qualifier:
    // Parameter: Joint * pJoints ������
    //************************************
    GestureParseResult Step1(Joint* pJoints);

    //************************************
    // Desc:      ��ⲽ��2�������������Ҳ࣬�����Ҽ����ࡣ������֮�䣩
    // Method:    Step2
    // FullName:  SwipeRightGesture::Step2
    // Access:    private 
    // Returns:   GestureParseResult �����
    // Qualifier:
    // Parameter: Joint * pJoints ������
    //************************************
    GestureParseResult Step2(Joint* pJoints);

    //************************************
    // Desc:      ��ⲽ��3.����λ���Ҽ���Ҳࡣ
    // Method:    Step3
    // FullName:  SwipeRightGesture::Step3
    // Access:    private 
    // Returns:   GestureParseResult �����
    // Qualifier:
    // Parameter: Joint * pJoints ������
    //************************************
    GestureParseResult Step3(Joint* pJoints);
};

