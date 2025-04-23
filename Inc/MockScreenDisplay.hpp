/*
 * MockScreenDisplay.hpp
 *
 *  Created on: Apr 22, 2025
 *      Author: Administrateur
 */

 #pragma once

 #include <iostream>
 #include <string>
 #include <cstdint>
 
 class MotorController;
 #include "MotorController.hpp"
 
 /**
  * Mock de ScreenDisplay pour tests sans Nextion.
  * Affiche les interactions via la console (printf).
  */
 class MockScreenDisplay {
 public:
     MOckScreenDisplay(UART_HandleTypeDef* uart = nullptr) {
         (void)uart; // inutilisé dans le mock
     }
 
     // --- Affichage dynamique simulé ---
     void showCadence(float rpm) {
         printf("[Écran] Cadence: %.1f tr/min\n", rpm);
     }
 
     void showTorque(float torque) {
         printf("[Écran] Couple: %.2f Nm\n", torque);
     }
 
     void showPower(float power) {
         printf("[Écran] Puissance: %.2f W\n", power);
     }
 
     void showMode(const char* modeName) {
         printf("[Écran] Mode: %s\n", modeName);
     }
 
     void showMode(ControlMode mode) {
         const char* str = "Inconnu";
         switch (mode) {
             case ControlMode::CADENCE:          str = "Cadence"; break;
             case ControlMode::TORQUE:           str = "Couple"; break;
             case ControlMode::POWER_CONCENTRIC: str = "Power Conc."; break;
             case ControlMode::POWER_ECCENTRIC:  str = "Power Excent."; break;
             case ControlMode::LINEAR:           str = "Linéaire"; break;
             default: break;
         }
         showMode(str);
     }
 
     void showGain(float gain) {
         printf("[Écran] Gain linéaire: %.2f\n", gain);
     }
 
     void showDutyCycle(float duty) {
         printf("[Écran] Duty Cycle: %.1f%%\n", duty * 100.0f);
     }
 
     void showDirection(DirectionMode dir) {
         const char* label = (dir == DirectionMode::REVERSE) ? "REVERSE" : "FORWARD";
         printf("[Écran] Direction: %s\n", label);
     }
 
     // --- Messages statiques simulés ---
     void showError(const char* message) {
         printf("[Écran - ERREUR]: %s\n", message);
     }
 
     void showWelcome() {
         printf("[Écran] Ergocycle S2M prêt !\n");
     }
 
     void clearScreen() {
         printf("[Écran] --- Écran effacé ---\n");
     }
 
     // --- Entrées utilisateur simulées ---
     int32_t readInt32() {
         int32_t val;
         printf("→ [Simu Écran] Entrez un int32 : ");
         std::cin >> val;
         return val;
     }
 
     float getUserCadence() {
         float rpm;
         printf("→ Cadence cible (tr/min) : ");
         std::cin >> rpm;
         return rpm;
     }
 
     float getUserPower() {
         float power;
         printf("→ Puissance cible (W) : ");
         std::cin >> power;
         return power;
     }
 
     float getUserTorque() {
         float torque;
         printf("→ Couple cible (Nm) : ");
         std::cin >> torque;
         return torque;
     }
 
     float getUserLinearGain() {
         float gain;
         printf("→ Gain linéaire : ");
         std::cin >> gain;
         return gain;
     }
 
     ControlMode getMode() {
         int val;
         printf("→ Mode (0:CAD, 1:TOR, 2:P_CONC, 3:P_ECC, 4:LIN): ");
         std::cin >> val;
         return static_cast<ControlMode>(val);
     }
 
     bool getStop() {
         int stop;
         printf("→ Stop demandé ? (0:non, 1:oui): ");
         std::cin >> stop;
         return stop == 1;
     }
 
     bool getCalibrateRequest() {
         int req;
         printf("→ Calibration demandée ? (0:non, 1:oui): ");
         std::cin >> req;
         return req == 1;
     }
 
 private:
     // Pas d’UART ici → tout est simulé
     void sendCommand(const char* cmd) {
         printf("[SendCmd] %s\n", cmd);
     }
 
     void sendText(const char* component, const char* message) {
         printf("[Écran] %s = \"%s\"\n", component, message);
     }
 
     void sendValue(const char* component, float value, const char* format = "%.1f") {
         char buffer[64];
         snprintf(buffer, sizeof(buffer), format, value);
         sendText(component, buffer);
     }
 };
 
 
 
 
 