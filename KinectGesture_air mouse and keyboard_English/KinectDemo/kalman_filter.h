#include "stdafx.h"
#include "Eigen/Dense"  // Using the "Eigen" library for matrix operations

using namespace Eigen;

// Kalman filter parameters
const double dt = 1/30;       // Time step
const double acceleration_variance = 10.0;  // Process noise covariance
const double measurement_variance = 10.0;   // Measurement noise covariance

// Kalman filter state variable
MatrixXf x(4, 1); // State vector, representing(px, py, vx, vy)
MatrixXf P(4, 4); // State Covariance matrix
MatrixXf F(4, 4); // State Transition Matrix
MatrixXf Q(4, 4); // Process noise covariance matrix
MatrixXf R(2, 2); // Measurement noise covariance matrix
MatrixXf H(2, 4); // Measurement matrix

// Measurement Vector
MatrixXf measurement(2, 1);

// Initialize Kalman filter
void initializeKalmanFilter()
{
    // Initialize State vector x(k)
    x << 0, 0, 0, 0;

    // Initialize State Covariance matrix P(k)
    P << 100, 0, 0, 0,
         0, 100, 0, 0,
         0, 0, 100, 0,
         0, 0, 0, 100;

    // Initialize State Transition Matrix F
    F << 1, 0, dt, 0,
         0, 1, 0, dt,
         0, 0, 1, 0,
         0, 0, 0, 1;

    // Initialize Process noise covariance matrix Q
    Q << pow(dt, 4) / 4 * acceleration_variance, 0, pow(dt, 3) / 2 * acceleration_variance, 0,
         0, pow(dt, 4) / 4 * acceleration_variance, 0, pow(dt, 3) / 2 * acceleration_variance,
         pow(dt, 3) / 2 * acceleration_variance, 0, pow(dt, 2) * acceleration_variance, 0,
         0, pow(dt, 3) / 2 * acceleration_variance, 0, pow(dt, 2) * acceleration_variance;

    // Initialize Measurement noise covariance matrix R
    R << measurement_variance, 0,
         0, measurement_variance;

    // Initialize Measurement matrix H
    H << 1, 0, 0, 0,
         0, 1, 0, 0;
}

// Kalman filter prediction steps
void kalmanFilterPredict()
{
    // Predict state vectors x(k)
    x = F * x;

    // Predict state covariance matrix P(k)
    P = F * P * F.transpose() + Q;
}

// Kalman filter updating steps
void kalmanFilterUpdate(const MatrixXf& measurement)
{
    // Calculate Kalman gain K(k)
    MatrixXf K = P * H.transpose() * (H * P * H.transpose() + R).inverse();

    // Update State vector x(k)
    x = x + K * (measurement - H * x);

    // Update State Covariance matrix P(k)
    P = (MatrixXf::Identity(4, 4) - K * H) * P;
}
