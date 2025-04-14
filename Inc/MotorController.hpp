#pragma once
using namespace std;

#include "main.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cmath>


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
    MotorController(UART_HandleTypeDef* controlUart, UART_HandleTypeDef* screenUart, float defaultLinearGain = 0.05f);

    void setDirection(DirectionMode dir);
    void stop();

    void setTorque(float torque, float rampRate = 6.0f);
    void setCadence(float rpm, float rampRate = 6.0f);

    float getCadence();
    void setPowerConcentric(float power, float rampRate = 6.0f);
    void setPowerEccentric(float power, float rampRate = 6.0f);
    void setLinear(float gain, float cadence);

    void setControlMode(ControlMode mode);
    void setInstruction(float value);
    void setLinearGain(float gain);
    void update(float measured_cadence);  // à appeler à chaque boucle, ex: toutes les 100ms

private:
    UART_HandleTypeDef* control_uart;
    UART_HandleTypeDef* screen_uart;

    DirectionMode direction;
    ControlMode controlMode;
    float instruction; //la valeur cible que l’on veut imposer au moteur, en fonction du mode actif.
    float linearGain;  //Pour le mode linéaire

    char rx_buffer[32];  // tampon pour lire les réponses UART

    void sendCommand(const char* format, ...);
    float applyDirection(float value);
};

