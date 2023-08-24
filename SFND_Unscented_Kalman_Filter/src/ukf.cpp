#include "ukf.h"
#include "Eigen/Dense"
#include <iostream>
using Eigen::MatrixXd;
using Eigen::VectorXd;

/**
 * Initializes Unscented Kalman filter
 */
UKF::UKF()
{
    // if this is false, laser measurements will be ignored (except during init)
    use_laser_ = true;

    // if this is false, radar measurements will be ignored (except during init)
    use_radar_ = true;

    // initial state vector
    x_ = VectorXd(5);
    x_.fill(0.0);
    // initial covariance matrix
    P_ = MatrixXd(5, 5);
    x_.fill(0.0);
    // Process noise standard deviation longitudinal acceleration in m/s^2
    std_a_ = 30;

    // Process noise standard deviation yaw acceleration in rad/s^2
    std_yawdd_ = 30;

    /**
     * DO NOT MODIFY measurement noise values below.
     * These are provided by the sensor manufacturer.
     */

    // Laser measurement noise standard deviation position1 in m
    std_laspx_ = 0.15;

    // Laser measurement noise standard deviation position2 in m
    std_laspy_ = 0.15;

    // Radar measurement noise standard deviation radius in m
    std_radr_ = 0.3;

    // Radar measurement noise standard deviation angle in rad
    std_radphi_ = 0.03;

    // Radar measurement noise standard deviation radius change in m/s
    std_radrd_ = 0.3;

    /**
     * End DO NOT MODIFY section for measurement noise values
     */

    /**
     * TODO: Complete the initialization. See ukf.h for other member properties.
     * Hint: one or more values initialized above might be wildly off...
     */

    // check first measurement
    is_initialized_ = false;

    // state dimension
    n_x_ = 5;

    // Augmented state dimension
    n_aug_ = 7;

    // Sigma point spreading paramater
    lambda_ = 3 - n_x_;

    // predicted sigma points matrix
    Xsig_pred_ = MatrixXd(5, 2 * n_aug_ + 1);
    Xsig_pred_.fill(0.0);
    // create vector for weights
    weights_ = VectorXd(2 * n_aug_ + 1);
    double weight_0 = lambda_ / (lambda_ + n_aug_);
    weights_(0) = weight_0;
    for (int i = 1; i < 2 * n_aug_ + 1; ++i)
    {
        double weight = 0.5 / (n_aug_ + lambda_);
        weights_(i) = weight;
    }
}

UKF::~UKF()
{
}

void UKF::ProcessMeasurement(MeasurementPackage meas_package)
{
    /**
     * TODO: Complete this function! Make sure you switch between lidar and radar
     * measurements.
     */
    if (!is_initialized_)
    {
        time_us_ = meas_package.timestamp_;

        if (meas_package.sensor_type_ == MeasurementPackage::RADAR)
        {
            // std::cout << "Radar" << meas_package.raw_measurements_[0] << " " << meas_package.raw_measurements_[1] <<
            // " "
            //           << meas_package.raw_measurements_[2] << std::endl;
            float rho = meas_package.raw_measurements_[0];
            float phi = meas_package.raw_measurements_[1];
            float rho_dot = meas_package.raw_measurements_[2];

            float px = rho * cos(phi);
            float py = rho * sin(phi);

            x_ << px, py, 0, 0, 0;
        }
        if (meas_package.sensor_type_ == MeasurementPackage::LASER)
        {
            // std::cout << "Laser: " << meas_package.raw_measurements_[0] << " " << meas_package.raw_measurements_[1]
            //           << std::endl;

            float px = meas_package.raw_measurements_[0];
            float py = meas_package.raw_measurements_[1];

            if (fabs(px) < 0.001 && fabs(py) < 0.001)
            {
                px = 0.001;
                py = 0.001;
            }

            x_ << px, py, 0, 0, 0;
        }
        P_ << 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, std_laspx_ * std_laspx_, 0, 0, 0, 0, 0,
            std_laspy_ * std_laspy_;

        time_us_ = meas_package.timestamp_;

        is_initialized_ = true;
        return;
    }

    double dt = (meas_package.timestamp_ - time_us_) / 1000000.0;
    time_us_ = meas_package.timestamp_;
    std::cout << "Prediction: " << std::endl;
    std::cout << P_ << std::endl;
    Prediction(dt);
    if (meas_package.sensor_type_ == MeasurementPackage::RADAR && use_radar_)
    {
        std::cout << "UpdateRadar: " << std::endl;
        UpdateRadar(meas_package);
        std::cout << P_ << std::endl;
    }
    else if (meas_package.sensor_type_ == MeasurementPackage::LASER && use_laser_)
    {
        std::cout << "UpdateLidar: " << std::endl;
        UpdateLidar(meas_package);
        std::cout << P_ << std::endl;
    }
}

