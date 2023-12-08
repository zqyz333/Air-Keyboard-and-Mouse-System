#pragma once
#include "Gesture.h"
#include "SwipeRightGesture.h"
#include "SwipeLeftGesture.h"
#include "SwipeUpGesture.h"
#include "SwipeDownGesture.h"

enum _GestureType
{
    None,

    // ���Ұ�
    SwipeRight,

    // �����
    SwipeLeft,

	// ���ϰ�
    SwipeUp,

	// ���°�
    SwipeDown,
};

// ��������
typedef _GestureType GestureType;

// ����ʶ�����ص���
typedef void(CALLBACK *GestureRecognized)(GestureType type);

// ��װ����ʶ��
class GestureWrapper
{
public:
    GestureWrapper(GestureType gestureType);
    ~GestureWrapper(void);


    //************************************
    // Desc:      ��������
    // Method:    UpdateGesture
    // FullName:  GestureWrapper::UpdateGesture
    // Access:    public 
    // Returns:   void
    // Qualifier:
    // Parameter: Joint * pJoints ������
    //************************************
    void UpdateGesture(Joint* pJoints);

    
    //************************************
    // Desc:      ����
    // Method:    Reset
    // FullName:  GestureWrapper::Reset
    // Access:    public 
    // Returns:   void
    // Qualifier:
    //************************************
    void Reset();

    // ����ʶ��ص�������
    GestureRecognized fGestureRecognized;
private:
    // ��������
    GestureType mGestureType;

    // ����ʶ��
    Gesture* pGesture;

    // ����������
    int mStepIndex;

    // �����Ϊ����ʱ��ͣ����֡����
    int mPuasedFrameCount;

    // ֡��
    int mFrameCount;

    // �Ƿ��ڴ���״̬
    bool bPaused;
};

