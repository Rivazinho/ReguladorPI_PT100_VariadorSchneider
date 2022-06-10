#include "ControlPI.h"

/***************************************************************************************/
/************************************ Regulador PI *************************************/
/***************************************************************************************/

ControlPI::ControlPI(uint16_t _ActMax, uint16_t _ActMin, double _Kp, double _Ki, unsigned long _Ts) // Definición de la clase ControlPI
{

    if (_ActMax < 100)
        ActMax = _ActMax;
    else
        ActMax = 100;
    if (_ActMin > 10)
        ActMin = _ActMin;
    else
        ActMin = 10;

    Kp = _Kp;
    Ki = _Ki;
    Ts = _Ts;
};

float ControlPI::velocidadRegulada(uint16_t Consigna, uint16_t TempActual) // Cálculo del regulador
{

    ErrorAct = Consigna - TempActual;                                                              // Cálculo del error
    float velocidadRegulada = velocidadAnterior + Kp * (ErrorAct - ErrorAnt) + Ki * Ts * ErrorAct; // Cálculo de la velocidad regulada

    if (velocidadRegulada > ActMax)
        velocidadRegulada = ActMax; // Limitación a 100 Hz
    if (velocidadRegulada < ActMin)
        velocidadRegulada = ActMin; // Limitación a 10 Hz

    ErrorAnt = ErrorAct;                   // Salva el valor para el siguiente cálculo
    velocidadAnterior = velocidadRegulada; // Salva el valor para el siguiente cálculo
    return velocidadRegulada;              // Devuelve la velocidad regulada
}