void UKF::Prediction(double delta_t)
{
    /**
     * TODO: Complete this function! Estimate the object's location.
     * Modify the state vector, x_. Predict sigma points, the state,
     * and the state covariance matrix.
     */
    // Step 1: Generate Sigma Point,equation in Lesson: 13. Generating Sigma Points and 16. UKF Augmentation

    // Augmented mean State
    VectorXd x_aug_ = VectorXd::Zero(n_aug_);
    x_aug_.head(5) = x_;
    x_aug_(5) = 0;
    x_aug_(6) = 0;

    // Augmented Convariance Matrix
    MatrixXd P_aug_ = MatrixXd::Zero(7, 7);
    P_aug_.topLeftCorner(5, 5) = P_;
    P_aug_(5, 5) = std_a_ * std_a_;
    P_aug_(6, 6) = std_yawdd_ * std_yawdd_;

    // Square root matrix A
    MatrixXd A = P_aug_.llt().matrixL();

    // sigma point Matrix with number sigma point = 2*n_aug_ + 1
    // std::cout << "sigma point Matrix with number sigma point = 2*n_aug_ + 1" << std::endl;
    MatrixXd Xsig_aug_ = MatrixXd(7, 2 * n_aug_ + 1);
    Xsig_aug_.col(0) = x_aug_;
    for (int i = 0; i < n_aug_; i++)
    {
        Xsig_aug_.col(i + 1) = x_aug_ + sqrt(lambda_ + n_aug_) * A.col(i);
        Xsig_aug_.col(i + 1 + n_aug_) = x_aug_ - sqrt(lambda_ + n_aug_) * A.col(i);
    }

    // Step 2: Predict Sigma Points

    // Lesson 20. Sigma Point Prediction

    // build matrix predicted sigma points matrix Xsig_pred_
    // std::cout << "build matrix predicted sigma points matrix Xsig_pred_" << std::endl;
    for (int i = 0; i < 2 * n_aug_ + 1; i++)
    {
        double p_x = Xsig_aug_(0, i);
        double p_y = Xsig_aug_(1, i);
        double v = Xsig_aug_(2, i);
        double yaw = Xsig_aug_(3, i);
        double yawd = Xsig_aug_(4, i);
        double nu_a = Xsig_aug_(5, i);
        double nu_yawdd = Xsig_aug_(6, i);
        // predicted state values
        double px_p, py_p;

        if (fabs(yawd) > 0.001)
        {
            px_p = p_x + v / yawd * (sin(yaw + yawd * delta_t) - sin(yaw));
            py_p = p_y + v / yawd * (cos(yaw) - cos(yaw + yawd * delta_t));
        }
        else
        {
            px_p = p_x + v * delta_t * cos(yaw);
            py_p = p_y + v * delta_t * sin(yaw);
        }

        double v_p = v;
        double yaw_p = yaw + yawd * delta_t;
        double yawd_p = yawd;

        // add noise
        px_p = px_p + 0.5 * nu_a * delta_t * delta_t * cos(yaw);
        py_p = py_p + 0.5 * nu_a * delta_t * delta_t * sin(yaw);
        v_p = v_p + nu_a * delta_t;
        yaw_p = yaw_p + 0.5 * nu_yawdd * delta_t * delta_t;
        yawd_p = yawd_p + nu_yawdd * delta_t;

        Xsig_pred_(0, i) = px_p;
        Xsig_pred_(1, i) = py_p;
        Xsig_pred_(2, i) = v_p;
        Xsig_pred_(3, i) = yaw_p;
        Xsig_pred_(4, i) = yawd_p;
    }

    // Step 3: Predict Mean and Covariance
    // Check lesson 22. Predicted Mean and Covariance
    // vector Weight of sigma point was create in contructor

    // predicted state mean

    // std::cout << "predicted state mean" << std::endl;
    x_.fill(0.0);
    for (int i = 0; i < 2 * n_aug_ + 1; i++)
    {
        x_ = x_ + weights_(i) * Xsig_pred_.col(i);
    }

    // predicted state covariance matrix
    // std::cout << "predicted state covariance matrix" << std::endl;
    P_.fill(0.0);
    for (int i = 0; i < 2 * n_aug_ + 1; ++i)
    { // iterate over sigma points
        // state difference
        VectorXd x_diff = Xsig_pred_.col(i) - x_;
        // angle normalization
        while (x_diff(3) > M_PI)
            x_diff(3) -= 2. * M_PI;
        while (x_diff(3) < -M_PI)
            x_diff(3) += 2. * M_PI;

        P_ = P_ + weights_(i) * x_diff * x_diff.transpose();
    }
}

