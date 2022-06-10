#ifndef ALTIVAR_H_
#define ALTIVAR_H_
#include "ModbusRTU.h" // Para utilizar la clase Modbus
class Altivar
{
    // Un objeto de esta clase representa a un variador de velocidad Schneider Altivar
    uint16_t _regCMD, _regVelocidad, _regEntradaAnalogica;
    uint8_t _nodo; 
    Modbus &_canal;
public:
    Altivar(uint16_t regCMD, uint16_t regVelocidad, uint16_t regEntradaAnalogica,
            uint8_t nodo, Modbus &canal);
    // Constructor. Parámetros:
    // - regCMD = Número de registro que permite modificar el modo de funcionamiento
    // - regVelocidad = Número de registro para establecer la velocidad
    // - regEntradaAnalogica = Número de registro para leer la entrada analógica en corriente
    // - nodo = número de nodo del variador en el canal Modbus
    // - canal = referencia al objeto de la clase Modbus que controla las comunicaciones
    void configura();
    // A utilizar en la función setup() para inicializar el variador
    void setVelocidad(float velocidad);
    // Establece la velocidad del motor. Se indica en Hz la frecuencia de la señal trifásica
    // generada por el variador. Puede tomar valores entre 10 y 100Hz
    uint16_t getEntradaAnalogicaCorriente();
    // Solicita al variador una medida de la corriente de su entrada analógica.
    // Es una entrada de rango 0 a 20 mA. Se devuelve en micro Amperios, es decir,
    // rango 0 a 20000.
};
class Altivar28 : public Altivar
{
    // Clase derivada para manejar un variador modelo Altivar28
public:
    Altivar28(uint8_t nodo, Modbus &canal);
    // Constructor para indicar:
    // - nodo = número de nodo del variador en el canal Modbus
    // - canal = referencia al objeto de la clase Modbus que controla las comunicaciones
};
class Altivar31 : public Altivar
{
    // Clase derivada para manejar un variador modelo Altivar31
public:
    Altivar31(uint8_t nodo, Modbus &canal);
    // Constructor para indicar:
    // - nodo = número de nodo del variador en el canal Modbus
    // - canal = referencia al objeto de la clase Modbus que controla las comunicaciones
};
#endif