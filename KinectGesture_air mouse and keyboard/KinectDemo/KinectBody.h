#pragma once
#include "KinectDevice.h"
class KinectBody
{
public:
    KinectBody(void);
    ~KinectBody(void);

    // �����Ӵ���
    int Run(HINSTANCE hInst, HWND hWnd);

    // ������Ϣ�ַ�
    static LRESULT CALLBACK MessageRouter(HWND, UINT, WPARAM, LPARAM);

    // ������Ϣ����
    LRESULT CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

private:
    // ���ھ��
    HANDLE mHwnd;

    // ����
    KinectConfig mKinectConfig;
    // Kinect �豸
    KinectDevice* pKinectDevice;
    // ��������֡
    void Update();
};