void UKF::UpdateLidar(MeasurementPackage meas_package)
{
    /**
     * TODO: Complete this function! Use lidar data to update the belief
     * about the object's position. Modify the state vector, x_, and
     * covariance, P_.
     * You can also calculate the lidar NIS, if desired.
     */
    // Step 1: Measurement Prediction

    // set measurement dimension, lidar can measure px, py
    int n_z = 2;
    // create matrix for sigma points in measurement space
    MatrixXd Zsig = MatrixXd::Zero(n_z, 2 * n_aug_ + 1);

    // transform sigma points into measurement space
    for (int i = 0; i < (2 * n_aug_ + 1); i++)
    {
        double p_x = Xsig_pred_(0, i);
        double p_y = Xsig_pred_(1, i);
        double v = Xsig_pred_(2, i);
        double yaw = Xsig_pred_(3, i);
        double yawd = Xsig_pred_(4, i);

        Zsig(0, i) = p_x; // p_x
        Zsig(1, i) = p_y; // p_y
    }

    // calculate mean predicted measurement
    VectorXd z_pred = VectorXd::Zero(n_z);
    for (int i = 0; i < (2 * n_aug_ + 1); i++)
    {
        z_pred = z_pred + Zsig.col(i) * weights_(i);
    }

    // calculate innovation covariance matrix S
    MatrixXd S = MatrixXd::Zero(n_z, n_z);

    for (int i = 0; i < 2 * n_aug_ + 1; ++i)
    {
        VectorXd z_diff = Zsig.col(i) - z_pred;
        while (z_diff(1) > M_PI)
            z_diff(1) -= 2. * M_PI;
        while (z_diff(1) < -M_PI)
            z_diff(1) += 2. * M_PI;
        S = S + weights_(i) * z_diff * z_diff.transpose();
    }

    MatrixXd R = MatrixXd::Zero(n_z, n_z);
    R(0, 0) = std_laspx_ * std_laspx_;
    R(1, 1) = std_laspy_ * std_laspy_;
    S = S + R;

    // Step 2: Update State

    // Cross-correlation Matrix
    MatrixXd Tc = MatrixXd::Zero(n_x_, n_z);
    VectorXd z_ = meas_package.raw_measurements_;
    for (int i = 0; i < 2 * n_aug_ + 1; i++)
    {

        VectorXd x_diff = Xsig_pred_.col(i) - x_;
        while (x_diff(3) > M_PI)
            x_diff(3) -= 2.0 * M_PI;
        while (x_diff(3) < -M_PI)
            x_diff(3) += 2.0 * M_PI;

        VectorXd z_diff = Zsig.col(i) - z_pred;
        while (z_diff(1) > M_PI)
            z_diff(1) -= 2.0 * M_PI;
        while (z_diff(1) < -M_PI)
            z_diff(1) += 2.0 * M_PI;

        Tc = Tc + weights_(i) * x_diff * z_diff.transpose();
    }

    // calculate Kalman gain K;
    MatrixXd K = MatrixXd(n_z, n_z);
    K = Tc * S.inverse();

    VectorXd z_diff = z_ - z_pred;
    while (z_diff(1) > M_PI)
        z_diff(1) -= 2.0 * M_PI;
    while (z_diff(1) < -M_PI)
        z_diff(1) += 2.0 * M_PI;
    // State update
    x_ = x_ + K * z_diff;
    P_ = P_ - K * S * K.transpose();
}

