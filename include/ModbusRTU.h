#ifndef MODBUS_H_
#define MODBUS_H_
#include <Arduino.h>
// Para utilizar clases de comunicación serie
class Modbus
{
    // Un objeto de esta clase representa a un canal de comunicación donde
    // se utiliza el protocolo Modbus-RTU para una placa Arduino Mega 2560
    // para que actúe como maestro en el canal.
private:
    HardwareSerial &_canal;
    // Referencia al objeto que maneja la comunicación en el canal serie
    byte _paquete[11];
    // Matriz de bytes donde se guardan los paquetes que se envían y reciben
    uint32_t _baudios; // Velocidad de comunicación en bits/s
    uint8_t _configuracion;
    // Configuración del canal serie en cuanto a bits/byte, paridad y bits parada
    // Guarda un valor (ejemplo, SERIAL_8N1) indicado por las mismas constantes
    // utilizadas en la inicialización de cualquier canal serie en Arduino
    uint16_t _msReposoEntrePaquetes;
    // Almacena cuántos ms tienen que transcurrir desde la última comunicación hasta
    // el comienzo de la siguiente. Corresponde, como mínimo, al envío de 3.5 bytes en
    // el canal.
    unsigned long _t; // Para medir tiempo
    void anadeCRC(uint8_t *paquete, uint16_t longitud);
    // Dado un paquete de bytes apuntado por 'paquete', que consta de 'longitud' bytes,
    // este método calcula su CRC16 y lo añade al final del paquete
public:
    Modbus(HardwareSerial &canal, uint32_t baudios = 9600,
           uint8_t configuracion = SERIAL_8N1);
    // Constructor al que se le indica:
    // - canal = el canal serie utilizado en la placa Arduino
    // - baudios = velocidad de comunicación en bits/s, por defecto 9600
    // - configuracion = configuración de la comunicación, por defecto con 8 bits/byte,
    // sin paridad y con 1 bit de parada
    void configura();
    // Hay que llamar a este método en la función setup() para inicializar el canal
    void escribeRegistro(uint8_t nodo, uint16_t registro, uint16_t valor);
    // Envía una orden al esclavo indicado por 'nodo' para modificar el registro interno
    // indicado por 'registro' y guardar en él el dato indicado por 'valor'
    // (un entero de 16 bits sin signo)
    uint16_t leeRegistro(uint8_t nodo, uint16_t registro);
    // Envía una orden al esclavo indicado por 'nodo' para leer su registro número 'registro'.
    // Devuelve el valor leído (un entero sin signo de 16 bits).
};
#endif