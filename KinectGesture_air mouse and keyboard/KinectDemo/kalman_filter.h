#include "stdafx.h"
#include "Eigen/Dense"  // 使用Eigen库进行矩阵运算

using namespace Eigen;

// 卡尔曼滤波器参数
const double dt = 1/30;       // 时间步长
const double acceleration_variance = 10.0;  // 过程噪声协方差
const double measurement_variance = 10.0;   // 测量噪声协方差

// 卡尔曼滤波器状态变量
MatrixXf x(4, 1); // 状态向量，表示(px, py, vx, vy)
MatrixXf P(4, 4); // 状态协方差矩阵
MatrixXf F(4, 4); // 状态转移矩阵
MatrixXf Q(4, 4); // 过程噪声协方差矩阵
MatrixXf R(2, 2); // 测量噪声协方差矩阵
MatrixXf H(2, 4); // 测量矩阵

//输入测量值
MatrixXf measurement(2, 1);

// 初始化卡尔曼滤波器
void initializeKalmanFilter()
{
    // 初始状态向量 x(k)
    x << 0, 0, 0, 0;

    // 初始状态协方差矩阵 P(k)
    P << 100, 0, 0, 0,
         0, 100, 0, 0,
         0, 0, 100, 0,
         0, 0, 0, 100;

    // 状态转移矩阵 F
    F << 1, 0, dt, 0,
         0, 1, 0, dt,
         0, 0, 1, 0,
         0, 0, 0, 1;

    // 过程噪声协方差矩阵 Q
    Q << pow(dt, 4) / 4 * acceleration_variance, 0, pow(dt, 3) / 2 * acceleration_variance, 0,
         0, pow(dt, 4) / 4 * acceleration_variance, 0, pow(dt, 3) / 2 * acceleration_variance,
         pow(dt, 3) / 2 * acceleration_variance, 0, pow(dt, 2) * acceleration_variance, 0,
         0, pow(dt, 3) / 2 * acceleration_variance, 0, pow(dt, 2) * acceleration_variance;

    // 测量噪声协方差矩阵 R
    R << measurement_variance, 0,
         0, measurement_variance;

    // 测量矩阵 H
    H << 1, 0, 0, 0,
         0, 1, 0, 0;
}

// 卡尔曼滤波器预测步骤
void kalmanFilterPredict()
{
    // 预测状态向量 x(k)
    x = F * x;

    // 预测状态协方差矩阵 P(k)
    P = F * P * F.transpose() + Q;
}

// 卡尔曼滤波器更新步骤
void kalmanFilterUpdate(const MatrixXf& measurement)
{
    // 计算卡尔曼增益 K(k)
    MatrixXf K = P * H.transpose() * (H * P * H.transpose() + R).inverse();

    // 更新状态向量 x(k)
    x = x + K * (measurement - H * x);

    // 更新状态协方差矩阵 P(k)
    P = (MatrixXf::Identity(4, 4) - K * H) * P;
}
