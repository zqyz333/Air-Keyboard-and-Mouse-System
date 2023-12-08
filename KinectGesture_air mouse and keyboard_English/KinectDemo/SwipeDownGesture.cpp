#include "stdafx.h"
#include "SwipeDownGesture.h"


SwipeDownGesture::SwipeDownGesture(void)
{
    mStepCount = 3;
}


SwipeDownGesture::~SwipeDownGesture(void)
{
}

GestureParseResult SwipeDownGesture::CheckGesture(Joint* pJoints, int nStepIndex)
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

GestureParseResult SwipeDownGesture::Step1(Joint* pJoints)
{
    Joint handRight = pJoints[JointType_HandRight];
    Joint handLeft = pJoints[JointType_HandLeft];
	Joint elbowLeft = pJoints[JointType_ElbowLeft];
    Joint spinBase = pJoints[JointType_SpineBase];
    Joint spinShoulder = pJoints[JointType_SpineShoulder];
	Joint shoulderLeft = pJoints[JointType_ShoulderLeft];

    // ����������֮ǰ�������ֵ��ڼ��
    if (handLeft.Position.Z < elbowLeft.Position.Z 
        && handRight.Position.Y < spinShoulder.Position.Y)
    {
        // ���ָ��ڣ����*3+�Σ�/4
        if (
			handLeft.Position.Y > (shoulderLeft.Position.Y*3+spinBase.Position.Y)/4)
        {   //������������
            if(handLeft.Position.X<shoulderLeft.Position.X){
			     return GestureParseResult::Succeed;
			}
        }
		return GestureParseResult::Pausing;
    }

    return GestureParseResult::Fail;
}

GestureParseResult SwipeDownGesture::Step2(Joint* pJoints)
{
    Joint handRight = pJoints[JointType_HandRight];
    Joint handLeft = pJoints[JointType_HandLeft];
	Joint elbowLeft = pJoints[JointType_ElbowLeft];
    Joint spinBase = pJoints[JointType_SpineBase];
    Joint spinShoulder = pJoints[JointType_SpineShoulder];
	Joint shoulderLeft = pJoints[JointType_ShoulderLeft];

    // ����������֮ǰ�������ֵ��ڼ��
    if (handLeft.Position.Z < elbowLeft.Position.Z 
        && handRight.Position.Y < spinShoulder.Position.Y)
    {
        // ���ָ��ڣ����*3+�Σ�/4
        if (handLeft.Position.Y > (shoulderLeft.Position.Y*3+spinBase.Position.Y)/4)
        {    //������������
             if(handLeft.Position.X<shoulderLeft.Position.X){
			     return GestureParseResult::Succeed;
			}
        }
		return GestureParseResult::Pausing;
    }

    return GestureParseResult::Fail;
}

GestureParseResult SwipeDownGesture::Step3(Joint* pJoints)
{
    Joint handRight = pJoints[JointType_HandRight];
    Joint handLeft = pJoints[JointType_HandLeft];
	Joint elbowLeft = pJoints[JointType_ElbowLeft];
    Joint spinBase = pJoints[JointType_SpineBase];
    Joint spinShoulder = pJoints[JointType_SpineShoulder];
	Joint shoulderLeft = pJoints[JointType_ShoulderLeft];

    // ����������֮ǰ�������ֵ��ڼ��
    if (handLeft.Position.Z < elbowLeft.Position.Z 
        && handRight.Position.Y < spinShoulder.Position.Y)
    {
        // ���ֵ��ڣ����*3+�Σ�/4
        if (handLeft.Position.Y < (shoulderLeft.Position.Y*3+spinBase.Position.Y)/4)
        {
			//������������
			if(handLeft.Position.X<shoulderLeft.Position.X){
			     return GestureParseResult::Succeed;
			}
        }
		return GestureParseResult::Pausing;
    }

    return GestureParseResult::Fail;
}
