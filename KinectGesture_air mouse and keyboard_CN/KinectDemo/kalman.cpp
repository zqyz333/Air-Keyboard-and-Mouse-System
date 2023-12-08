#include <Eigen/Dense>  // 使用Eigen库进行矩阵运算
#include "kalman.h"
#include "stdafx.h"
#include <iostream>
using namespace std;
using namespace Eigen;


int main() {
    // 创建卡尔曼滤波器对象
    KalmanFilter kf;

    // 模拟观测数据
    Vector2d z(2, 3);

    // 执行滤波过程
    kf.predict();
    kf.update(z);

    // 获取估计的位置
    Vector2d state = kf.getState();

    // 输出结果
    cout << "Estimated position: (" << state(0) << ", " << state(1) << ")" << endl;

    return 0;
}