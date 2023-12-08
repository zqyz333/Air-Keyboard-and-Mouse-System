#include "stdafx.h"
#include "resource.h"
#include "KinectBody.h"
#include "GestureDetection.h"
#include <Strsafe.h>

HINSTANCE hGlobalInst;
HWND hGlobalHwnd;
GestureDetection* pGestureDetection = NULL;
void WINAPI OnGestureRecognized(GestureType type);
// 骨骼帧数据回调
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
    hGlobalInst = hInst;// 保存对应用的全局引用

    // 初始化窗体类
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

    // 创建窗体
    HWND hWnd = CreateDialogParam(
        NULL,
        MAKEINTRESOURCE(IDD_BODY),
        NULL,
        (DLGPROC) KinectBody::MessageRouter,
        reinterpret_cast<LPARAM> (this)
        );

    // 屏幕大小
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
        // 更新数据
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
            // 配置
            mKinectConfig.bAcquireColor = false;
            mKinectConfig.bAcquireDepth = false;
            mKinectConfig.bAcquireBody = true;
            mKinectConfig.hWndForColor = NULL;
            mKinectConfig.hWndForDepth = NULL;
            mKinectConfig.hWndForBody = GetDlgItem(hWnd, IDC_BODY);
            mKinectConfig.BodyFrameCallback = HandleBodyFrame;

            // 初始化手势识别
            pGestureDetection = new GestureDetection();
            pGestureDetection->fGestureRecongnized = OnGestureRecognized;
            pGestureDetection->AddGesture(GestureType::SwipeRight);//添加识别手势类型
            pGestureDetection->AddGesture(GestureType::SwipeLeft);
			pGestureDetection->AddGesture(GestureType::SwipeUp);
			pGestureDetection->AddGesture(GestureType::SwipeDown);

            pKinectDevice = new KinectDevice(&mKinectConfig);
            HRESULT hr = pKinectDevice->Open();
            if (FAILED(hr))
            {
                MessageBox(hWnd, L"初始化失败。", L"提示", MB_OK);
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
    // TODO:处理骨骼帧数据

    IBody* pBody = NULL;
    float distance = -1;
    // 寻找离镜头最近的人体
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
    HRESULT hr = pBody->get_HandRightState(&righHandState);// 获取右手状态
    if (SUCCEEDED(hr)) 
    {
        StringCchPrintf(result, _countof(result), L"状态：%s\t", (righHandState == HandState_Open? L"打开" : L"握拳")); // 只判断2种
        SetDlgItemText(hGlobalHwnd, IDC_RIGHT_HAND, result);
    }
    HandState leftHandState = HandState_Unknown;
    hr = pBody->get_HandLeftState(&leftHandState); // 获取左手状态
    if (SUCCEEDED(hr)) 
    {
        StringCchPrintf(result, _countof(result), L"状态：%s\t", (leftHandState == HandState_Open? L"打开" : L"握拳"));
        SetDlgItemText(hGlobalHwnd, IDC_LEFT_HAND, result);
    }

    // 识别手势
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
        SetDlgItemText(hGlobalHwnd, IDC_GESTURE, L"向右摆手");
		keybd_event(68,0,0,0);
		keybd_event(68,0,KEYEVENTF_KEYUP,0);
        break;
    case GestureType::SwipeLeft:
        SetDlgItemText(hGlobalHwnd, IDC_GESTURE, L"向左摆手");
		keybd_event(65,0,0,0);
		keybd_event(65,0,KEYEVENTF_KEYUP,0);
        break;
	case GestureType::SwipeUp:
        SetDlgItemText(hGlobalHwnd, IDC_GESTURE, L"向上摆手");
		keybd_event(87,0,0,0);
		keybd_event(87,0,KEYEVENTF_KEYUP,0);
        break;
	case GestureType::SwipeDown:
        SetDlgItemText(hGlobalHwnd, IDC_GESTURE, L"向下摆手");
		keybd_event(83,0,0,0);
		keybd_event(83,0,KEYEVENTF_KEYUP,0);
        break;
    default:
        break;
    }
}
