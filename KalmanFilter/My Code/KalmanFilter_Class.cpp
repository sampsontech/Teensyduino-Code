/*
* Kalman Filter Classe
*
* Library for simple Kalman filter
*
* Requirements: -
*
* TODO:  - 
*
* REF: - Created by Max Lunin http://max.lunin.info : <nut.code.monkey@gmail.com>
*      - http://en.wikipedia.org/wiki/Kalman_filter
*
*
* USAGE:
*   KalmanFilter kalmanFilter;            // create/declare with default parameters
*
*   kalmanFilter.setState(startValue);
*   kalmanFilter.setCovariance(0.1);      // optional 
*
*   while (true)
*   {
*       double value = getValueFromSensor();
*       kalmanFilter.correct(value);
*       double correctedValue = kalmanFilter.getState();
*   }
*/

#ifndef KALMAN_FILTER_CLASS
#define KALMAN_FILTER_CLASS

#include "Arduino.h"


class KalmanFilter {
public:
    KalmanFilter(double q = 1, double r = 1, double f = 1, double h = 1) {
    F = f;
    Q = q;
    H = h;
    R = r;
}

    double getState() const { return state; }

    void setState(double state) { this->state = state; }
    
    void setCovariance(double covariance) { this->covariance = covariance; }
     
    void correct(double data) {
    x0 = F * state;
    p0 = F * F * covariance + Q;
    
    //measurement update - correction
    double K = (H * p0) / (H * p0 * H + R);
    state = x0 + K * (data - H * x0);
    covariance = (1 - K * H) * p0;
}

public:
    double getCovariance() const { return covariance; }
    double getX0() const { return x0; }
    double getP0() const  { return p0; }
    double getF() const { return F; }
    double getQ() const { return Q; }
    double getH() const  { return H; }
    double getR() const { return R; }
    
private:
    double x0; // predicted state
    double p0; // predicted covariance
    double F; // factor of real value to previous real value
    double Q; // measurement noise
    double H; // factor of measured value to real value
    double R; // environment noise
    double state;
    double covariance;
};

#endif    // KALMAN_FILTER_CLASS

