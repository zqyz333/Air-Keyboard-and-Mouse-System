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
            // Configuration
            mKinectConfig.bAcquireColor = false;
            mKinectConfig.bAcquireDepth = false;
            mKinectConfig.bAcquireBody = true;
            mKinectConfig.hWndForColor = NULL;
            mKinectConfig.hWndForDepth = NULL;
            mKinectConfig.hWndForBody = GetDlgItem(hWnd, IDC_BODY);
            mKinectConfig.BodyFrameCallback = HandleBodyFrame;

            // Initialize Gestures Recognition
            pGestureDetection = new GestureDetection();
            pGestureDetection->fGestureRecongnized = OnGestureRecognized;
            pGestureDetection->AddGesture(GestureType::SwipeRight);
            pGestureDetection->AddGesture(GestureType::SwipeLeft);
			pGestureDetection->AddGesture(GestureType::SwipeUp);//Added recognition gesture types
			pGestureDetection->AddGesture(GestureType::SwipeDown);//Added recognition gesture types

            pKinectDevice = new KinectDevice(&mKinectConfig);
            HRESULT hr = pKinectDevice->Open();
            if (FAILED(hr))
            {
                MessageBox(hWnd, L"Initialization Failed。", L"tip", MB_OK);
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
    // TODO:Processing bone frame data

    IBody* pBody = NULL;
    float distance = -1;
    // Searching for the closest human body to the camera
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

				// Determine if the left hand is above(and left) the left shoulder

				if (joints[6].Position.X<joints[4].Position.X && joints[6].Position.Y>joints[4].Position.Y){lf_flag1=1;}
				else {lf_flag1=0;}

				if (lf_flag && lf_flag1){
					// loading hand coordinates
					xx = joints[10].Position.X;
					yy = joints[10].Position.Y;
					zz = joints[10].Position.Z;
					/*absx = (int)((1920/2*0 + 160*(xx)/zz*0.004/0.0001875)/1920*65536);
					absy = (int)((1080/2*2 - 160*(yy)/zz*0.004/0.0002509)/1080*65536);*/
					//mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, absx, absy, 0, 0 );

					// Coordinate Transformation: Image coordinate->Camera coordinate system->Linear camera model
					absx = (int)((1920/2*0 + 500*(xx)/zz*0.004/0.0001891)/1920*65536);
					absy = (int)((1080/2*2.8 - 500*(yy)/zz*0.004/0.0002204)/1080*65536);


					// Initialize Kalman filter
					if (kalman_flag==0){
							initializeKalmanFilter();
							kalman_flag = 1;
							
					}
					//if(ten++>=1){
					//// load Measurement value to vector
					//MatrixXf measurement(2, 1);
					//measurement << (float)absx, (float)absy;

					//// run Kalman filter
					//kalmanFilterPredict();
					//kalmanFilterUpdate(measurement);

					//// ouput Kalman filter result
					//mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, (int)x(0), (int)x(1), 0, 0 );
					//ten = 0;
					////mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, absx, absy, 0, 0 );
					//}

					measurement << (float)absx, (float)absy;

					// run Kalman filter
					for(int j=0;j<100;j++){
						kalmanFilterPredict();
						kalmanFilterUpdate(measurement);
					}

					 // ouput Kalman filter result to the mouse coordinate
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
    HRESULT hr = pBody->get_HandRightState(&righHandState);// Get right hand state
    if (SUCCEEDED(hr)) 
    {
        StringCchPrintf(result, _countof(result), L"state：%s\t", (righHandState == HandState_Open? L"open" : L"fist")); // 只判断2种
        SetDlgItemText(hGlobalHwnd, IDC_RIGHT_HAND, result);
		if(lf_flag==1*lf_flag1==1){
			// Determine if the right hand is fisting
			Flag1 = righHandState == HandState_Open? 0:1;
			if (Flag1!=Flag2){
				//// Perform the "Left down" or "Left up" operation based on whether the Flag1 is 1
				if(Flag1!=1){
					mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
				}
				else{
					mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
				}

				Flag2 = Flag1;//Assign Flag1's vlaue to Flag2
			}
			//// Use the middle gesture to determine whether to press the right button: it is generally not very useful, so it was not added
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
    hr = pBody->get_HandLeftState(&leftHandState); // Get left hand state
    if (SUCCEEDED(hr)) 
    {
        StringCchPrintf(result, _countof(result), L"state：%s\t", (leftHandState == HandState_Open? L"open" : L"fist"));
        SetDlgItemText(hGlobalHwnd, IDC_LEFT_HAND, result);
		// Determine if the left hand is fisting
		if(lf_flag1==1){
			lf_flag = leftHandState == HandState_Closed? 1:0;}
		 }
		else{
			lf_flag = 0;
		}

    // Recognizing gestures
    if (pGestureDetection)
    {
        Joint joints[JointType_Count];
        pBody->GetJoints(_countof(joints), joints);
        pGestureDetection->UpdateAllGesture(joints);
    }
}

//air keyboard
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
			SetDlgItemText(hGlobalHwnd, IDC_GESTURE, L"SwipeRight");
			/*keybd_event(68,0,0,0);
			keybd_event(68,0,KEYEVENTF_KEYUP,0);*/
			morse[i] = '0';
			Beep(800,500);
			i++;
			break;
		case GestureType::SwipeLeft:
			SetDlgItemText(hGlobalHwnd, IDC_GESTURE, L"SwipeLeft");
			/*keybd_event(65,0,0,0);
			keybd_event(65,0,KEYEVENTF_KEYUP,0);*/
			morse[i] = '1';
			Beep(800,500);
			i++;
			break;
		case GestureType::SwipeUp:
			SetDlgItemText(hGlobalHwnd, IDC_GESTURE, L"SwipeUp");
			/*keybd_event(87,0,0,0);
			keybd_event(87,0,KEYEVENTF_KEYUP,0);*/
			morse[i] = '\0';
			Beep(1000,800);
			i++;
			break;
		case GestureType::SwipeDown:
			SetDlgItemText(hGlobalHwnd, IDC_GESTURE, L"SwipeDown");
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
