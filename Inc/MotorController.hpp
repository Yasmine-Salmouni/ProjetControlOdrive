/*
 * MotorController.hpp
 *
 *  Created on: Apr 14, 2025
 *      Author: Yasmine Salmouni
 */

 #pragma once
 using namespace std;
 
 #include <cstdarg>
 #include <cstdio>
 #include <cstring>
 #include <cmath>
 
 #include "ScreenDisplay.hpp"
 #include "VESCInterface.hpp"
 #include "MotorComputations.hpp"
 
 enum class DirectionMode {
     FORWARD,
     REVERSE
 };
 
 enum class ControlMode {
     TORQUE,
     CADENCE,
     POWER_CONCENTRIC, 
     POWER_ECCENTRIC,
     LINEAR
 };
 
 class MotorController {
 public:
     MotorController(UART_HandleTypeDef* controlUart, UART_HandleTypeDef* screenUart, float torqueConstant);
 
     void stop(float rampRate = 6.0f);
 
     void setTorque(float torque, float rampRate = 6.0f); //réecrire la fonction pour respecter le ramprate
     void setCadence(float rpm, float rampRate = 6.0f); //réecrire la fonction pour respecter le ramprate
 
     float getCadence();
     float getTorque();
     float getDutyCycle();
     float getPower();
     float getGain();
     ControlMode getControlMode();
     void setPowerConcentric(float power, float rampRate = 6.0f); //réecrire la fonction pour respecter le ramprate
     void setPowerEccentric(float power, float rampRate = 6.0f); //réecrire la fonction pour respecter le ramprate
     void setLinear(float gain, float cadence);
     void update(float measured_cadence);  // à appeler à chaque boucle, ex: toutes les 100ms

     void setDirection(DirectionMode dir);
     void setControlMode(ControlMode mode);
     void setInstruction(float value);
     void setLinearGain(float gain);
     void setrampRate(float rampRate);
     void settorqueConstant(float torque);


     void updateFromScreen();
     void updateScreen();
 
 private:
     UART_HandleTypeDef* control_uart;
     UART_HandleTypeDef* screen_uart;
 
     DirectionMode direction;
     ControlMode controlMode;
     float instruction; //la valeur cible que l’on veut imposer au moteur, en fonction du mode actif.
     float linearGain;  //Pour le mode linéaire
     float torqueConstant;  // Nm/A
     float lastAppliedCurrent;
     float ramp;

 
     char rx_buffer[32];  // tampon pour lire les réponses UART
 
     ScreenDisplay* screen;
     VESCInterface* vesc;
     MotorComputations computations;
 
     float applyDirection(float value);
 };
 