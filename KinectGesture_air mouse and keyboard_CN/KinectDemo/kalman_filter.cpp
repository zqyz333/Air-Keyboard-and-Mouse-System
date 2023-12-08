#include "kalman_filter.h"

using namespace Eigen;

// 定义卡尔曼滤波器状态变量
MatrixXf x(4, 1);
MatrixXf P(4, 4);
MatrixXf F(4, 4);
MatrixXf Q(4, 4);
MatrixXf R(2, 2);
MatrixXf H(2, 4);

// 初始化卡尔曼滤波器
void initializeKalmanFilter()
{
    // 初始状态向量 x(k)
    x << 0, 0, 0, 0;

    // 初始状态协方差矩阵 P(k)
    P << 1, 0, 0, 0,
         0, 1, 0, 0,
         0, 0, 1, 0,
         0, 0, 0, 1;

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

// 初始化卡尔曼滤波器
    initializeKalmanFilter();

    // 模拟测量值
    MatrixXd measurement(2, 1);
    measurement << 1, 2;

    // 运行卡尔曼滤波器
    kalmanFilterPredict();
    kalmanFilterUpdate(measurement);

    // 输出滤波结果
    std::cout << "Filtered state: " << x << std::endl;

    return 0;