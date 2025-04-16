/*
 * MotorController.hpp
 *
 *  Created on: Apr 16, 2025
 *      Author: Yasmine Salmouni
 */

 #pragma once
 using namespace std;

 #include <cmath>

 class MotorComputations {
    public:
        MotorComputations(); 
    
        float computeTorqueFromCurrent(float current) const;
        float computeCurrentFromTorque(float torque) const;
    
        float computePower(float torque, float cadence_rpm) const;
        float computeOmega(float cadence_rpm) const;
    
    private:
        float torqueConstant;  // en Nm/A
    };
    