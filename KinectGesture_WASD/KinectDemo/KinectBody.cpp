#include "stdafx.h"
#include "resource.h"
#include "KinectBody.h"
#include "GestureDetection.h"
#include <Strsafe.h>

HINSTANCE hGlobalInst;
HWND hGlobalHwnd;
GestureDetection* pGestureDetection = NULL;
void WINAPI OnGestureRecognized(GestureType type);
// ����֡���ݻص�
void HandleBodyFrame(IBody** ppBodies, int bodyCount);
KinectBody::KinectBody(void)
{
}


KinectBody::~KinectBody(void)
{
	if (pKinectDevice != NULL) 
	{
		pKinectDevice->Close();
		delete pKinectDevice;
	}
}

int KinectBody::Run(HINSTANCE hInst, HWND parent) 
{
    EnableWindow(parent, false);
    MSG msg = {0};
    hGlobalInst = hInst;// �����Ӧ�õ�ȫ������

    // ��ʼ��������
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.cbWndExtra    = DLGWINDOWEXTRA;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon         = LoadIcon(hInst, MAKEINTRESOURCE(IDI_KINECTDEMO));
    wc.lpfnWndProc   = DefDlgProc;
    wc.lpszClassName = _TEXT("KinectBodyCls");

    if (!RegisterClass(&wc)) 
    {
        EnableWindow(parent, TRUE);
        SetForegroundWindow(parent);
        return 0;
    }

    // ��������
    HWND hWnd = CreateDialogParam(
        NULL,
        MAKEINTRESOURCE(IDD_BODY),
        NULL,
        (DLGPROC) KinectBody::MessageRouter,
        reinterpret_cast<LPARAM> (this)
        );

    // ��Ļ��С
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    RECT region;
    GetWindowRect(hWnd, &region);
    int w = region.right - region.left;
    int h = region.bottom - region.top;
    int x = (screenWidth - w) / 2;
    int y = (screenHeight - h) / 2;

    MoveWindow(hWnd, x, y, w, h, FALSE);
    ShowWindow(hWnd,SW_SHOW);

    while (WM_QUIT != msg.message)
    {
        // ��������
        Update();
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (hWnd && IsDialogMessage(hWnd, &msg)) 
            {
                continue;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    UnregisterClass(_TEXT("KinectBodyCls"), hInst);
    EnableWindow(parent, TRUE);
    SetForegroundWindow(parent);

    return 1;
}

LRESULT CALLBACK KinectBody::MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    KinectBody* pThis = NULL;
    if (WM_INITDIALOG == uMsg)
    {
        pThis = reinterpret_cast<KinectBody*> (lParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR> (pThis));
    }
    else 
    {
        pThis = reinterpret_cast<KinectBody*> (GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (pThis)
    {
        return pThis->DlgProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

LRESULT CALLBACK KinectBody::DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
    int wmId, wmEvent;
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        {
            mHwnd = hWnd;
            hGlobalHwnd = hWnd;
            // ����
            mKinectConfig.bAcquireColor = false;
            mKinectConfig.bAcquireDepth = false;
            mKinectConfig.bAcquireBody = true;
            mKinectConfig.hWndForColor = NULL;
            mKinectConfig.hWndForDepth = NULL;
            mKinectConfig.hWndForBody = GetDlgItem(hWnd, IDC_BODY);
            mKinectConfig.BodyFrameCallback = HandleBodyFrame;

            // ��ʼ������ʶ��
            pGestureDetection = new GestureDetection();
            pGestureDetection->fGestureRecongnized = OnGestureRecognized;
            pGestureDetection->AddGesture(GestureType::SwipeRight);//���ʶ����������
            pGestureDetection->AddGesture(GestureType::SwipeLeft);
			pGestureDetection->AddGesture(GestureType::SwipeUp);
			pGestureDetection->AddGesture(GestureType::SwipeDown);

            pKinectDevice = new KinectDevice(&mKinectConfig);
            HRESULT hr = pKinectDevice->Open();
            if (FAILED(hr))
            {
                MessageBox(hWnd, L"��ʼ��ʧ�ܡ�", L"��ʾ", MB_OK);
            }
        }
        break;
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return FALSE;
}

void KinectBody::Update()
{
    if (!pKinectDevice)
        return;
    pKinectDevice->Update();
}

void HandleBodyFrame(IBody** ppBodies, int bodyCount)
{
    // TODO:�������֡����

    IBody* pBody = NULL;
    float distance = -1;
    // Ѱ���뾵ͷ���������
    for (int i = 0; i < bodyCount; i ++)
    {
        IBody* tmpBody = ppBodies[i];
        if (tmpBody)
        {
            BOOLEAN bTracked = FALSE;
            tmpBody->get_IsTracked(&bTracked);
            if (bTracked)
            {
                Joint joints[JointType_Count];
                tmpBody->GetJoints(_countof(joints), joints);
                Joint head = joints[JointType_Head];
                float tmpDis = head.Position.Z;
                if (distance >= 0)
                {
                    if (tmpDis < distance)
                    {
                        distance = tmpDis;
                        pBody = tmpBody;
                    }
                }
                else
                {
                    distance = tmpDis;
                    pBody = tmpBody;
                }
            }
        }
    }

    if (pBody == NULL)
        return;
    WCHAR result[60];
    HandState righHandState = HandState_Unknown;
    HRESULT hr = pBody->get_HandRightState(&righHandState);// ��ȡ����״̬
    if (SUCCEEDED(hr)) 
    {
        StringCchPrintf(result, _countof(result), L"״̬��%s\t", (righHandState == HandState_Open? L"��" : L"��ȭ")); // ֻ�ж�2��
        SetDlgItemText(hGlobalHwnd, IDC_RIGHT_HAND, result);
    }
    HandState leftHandState = HandState_Unknown;
    hr = pBody->get_HandLeftState(&leftHandState); // ��ȡ����״̬
    if (SUCCEEDED(hr)) 
    {
        StringCchPrintf(result, _countof(result), L"״̬��%s\t", (leftHandState == HandState_Open? L"��" : L"��ȭ"));
        SetDlgItemText(hGlobalHwnd, IDC_LEFT_HAND, result);
    }

    // ʶ������
    if (pGestureDetection)
    {
        Joint joints[JointType_Count];
        pBody->GetJoints(_countof(joints), joints);
        pGestureDetection->UpdateAllGesture(joints);
    }
}

void WINAPI OnGestureRecognized(GestureType type)
{
    switch (type)
    {
    case GestureType::SwipeRight:
        SetDlgItemText(hGlobalHwnd, IDC_GESTURE, L"���Ұ���");
		keybd_event(68,0,0,0);
		keybd_event(68,0,KEYEVENTF_KEYUP,0);
        break;
    case GestureType::SwipeLeft:
        SetDlgItemText(hGlobalHwnd, IDC_GESTURE, L"�������");
		keybd_event(65,0,0,0);
		keybd_event(65,0,KEYEVENTF_KEYUP,0);
        break;
	case GestureType::SwipeUp:
        SetDlgItemText(hGlobalHwnd, IDC_GESTURE, L"���ϰ���");
		keybd_event(87,0,0,0);
		keybd_event(87,0,KEYEVENTF_KEYUP,0);
        break;
	case GestureType::SwipeDown:
        SetDlgItemText(hGlobalHwnd, IDC_GESTURE, L"���°���");
		keybd_event(83,0,0,0);
		keybd_event(83,0,KEYEVENTF_KEYUP,0);
        break;
    default:
        break;
    }
}
