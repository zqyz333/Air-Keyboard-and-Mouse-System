#include "stdafx.h"
#include "SwipeRightGesture.h"


SwipeRightGesture::SwipeRightGesture(void) : Gesture()
{
    mStepCount = 3;
}


SwipeRightGesture::~SwipeRightGesture(void)
{
}

//************************************
    // Desc:      ������ơ����Ұ������ƣ�����������
    // Method:    CheckGesture
    // FullName:  SwipeRightGesture::CheckGesture
    // Access:    public 
    // Returns:   GestureParseResult
    // Qualifier:
    // Parameter: Joint * pJoints ������
    // Parameter: int nStepIndex ��ⲽ��
    //************************************
GestureParseResult SwipeRightGesture::CheckGesture(Joint* pJoints, int nStepIndex)
{
    switch (nStepIndex)
    {
    case 0:
        return Step1(pJoints);
    case 1:
        return Step2(pJoints);
    case 2:
        return Step3(pJoints);
    default:
        break;
    }
    return GestureParseResult::Fail;
}

//************************************
// Desc:      ��ⲽ��1��������������ࡣ
// Method:    Step1
// FullName:  SwipeRightGesture::Step1
// Access:    private 
// Returns:   GestureParseResult �����
// Qualifier:
// Parameter: Joint * pJoints ������
//************************************
GestureParseResult SwipeRightGesture::Step1(Joint* pJoints)
{
    Joint handLeft = pJoints[JointType_HandLeft];
    Joint handRight = pJoints[JointType_HandRight];
    Joint elbowLeft = pJoints[JointType_ElbowLeft];
    Joint shoulderLeft = pJoints[JointType_ShoulderLeft];
    Joint head = pJoints[JointType_Head];
    Joint spineBase = pJoints[JointType_SpineBase];
	Joint spineShoulder = pJoints[JointType_SpineShoulder];

    // �������ⲿ֮ǰ�����ִ���
    if (handLeft.Position.Z < elbowLeft.Position.Z &&
        handRight.Position.Y < spineShoulder.Position.Y)
    {
        // ���ֵ���ͷ�����������β�
        if (//handLeft.Position.Y  < head.Position.Y &&
            handLeft.Position.Y > spineBase.Position.Y)
        {
            // ����λ�ڼ�����
            if (handLeft.Position.X < shoulderLeft.Position.X)
            {
                return GestureParseResult::Succeed;
            }

            return GestureParseResult::Pausing;
        }

    }

    return GestureParseResult::Fail;
}

GestureParseResult SwipeRightGesture::Step2(Joint* pJoints)
{
    Joint handLeft = pJoints[JointType_HandLeft];
    Joint handRight = pJoints[JointType_HandRight];
    Joint elbowLeft = pJoints[JointType_ElbowLeft];
    Joint shoulderLeft = pJoints[JointType_ShoulderLeft];
    Joint shoulderRight = pJoints[JointType_ShoulderRight];
    Joint head = pJoints[JointType_Head];
    Joint spineBase = pJoints[JointType_SpineBase];
    Joint spineShoulder = pJoints[JointType_SpineShoulder];

    // �������ⲿ֮ǰ�����ִ���
    if (handLeft.Position.Z < elbowLeft.Position.Z &&
        handRight.Position.Y < spineShoulder.Position.Y)
    {
        // ���ֵ���ͷ�����������β�
        if (//handLeft.Position.Y < head.Position.Y &&
            handLeft.Position.Y > spineBase.Position.Y)
        {
            // ����λ�������Ҳ࣬��λ���Ҽ�����
            if (handLeft.Position.X < shoulderRight.Position.X &&
                //handLeft.Position.X > shoulderLeft.Position.X)
                    handLeft.Position.X > spineShoulder.Position.X)
            {
                return GestureParseResult::Succeed;
            }

            return GestureParseResult::Pausing;
        }
    }

    return GestureParseResult::Fail;
}

//************************************
// Desc:      ��ⲽ��3.����λ���Ҽ���Ҳࡣ
// Method:    Step3
// FullName:  SwipeRightGesture::Step3
// Access:    private 
// Returns:   GestureParseResult �����
// Qualifier:
// Parameter: Joint * pJoints ������
//************************************
GestureParseResult SwipeRightGesture::Step3(Joint* pJoints)
{
    Joint handLeft = pJoints[JointType_HandLeft];
    Joint handRight = pJoints[JointType_HandRight];
    Joint elbowLeft = pJoints[JointType_ElbowLeft];
    Joint shoulderRight = pJoints[JointType_ShoulderRight];
    Joint head = pJoints[JointType_Head];
    Joint spineBase = pJoints[JointType_SpineBase];
    Joint spineShoulder = pJoints[JointType_SpineShoulder];

    // �������ⲿ֮ǰ�����ִ���
    if (handLeft.Position.Z < elbowLeft.Position.Z &&
        handRight.Position.Y < spineShoulder.Position.Y)
    {
        // ���ֵ���ͷ�����������β�
        if (//handLeft.Position.Y < head.Position.Y &&
            handLeft.Position.Y > spineBase.Position.Y)
        {
            // ����λ�ڼ���Ҳ�
            //if (handLeft.Position.X > shoulderRight.Position.X)
            if (handLeft.Position.X > spineShoulder.Position.X )
            {
                return GestureParseResult::Succeed;
            }

            return GestureParseResult::Pausing;
        }
    }

    return GestureParseResult::Fail;
}