void UKF::UpdateRadar(MeasurementPackage meas_package)
{
    /**
     * TODO: Complete this function! Use radar data to update the belief
     * about the object's position. Modify the state vector, x_, and
     * covariance, P_.
     * You can also calculate the radar NIS, if desired.
     */
    // Step 1: Measurement Prediction
    // Lesson 25. Measurement Prediction
    // set measurement dimension, radar can measure r, phi, and r_dot
    int n_z = 3;
    // create matrix for sigma points in measurement space
    MatrixXd Zsig = MatrixXd(n_z, 2 * n_aug_ + 1);

    // transform sigma points into measurement space
    for (int i = 0; i < (2 * n_aug_ + 1); i++)
    {
        double p_x = Xsig_pred_(0, i);
        double p_y = Xsig_pred_(1, i);
        double v = Xsig_pred_(2, i);
        double yaw = Xsig_pred_(3, i);
        double yawd = Xsig_pred_(4, i);

        Zsig(0, i) = sqrt(p_x * p_x + p_y * p_y);                                             // r
        Zsig(1, i) = atan2(p_y, p_x);                                                         // phi
        Zsig(2, i) = (p_x * cos(yaw) * v + p_y * sin(yaw) * v) / sqrt(p_x * p_x + p_y * p_y); // r_dot
    }

    // calculate mean predicted measurement
    VectorXd z_pred = VectorXd::Zero(n_z);
    for (int i = 0; i < (2 * n_aug_ + 1); i++)
    {
        z_pred = z_pred + Zsig.col(i) * weights_(i);
    }

    // calculate innovation covariance matrix S
    MatrixXd S = MatrixXd::Zero(n_z, n_z);

    for (int i = 0; i < 2 * n_aug_ + 1; ++i)
    {
        VectorXd z_diff = Zsig.col(i) - z_pred;
        // angle normalization
        while (z_diff(1) > M_PI)
            z_diff(1) -= 2. * M_PI;
        while (z_diff(1) < -M_PI)
            z_diff(1) += 2. * M_PI;
        S = S + weights_(i) * z_diff * z_diff.transpose();
    }

    MatrixXd R = MatrixXd::Zero(n_z, n_z);
    R(0, 0) = std_radr_ * std_radr_;
    R(1, 1) = std_radphi_ * std_radphi_;
    R(2, 2) = std_radrd_ * std_radrd_;
    S = S + R;

    // Step 2: Update State
    // Lesson 28. UKF Update
    // Cross-correlation Matrix
    MatrixXd Tc = MatrixXd::Zero(n_x_, n_z);
    VectorXd z_ = meas_package.raw_measurements_;
    for (int i = 0; i < 2 * n_aug_ + 1; i++)
    {

        VectorXd x_diff = Xsig_pred_.col(i) - x_;
        while (x_diff(3) > M_PI)
            x_diff(3) -= 2. * M_PI;
        while (x_diff(3) < -M_PI)
            x_diff(3) += 2. * M_PI;

        VectorXd z_diff = Zsig.col(i) - z_pred;
        while (z_diff(1) > M_PI)
            z_diff(1) -= 2. * M_PI;
        while (z_diff(1) < -M_PI)
            z_diff(1) += 2. * M_PI;
        Tc = Tc + weights_(i) * x_diff * z_diff.transpose();
    }

    // calculate Kalman gain K;
    MatrixXd K = Tc * S.inverse();

    VectorXd z_diff = z_ - z_pred;
    while (z_diff(1) > M_PI)
        z_diff(1) -= 2.0 * M_PI;
    while (z_diff(1) < -M_PI)
        z_diff(1) += 2.0 * M_PI;
    // State update
    x_ = x_ + K * z_diff;
    P_ = P_ - K * S * K.transpose();
}