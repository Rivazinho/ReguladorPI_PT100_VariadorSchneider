#ifndef CONTROLPI_H_
#define CONTROLPI_H_
#include <Arduino.h>

class ControlPI
{

private:
    unsigned long Ts;            // Variable para almacenar el periodo de muestreo
    uint16_t ActMax;             // Variable para limitar la actuación máxima
    uint16_t ActMin;             // Variable para limitar la actuación mínima
    double Kp;                   // Constante proporcional
    double Ki;                   // Constante integral
    int16_t ErrorAct;            // Variable para almacenar el error actual
    int16_t ErrorAnt = 0;        // Variable para almacenar el error anterior
    float velocidadAnterior = 0; // Variable para almacenar el valor anterior de la velocidad

public:
    ControlPI(uint16_t _ActMax, uint16_t _ActMin, double _Kp, double _Ki, unsigned long _Ts); // Constructor de la clase ControlPI
    float velocidadRegulada(uint16_t Consigna, uint16_t TempActual);                          // Variable para almacenar la nueva velocidad
};

#endif
