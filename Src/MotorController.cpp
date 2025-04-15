/*
 * MotorController.cpp
 *
 *  Created on: Apr 14, 2025
 *      Author: Yasmine Salmouni
 */

 #include "../Inc/MotorController.hpp"

 MotorController::MotorController(UART_HandleTypeDef* controlUart, UART_HandleTypeDef* screenUart, float defaultLinearGain)
     : control_uart(controlUart),
     screen_uart(screenUart),
     direction(DirectionMode::FORWARD),
     controlMode(ControlMode::CADENCE),
     instruction(0.0f),
     linearGain(defaultLinearGain),
     torqueConstant(0.05f)
 {
     screen = new ScreenDisplay(screen_uart);
     vesc = new VESCInterface(control_uart);
 }
 //Par défaut le moteur est en modes forward et cadence avec une vitesse nulle
 
 void MotorController::setDirection(DirectionMode dir) {
     direction = dir;
 }
 
 void MotorController::setControlMode(ControlMode mode) {
     controlMode = mode;
 }
 
 void MotorController::setInstruction(float value) {
     instruction = value;
     if (controlMode == ControlMode::LINEAR) return;  // linear se gère dynamiquement
     switch (controlMode) {
         case ControlMode::CADENCE:
             setCadence(value);
             break;
         case ControlMode::TORQUE:
             setTorque(value);
             break;
         case ControlMode::POWER_CONCENTRIC:
             setPowerConcentric(value);
             break;
         case ControlMode::POWER_ECCENTRIC:
             setPowerEccentric(value);
             break;
         default:
             break;
     }
 }
 
 void MotorController::setLinearGain(float gain)  
 {
     linearGain = gain;
 }

 void MotorController::settorqueConstant(float torque)  
 {
    torqueConstant = torque;
 }
 
 void MotorController::setCadence(float rpm, float rampRate) //Implémenter une version avec rampRate
 {
     float value = applyDirection(rpm);
     //sendCommand("v 0 %.2f\n", value);
     vesc->setRPM(value);
 }
 
 void MotorController::setTorque(float torque, float rampRate) //Implementer une version avec rampRate
 {
     float value = applyDirection(torque);
     //sendCommand("c 0 %.2f\n", value);
     vesc->setCurrent(value);
 }
 
 float MotorController::getCadence() {
     memset(rx_buffer, 0, sizeof(rx_buffer));  // vide le buffer
 
     sendCommand("r axis0.encoder.vel_estimate\n");  // commande à ODrive
 
     // Lire la réponse (ex: "15.35\n")
     if (HAL_UART_Receive(control_uart, (uint8_t*)rx_buffer, sizeof(rx_buffer), 100) == HAL_OK) {
         float vel_rad_s = atof(rx_buffer);  // conversion ASCII → float
         float cadence_rpm = vel_rad_s * 60.0f / (2.0f * M_PI);
         return cadence_rpm;
     } else {
         // en cas d’échec
         // Affiche un message d'erreur sur l'ecran
         const char* msg = "t0.txt=\"Erreur: réception cadence\"";
         HAL_UART_Transmit(screen_uart, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
         uint8_t end_cmd[3] = {0xFF, 0xFF, 0xFF};
         HAL_UART_Transmit(screen_uart, end_cmd, 3, HAL_MAX_DELAY);
         // Retourne une valeur spéciale
         return -1.0f;
     }
 }
 
 void MotorController::setPowerConcentric(float power, float rampRate)
 {
     float cadence = getCadence();  // Lecture de la vitesse réelle
 
     // Vérifie si getCadence() a échoué (renvoie une valeur d’erreur)
     if (cadence < 0.0f)
     {
         // Option : afficher un message d'erreur sur l'écran
         const char* msg = "Erreur: réception cadence";
         screen->showError(msg);
         return;
 
     }
     // Sécurité : éviter division par zéro
     //Même si l’utilisateur pédale à 0 tr/min, on agit comme si c’était au moins 1 tr/min
     if (cadence < 1.0f)
     {
         cadence = 1.0f;
     }
 
     // Conversion cadence → vitesse angulaire ω (en rad/s)
     float omega = cadence * 2.0f * M_PI / 60.0f;
 
     // Calcul du couple réel à appliquer
     float torque = power / omega;
 
     // Appliquer le sens (FORWARD = + / REVERSE = -)
     torque = applyDirection(torque);
 
     sendCommand("c 0 %.2f\n", torque);
 }
 
 
 void MotorController::setPowerEccentric(float power, float rampRate)
 {
     float cadence = getCadence();  // Lecture de la vitesse réelle
 
     //Vérifie si la lecture a échoué (getCadence retourne -1.0 en cas d'erreur)
     if (cadence < 0.0f)
     {
         // Option : afficher un message d'erreur sur l'écran
         const char* msgBIS = "Erreur: réception cadence";
         screen->showError(msgBIS);
         return;
     }
 
     // Sécurité : éviter division par zéro
     //Même si l’utilisateur pédale à 0 tr/min, on agit comme si c’était au moins 1 tr/min
     if (cadence < 1.0f) {
         cadence = 1.0f;
     }
 
     // Conversion cadence → vitesse angulaire (rad/s)
     float omega = cadence * 2.0f * M_PI / 60.0f;
 
     // Calcul du couple nécessaire (négatif en excentrique)
     float torque = -power / omega;
 
     // Applique la direction choisie (REVERSE ou FORWARD)
     torque = applyDirection(torque);
 
     sendCommand("c 0 %.2f\n", torque);
 }
 
 
 void MotorController::setLinear(float gain, float cadence) {
     linearGain = gain;
     float torque = linearGain * cadence;
     float value = applyDirection(torque);
     sendCommand("c 0 %.2f\n", value);
 }
 
 void MotorController::update(float measured_cadence) {
     if (controlMode == ControlMode::LINEAR) {
         setLinear(linearGain, measured_cadence);
     }
 }
 
 void MotorController::stop() {
     sendCommand("v 0 0\n");
 }
 
 void MotorController::sendCommand(const char* format, ...) //Les ... permettent de passer un nombre variable d’arguments à la fonction.
                                                             //C’est ce qu’on appelle une fonction variadique.
 {
     char buffer[64]; //en C++ char == caractère ASCII
     va_list args; //args liste d’arguments variables
     va_start(args, format); //Le `format` contient la forme (`"v 0 %.2f\n"`), et les valeurs (ex: `30.0`) viennent ensuite
     vsnprintf(buffer, sizeof(buffer), format, args); // On remplit buffer avec le texte complet :
                                                     //→ on remplace %.2f dans le format par la vraie valeur (ex: 30.00)
                                                     //Résultat dans buffer : "v 0 30.00\n"
     va_end(args); //On termine la lecture de args
     HAL_UART_Transmit(control_uart, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY); //On envoie le contenu de buffer à l’ODrive via l’UART. uart = le port série (USART1)
 }
 
 float MotorController::applyDirection(float value) {
     return (direction == DirectionMode::REVERSE) ? -value : value;
 }
 
 /*if (direction == DirectionMode::REVERSE)
     return -value;
 else
     return value;*/
 
 
 
 