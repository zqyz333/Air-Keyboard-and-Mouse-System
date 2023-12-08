#include "stdafx.h"
#include "Eigen/Dense"  // ʹ��Eigen����о�������

using namespace Eigen;

// �������˲�������
const double dt = 1/30;       // ʱ�䲽��
const double acceleration_variance = 10.0;  // ��������Э����
const double measurement_variance = 10.0;   // ��������Э����

// �������˲���״̬����
MatrixXf x(4, 1); // ״̬��������ʾ(px, py, vx, vy)
MatrixXf P(4, 4); // ״̬Э�������
MatrixXf F(4, 4); // ״̬ת�ƾ���
MatrixXf Q(4, 4); // ��������Э�������
MatrixXf R(2, 2); // ��������Э�������
MatrixXf H(2, 4); // ��������

//�������ֵ
MatrixXf measurement(2, 1);

// ��ʼ���������˲���
void initializeKalmanFilter()
{
    // ��ʼ״̬���� x(k)
    x << 0, 0, 0, 0;

    // ��ʼ״̬Э������� P(k)
    P << 100, 0, 0, 0,
         0, 100, 0, 0,
         0, 0, 100, 0,
         0, 0, 0, 100;

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
