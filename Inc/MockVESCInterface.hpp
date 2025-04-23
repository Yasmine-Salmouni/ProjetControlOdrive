/*
 * MockVESCInterface.hpp
 *
 *  Created on: Apr 22, 2025
 *      Author: Administrateur
 */

 #pragma once

 #include <iostream>
 #include <cstdint>
 
 /**
  * Mock de VESCInterface pour simulation sans VESC.
  */
 class MockVESCInterface {
 public:
     explicit MockVESCInterface(UART_HandleTypeDef* = nullptr) {}
 
     void setCurrent(float current) {
         std::cout << "[VESC] Commande courant: " << current << " A" << std::endl;
         lastCurrent = current;
     }
 
     void setRPM(int32_t rpmVal) {
         std::cout << "[VESC] Commande RPM: " << rpmVal << " tr/min" << std::endl;
         simulatedRPM = static_cast<float>(rpmVal);
     }
 
     bool getValues() {
         std::cout << "[VESC] getValues appelé" << std::endl;
         return true;
     }
 
     float getRPM() {
         std::cout << "[VESC] Lecture RPM simulé: " << simulatedRPM << std::endl;
         return simulatedRPM;
     }
 
     float getCurrent() {
         std::cout << "[VESC] Lecture courant simulé: " << lastCurrent << std::endl;
         return lastCurrent;
     }
 
     float getDutyCycle() {
         std::cout << "[VESC] Lecture duty cycle simulé: " << duty << std::endl;
         return duty;
     }
 
 private:
     float simulatedRPM = 60.0f;  // valeur fictive
     float lastCurrent = 1.5f;    // courant simulé
     float duty = 0.25f;          // 25%
 };
 
 
 
 
 