/*
 * ScreenDisplay.hpp
 *
 *  Created on: Apr 14, 2025
 *      Author: Yasmine Salmouni
 */

 #pragma once
 using namespace std;
 
 #include "main.h"
 
 #include <cstring>
 #include <cstdio>
 #include <cstdint>
 
 
 /**
  * @brief Classe pour gérer la communication avec un écran Nextion via UART
  * Adaptée pour un écran Enhanced NX4832K035
  */
 
 class ScreenDisplay {
 public:
 
     ScreenDisplay(UART_HandleTypeDef* EcranUart);
 
     // Affichage des valeurs dynamiques
     void showCadence(float rpm);
     void showTorque(float torque);
     void showPower(float power);
     void showMode(const char* modeName);
 
     // Affichage de messages statiques
     void showError(const char* message);
     void showWelcome();
     void clearScreen();
 
 private:
     UART_HandleTypeDef* ecran_uart;
 
     // Méthodes internes d'envoi
     void sendCommand(const char* cmd);
     void sendText(const char* component, const char* message);
     void sendValue(const char* component, float value, const char* format = "%.1f");
 };
 
 
 
 