#include <Eigen/Dense>  // ʹ��Eigen����о�������
#include "kalman.h"
#include "stdafx.h"
#include <iostream>
using namespace std;
using namespace Eigen;


int main() {
    // �����������˲�������
    KalmanFilter kf;

    // ģ��۲�����
    Vector2d z(2, 3);

    // ִ���˲�����
    kf.predict();
    kf.update(z);

    // ��ȡ���Ƶ�λ��
    Vector2d state = kf.getState();

    // ������
    cout << "Estimated position: (" << state(0) << ", " << state(1) << ")" << endl;

    return 0;
}