#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

#include <Eigen/Dense>

// �����������˲�������
void initializeKalmanFilter();
void kalmanFilterPredict();
void kalmanFilterUpdate(const Eigen::MatrixXf& measurement);

// �����������˲���״̬����
extern Eigen::MatrixXf x;
extern Eigen::MatrixXf P;
extern Eigen::MatrixXf F;
extern Eigen::MatrixXf Q;
extern Eigen::MatrixXf R;
extern Eigen::MatrixXf H;
extern Eigen::MatrixXf measurement;
#endif  // KALMAN_FILTER_H