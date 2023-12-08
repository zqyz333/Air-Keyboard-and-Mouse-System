#pragma once
#include <Kinect.h>

// ���ƽ������
enum _GestureParseResult
{
    // ʧ��
    Fail,

    // �ɹ�
    Succeed,

    // ����
    Pausing
};

// ���ƽ������
typedef _GestureParseResult GestureParseResult;

class Gesture
{
public:
    Gesture(void);
    ~Gesture(void);
    //************************************
    // Desc:      ���Ƽ�⣬��ͬ�����ƽ��ֲ�ͬ�Ĳ�����м�⡣
    // Method:    CheckGesture
    // FullName:  Gesture::CheckGesture
    // Access:    virtual public 
    // Returns:   GestureParseResult �����
    // Qualifier:
    // Parameter: Joint * pJoints ������
    // Parameter: int nStepIndex ��ⲽ��
    //************************************
    virtual GestureParseResult CheckGesture(Joint* pJoints, int nStepIndex) = 0;
    int GetStepCount();
protected:
    // ��ⲽ����
    int mStepCount;
};

