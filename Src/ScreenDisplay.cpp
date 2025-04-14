/*
 * ScreenDisplay.cpp
 *
 *  Created on: Apr 14, 2025
 *      Author: Yasmine Salmouni
 */


 #include "../Inc/ScreenDisplay.hpp"


 ScreenDisplay::ScreenDisplay(UART_HandleTypeDef* EcranUart) : ecran_uart(EcranUart) {}
 
 void ScreenDisplay::sendCommand(const char* cmd) {
     HAL_UART_Transmit(ecran_uart, (uint8_t*)cmd, strlen(cmd), HAL_MAX_DELAY);
     uint8_t end[3] = {0xFF, 0xFF, 0xFF};
     HAL_UART_Transmit(ecran_uart, end, 3, HAL_MAX_DELAY);
 }
 
 void ScreenDisplay::sendText(const char* component, const char* message) {
     char buffer[64];
     snprintf(buffer, sizeof(buffer), "%s.txt=\"%s\"", component, message);
     sendCommand(buffer);
 }
 
 void ScreenDisplay::sendValue(const char* component, float value, const char* format)
 // Afficher un nombre (float) dans un champ texte (t1, cad, pow, etc.) sur l’écran Nextion,
 //en utilisant un format personnalisé (ex : %.1f ou %.2f).
 {
     char valueStr[32];
     snprintf(valueStr, sizeof(valueStr), format, value);
 
     char buffer[64];
     snprintf(buffer, sizeof(buffer), "%s.txt=\"%s\"", component, valueStr);
     sendCommand(buffer);
 }
 
 // --- Fonctions spécifiques de haut niveau ---
 
 void ScreenDisplay::showCadence(float rpm) {
     sendValue("cad", rpm);  // champ texte nommé "cad"
 }
 
 void ScreenDisplay::showTorque(float torque) {
     sendValue("tor", torque);  // champ texte nommé "tor"
 }
 
 void ScreenDisplay::showPower(float power) {
     sendValue("pow", power);  // champ texte nommé "pow"
 }
 
 void ScreenDisplay::showMode(const char* modeName) {
     sendText("mode", modeName);  // champ texte nommé "mode"
 }
 
 void ScreenDisplay::showError(const char* message) {
     sendText("err", message);  // champ texte nommé "err"
 }
 
 void ScreenDisplay::showWelcome() {
     sendText("t0", "Ergocycle S2M Ready!");
 }
 
 void ScreenDisplay::clearScreen() {
     sendCommand("cls BLACK");  // Efface l'écran
 }
 
 
 
 
 
 