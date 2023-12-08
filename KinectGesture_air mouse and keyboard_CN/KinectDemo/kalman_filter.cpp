#include "kalman_filter.h"

using namespace Eigen;

// ���忨�����˲���״̬����
MatrixXf x(4, 1);
MatrixXf P(4, 4);
MatrixXf F(4, 4);
MatrixXf Q(4, 4);
MatrixXf R(2, 2);
MatrixXf H(2, 4);

// ��ʼ���������˲���
void initializeKalmanFilter()
{
    // ��ʼ״̬���� x(k)
    x << 0, 0, 0, 0;

    // ��ʼ״̬Э������� P(k)
    P << 1, 0, 0, 0,
         0, 1, 0, 0,
         0, 0, 1, 0,
         0, 0, 0, 1;

    // ״̬ת�ƾ��� F
    F << 1, 0, dt, 0,
         0, 1, 0, dt,
         0, 0, 1, 0,
         0, 0, 0, 1;

    // ��������Э������� Q
    Q << pow(dt, 4) / 4 * acceleration_variance, 0, pow(dt, 3) / 2 * acceleration_variance, 0,
         0, pow(dt, 4) / 4 * acceleration_variance, 0, pow(dt, 3) / 2 * acceleration_variance,
         pow(dt, 3) / 2 * acceleration_variance, 0, pow(dt, 2) * acceleration_variance, 0,
         0, pow(dt, 3) / 2 * acceleration_variance, 0, pow(dt, 2) * acceleration_variance;

    // ��������Э������� R
    R << measurement_variance, 0,
         0, measurement_variance;

    // �������� H
    H << 1, 0, 0, 0,
         0, 1, 0, 0;
}

// �������˲���Ԥ�ⲽ��
void kalmanFilterPredict()
{
    // Ԥ��״̬���� x(k)
    x = F * x;

    // Ԥ��״̬Э������� P(k)
    P = F * P * F.transpose() + Q;
}

// �������˲������²���
void kalmanFilterUpdate(const MatrixXf& measurement)
{
    // ���㿨�������� K(k)
    MatrixXf K = P * H.transpose() * (H * P * H.transpose() + R).inverse();

    // ����״̬���� x(k)
    x = x + K * (measurement - H * x);

    // ����״̬Э������� P(k)
    P = (MatrixXf::Identity(4, 4) - K * H) * P;
}

// ��ʼ���������˲���
    initializeKalmanFilter();

    // ģ�����ֵ
    MatrixXd measurement(2, 1);
    measurement << 1, 2;

    // ���п������˲���
    kalmanFilterPredict();
    kalmanFilterUpdate(measurement);

    // ����˲����
    std::cout << "Filtered state: " << x << std::endl;

    return 0;