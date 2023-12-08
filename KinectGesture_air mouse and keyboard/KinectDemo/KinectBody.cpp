#include "stdafx.h"
#include "Eigen\Dense"
#include "kalman.h"
#include "resource.h"
#include "KinectBody.h"
#include "GestureDetection.h"
#include <Strsafe.h>
#include "kinect.h"

#include "kalman_filter.h"
using namespace Eigen;
//kalman filter 准备


int absx = 0;
int absy = 0;
int absx1 = 0;
int absy1 = 0;
float xx=0;
float yy=0;
float zz=0;
float jx=0;
float jy=0;
int Flag1 = 0;
int Flag2 = 0;
int lassoflag1=0;
int lassoflag2=0;

int lf_flag = 0;
int lf_flag1=0;
char morse[5];
int i=0;
int kalman_flag = 0;
int ten = 0;
HINSTANCE hGlobalInst;
HWND hGlobalHwnd;
GestureDetection* pGestureDetection = NULL;

// Joint* pJoints = NULL;

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

				//左手是否在左肩的左上方

				if (joints[6].Position.X<joints[4].Position.X && joints[6].Position.Y>joints[4].Position.Y){lf_flag1=1;}
				else {lf_flag1=0;}

				if (lf_flag && lf_flag1){
					//手部
					xx = joints[10].Position.X;
					yy = joints[10].Position.Y;
					zz = joints[10].Position.Z;
					/*absx = (int)((1920/2*0 + 160*(xx)/zz*0.004/0.0001875)/1920*65536);
					absy = (int)((1080/2*2 - 160*(yy)/zz*0.004/0.0002509)/1080*65536);*/
					//mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, absx, absy, 0, 0 );

					//卡尔曼滤波
					absx = (int)((1920/2*0 + 500*(xx)/zz*0.004/0.0001891)/1920*65536);
					absy = (int)((1080/2*2.8 - 500*(yy)/zz*0.004/0.0002204)/1080*65536);


					// 初始化卡尔曼滤波器
					if (kalman_flag==0){
							initializeKalmanFilter();
							kalman_flag = 1;
							
					}
					//if(ten++>=1){
					//// 输入测量值
					//MatrixXf measurement(2, 1);
					//measurement << (float)absx, (float)absy;

					//// 运行卡尔曼滤波器
					//kalmanFilterPredict();
					//kalmanFilterUpdate(measurement);

					//// 输出滤波结果
					//mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, (int)x(0), (int)x(1), 0, 0 );
					//ten = 0;
					////mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, absx, absy, 0, 0 );
					//}

					measurement << (float)absx, (float)absy;

					// 运行卡尔曼滤波器
					for(int j=0;j<100;j++){
						kalmanFilterPredict();
						kalmanFilterUpdate(measurement);
					}

					 //输出滤波结果
					mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, (int)x(0), (int)x(1), 0, 0 );
					//mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, absx, absy, 0, 0 );
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
		if(lf_flag==1*lf_flag1==1){
			//判断是否按下左键
			Flag1 = righHandState == HandState_Open? 0:1;
			if (Flag1!=Flag2){
				////根据第一个flag是否为1进行“左键按下”或“左键抬起”操作
				if(Flag1!=1){
					mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
				}
				else{
					mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
				}

				Flag2 = Flag1;//前后flag赋值
			}
			////用中间手势判断是否按下右键：一般用处不大，就没添加
			//lassoflag1 = righHandState == HandState_Lasso? 0:1;
			//if (lassoflag1!=lassoflag2){
			//	if(lassoflag1){
			//		mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
			//	} 
			//	else{
			//		mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
			//	}
			//	lassoflag2 = lassoflag1;
			//}
		}
		
    }
    HandState leftHandState = HandState_Unknown;
    hr = pBody->get_HandLeftState(&leftHandState); // 获取左手状态
    if (SUCCEEDED(hr)) 
    {
        StringCchPrintf(result, _countof(result), L"状态：%s\t", (leftHandState == HandState_Open? L"打开" : L"握拳"));
        SetDlgItemText(hGlobalHwnd, IDC_LEFT_HAND, result);
		//判断左手是否握拳
		if(lf_flag1==1){
			lf_flag = leftHandState == HandState_Closed? 1:0;}
		 }
		else{
			lf_flag = 0;
		}

    // 识别手势
    if (pGestureDetection)
    {
        Joint joints[JointType_Count];
        pBody->GetJoints(_countof(joints), joints);
        pGestureDetection->UpdateAllGesture(joints);
    }
}

//键盘
string morse_base[26] = {"01","1000","1010","100","0","0010","110","0000","00","0111","101","0100","11","10","111","0110","1101","010","000","1","001","0001","011","1001","1011","1100"};
int mosi(string char_input){
for(int i=0; i<26; i++){
 if(char_input == morse_base[i]){
  return (65 + i);
  break;
  }
 }
}

void WINAPI OnGestureRecognized(GestureType type)
{   if (!(lf_flag==1 * lf_flag1==1)){
		switch (type)
		{
		case GestureType::SwipeRight:
			SetDlgItemText(hGlobalHwnd, IDC_GESTURE, L"向右摆手");
			/*keybd_event(68,0,0,0);
			keybd_event(68,0,KEYEVENTF_KEYUP,0);*/
			morse[i] = '0';
			Beep(800,500);
			i++;
			break;
		case GestureType::SwipeLeft:
			SetDlgItemText(hGlobalHwnd, IDC_GESTURE, L"向左摆手");
			/*keybd_event(65,0,0,0);
			keybd_event(65,0,KEYEVENTF_KEYUP,0);*/
			morse[i] = '1';
			Beep(800,500);
			i++;
			break;
		case GestureType::SwipeUp:
			SetDlgItemText(hGlobalHwnd, IDC_GESTURE, L"向上摆手");
			/*keybd_event(87,0,0,0);
			keybd_event(87,0,KEYEVENTF_KEYUP,0);*/
			morse[i] = '\0';
			Beep(1000,800);
			i++;
			break;
		case GestureType::SwipeDown:
			SetDlgItemText(hGlobalHwnd, IDC_GESTURE, L"向下摆手");
			/*keybd_event(83,0,0,0);
			keybd_event(83,0,KEYEVENTF_KEYUP,0);*/
			break;
		default:
			break;
		}
		if((i!=0 && morse[i-1] == '\0')||i>=5){
			keybd_event((mosi(morse)),0,0,0);
			keybd_event((mosi(morse)),0,KEYEVENTF_KEYUP,0);
			i=0;
		}
	}
	
}
