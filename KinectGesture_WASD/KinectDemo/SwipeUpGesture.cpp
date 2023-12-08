#include "stdafx.h"
#include "SwipeUpGesture.h"


SwipeUpGesture::SwipeUpGesture(void)
{
    mStepCount = 3;
}


SwipeUpGesture::~SwipeUpGesture(void)
{
}

GestureParseResult SwipeUpGesture::CheckGesture(Joint* pJoints, int nStepIndex)
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

GestureParseResult SwipeUpGesture::Step1(Joint* pJoints)
{
    Joint handRight = pJoints[JointType_HandRight];
    Joint handLeft = pJoints[JointType_HandLeft];
    Joint head = pJoints[JointType_Head];
    Joint elbowRight = pJoints[JointType_ElbowRight];
    Joint spinBase = pJoints[JointType_SpineBase];
    Joint spinShoulder = pJoints[JointType_SpineShoulder];
    Joint shoulderRight = pJoints[JointType_ShoulderRight];

    // ����������֮ǰ�������ֵ��ڼ��
    if (handRight.Position.Z < elbowRight.Position.Z 
        && handLeft.Position.Y < spinShoulder.Position.Y)
    {
        // ���ֵ��ڼ粿
        if (handRight.Position.Y < shoulderRight.Position.Y)
        {
            //�������Ҽ��Ҳ�
            if(handRight.Position.X>shoulderRight.Position.X){
			     return GestureParseResult::Succeed;
			}
        }
		return GestureParseResult::Pausing;
    }

    return GestureParseResult::Fail;
}

GestureParseResult SwipeUpGesture::Step2(Joint* pJoints)
{
    Joint handRight = pJoints[JointType_HandRight];
    Joint handLeft = pJoints[JointType_HandLeft];
    Joint head = pJoints[JointType_Head];
    Joint elbowRight = pJoints[JointType_ElbowRight];
    Joint spinBase = pJoints[JointType_SpineBase];
    Joint spinShoulder = pJoints[JointType_SpineShoulder];
    Joint shoulderRight = pJoints[JointType_ShoulderRight];

    // ����������֮ǰ�������ֵ��ڼ��
    if (handRight.Position.Z < elbowRight.Position.Z 
        && handLeft.Position.Y < spinShoulder.Position.Y)
    {
        // ���ֵ��ڼ粿
        if (
            handRight.Position.Y < shoulderRight.Position.Y)
        {
            //�������Ҽ��Ҳ�
            if(handRight.Position.X>shoulderRight.Position.X){
			     return GestureParseResult::Succeed;
			}
        }
		return GestureParseResult::Pausing;
    }

    return GestureParseResult::Fail;
}

GestureParseResult SwipeUpGesture::Step3(Joint* pJoints)
{
    Joint handRight = pJoints[JointType_HandRight];
    Joint handLeft = pJoints[JointType_HandLeft];
    Joint head = pJoints[JointType_Head];
    Joint elbowRight = pJoints[JointType_ElbowRight];
    Joint spinBase = pJoints[JointType_SpineBase];
    Joint spinShoulder = pJoints[JointType_SpineShoulder];
    Joint shoulderRight = pJoints[JointType_ShoulderRight];
    Joint shoulderLeft = pJoints[JointType_ShoulderLeft];

    // ����������֮ǰ�������ֵ��ڼ��
    if (handRight.Position.Z < elbowRight.Position.Z 
        && handLeft.Position.Y < spinShoulder.Position.Y)
    {
        // ���ָ��ڼ��
        if (
            handRight.Position.Y > shoulderRight.Position.Y)
        {   //�������Ҽ��Ҳ�
            if(handRight.Position.X>shoulderRight.Position.X){
			     return GestureParseResult::Succeed;
			}
        }
		return GestureParseResult::Pausing;
    }

    return GestureParseResult::Fail;
}
