/*
 * MockMotorController.hpp
 *
 *  Created on: Apr 22, 2025
 *      Author: Administrateur
 */

 #pragma once

 #include <iostream>
 #include <cmath>
 
 class MockVESCInterface;
 #include "MockVESCInterface.hpp"
 
 class MockScreenDIsplay;
 #include "MockScreenDisplay.hpp"
 
 class MotorComputations;
 #include "MotorComputations.hpp"
 
 enum class DirectionMode { FORWARD, REVERSE };
 enum class ControlMode {
     TORQUE,
     CADENCE,
     POWER_CONCENTRIC,
     POWER_ECCENTRIC,
     LINEAR
 };
 
 class MockMotorController {
 public:
     MockMotorController(UART_HandleTypeDef* ctrl = nullptr, UART_HandleTypeDef* scr = nullptr, float kt = 1.0f)
         : control_uart(ctrl),
           screen_uart(scr),
           direction(DirectionMode::FORWARD),
           controlMode(ControlMode::CADENCE),
           instruction(0.0f),
           linearGain(0.05f),
           torqueConstant(kt),
           lastAppliedCurrent(0.0f),
           ramp(6.0f),
           screen(new MockScreenDisplay(nullptr)),
           vesc(new MockVESCInterface(nullptr)),
           computations(kt) {}
 
     void stop(float rampRate = 6.0f) {
         std::cout << "[MockCTRL] Stop moteur avec rampe de " << rampRate << " A/s" << std::endl;
         vesc->setCurrent(0.0f);
     }
 
     void setTorque(float torque, float = 6.0f) {
         float effective = applyDirection(torque);
         float current = computations.computeCurrentFromTorque(effective);
         vesc->setCurrent(current);
     }
 
     void setCadence(float rpm, float = 6.0f) {
         float value = applyDirection(rpm);
         vesc->setRPM(static_cast<int32_t>(value));
     }
 
     float getCadence() { return vesc->getRPM(); }
 
     float getTorque() {
         float current = vesc->getCurrent();
         return computations.computeTorqueFromCurrent(current);
     }
 
     float getDutyCycle() { return vesc->getDutyCycle(); }
 
     float getPower() {
         float torque = getTorque();
         float rpm = getCadence();
         return computations.computePower(torque, rpm);
     }
 
     float getGain() { return linearGain; }
 
     ControlMode getControlMode() { return controlMode; }
 
     void setPowerConcentric(float power, float = 6.0f) {
         float rpm = getCadence();
         float omega = computations.computeOmega(rpm < 1.0f ? 1.0f : rpm);
         float torque = power / omega;
         setTorque(torque);
     }
 
     void setPowerEccentric(float power, float = 6.0f) {
         float rpm = getCadence();
         float omega = computations.computeOmega(rpm < 1.0f ? 1.0f : rpm);
         float torque = -power / omega;
         setTorque(torque);
     }
 
     void setLinear(float gain, float cadence) {
         linearGain = gain;
         float torque = gain * cadence;
         setTorque(torque);
     }
 
     void update(float measured_cadence) {
         if (controlMode == ControlMode::LINEAR) {
             setLinear(linearGain, measured_cadence);
         }
     }
 
     void setDirection(DirectionMode dir) { direction = dir; }
     void setControlMode(ControlMode mode) { controlMode = mode; }
     void setInstruction(float value) { instruction = value; }
     void setLinearGain(float gain) { linearGain = gain; }
     void setrampRate(float rampRate) { ramp = rampRate; }
     void settorqueConstant(float kt) {
         torqueConstant = kt;
         computations.setTorqueConstant(kt);
     }
 
     void updateFromScreen() {
         screen->showWelcome();
         float torque = screen->getUserTorque();
         setInstruction(torque);
         std::cout << "[MockCTRL] Mise à jour depuis écran" << std::endl;
     }
 
     void updateScreen() {
         screen->showTorque(getTorque());
         screen->showCadence(getCadence());
         screen->showPower(getPower());
         screen->showMode(getControlMode());
     }
 
 private:
     UART_HandleTypeDef* control_uart;
     UART_HandleTypeDef* screen_uart;
 
     DirectionMode direction;
     ControlMode controlMode;
     float instruction;
     float linearGain;
     float torqueConstant;
     float lastAppliedCurrent;
     float ramp;
 
     MockScreenDisplay* screen;
     MockVESCInterface* vesc;
     MotorComputations computations;
 
     float applyDirection(float val) {
         return (direction == DirectionMode::REVERSE) ? -val : val;
     }
 };
 
 
 
 
 