#include "Altivar.h"
#include "ModbusRTU.h"

// Constructor de la clase Altivar
Altivar::Altivar(uint16_t regCMD, uint16_t regVelocidad, uint16_t regEntradaAnalogica,
            uint8_t nodo, Modbus &canal): _canal(canal)
            {
                _regCMD=regCMD;
                _regVelocidad=regVelocidad;
                _regEntradaAnalogica=regEntradaAnalogica;
                _nodo=nodo;

            }

void Altivar::configura()
{
    uint16_t Apagado = 0x6;
    uint16_t Encendido = 0x7;
    uint16_t Habilitado = 0xf;
    uint16_t Reinicio = 0x80;

    _canal.escribeRegistro (_nodo, _regCMD, Reinicio);
    _canal.escribeRegistro (_nodo, _regCMD, Apagado);
    _canal.escribeRegistro (_nodo, _regCMD, Encendido);
    _canal.escribeRegistro (_nodo, _regCMD, Habilitado);
    //Serial.println("Registros escritos");
}

void Altivar::setVelocidad(float Velocidad)
{
    _canal.escribeRegistro(_nodo, _regVelocidad, (uint16_t)(Velocidad*10));
}

uint16_t Altivar::getEntradaAnalogicaCorriente()
{
    return _canal.leeRegistro(_nodo, _regEntradaAnalogica);
}

Altivar28::Altivar28 (uint8_t nodo, Modbus &canal) : Altivar (400, 401, 480, 1, canal)
{
    // Constructor Altivar 28
}

Altivar31::Altivar31 (uint8_t nodo, Modbus &canal) : Altivar (8501, 8502, 5244, 1, canal)
{
    // Constructor Altivar 31
}