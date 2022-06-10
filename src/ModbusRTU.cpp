#include "ModbusRTU.h"

Modbus::Modbus(HardwareSerial &canal, uint32_t baudios, uint8_t configuracion)
    : _canal(canal)
{
    // Constructor para inicializar un objeto de la clase Modbus. Parámetros:
    // - canal: referencia al objeto que maneja la comunicación en la placa.
    // - baudios: velocidad de comunicación en bits/s, por defecto 9600
    // - configuracion: para indicar el número de bits/byte, paridad y bits de parada. Hay que
    // indicar una constante de las que figuran en la ayuda del método Serial.begin.
    // Por defecto SERIAL_8N1 para configurar el canal con 8 bits/byte, sin bit
    // de paridad y con 1 bit de parada.
    // Inicializa la referencia _canal para que apunte al mismo objeto indicado en el primer
    // parámetro.
    _baudios = baudios;
    _configuracion = configuracion;
    // Guarda en el objeto estos parámetros
    _t = millis(); // Guarda el instante actual
    _msReposoEntrePaquetes = 10.0 * 10.0 * 1000.0 / _baudios;
    // Entre dos transmisiones el canal tiene que estar en reposo durante al menos el tiempo
    // de transmisión de 3.5 bytes. En este caso vamos a mantener el canal en reposo durante
    // el tiempo de transmisión de 10 bytes. Por cada byte se envía un bit de comienzo,
    // 8 bits de datos y un bit de parada, total 10 bits. El tiempo de transmisión de un
    // bit en ms es 1000 / _baudios.
}
void Modbus::configura()
{
    // En el setup() del programa hay que llamar a este método para configurar el canal serie
    _canal.begin(_baudios, _configuracion); // Configura los parámetros
    while (!_canal)
        ; // Espera a que el canal esté listo
}
void Modbus::anadeCRC(uint8_t *paquete, uint16_t longitud)
{
    // Calcula el CRC16 del paquete apuntado por el puntero 'paquete', que contiene
    // 'longitud' bytes. Añade el CRC16 al final del paquete.
    uint16_t CRC16 = 0xFFFF; // Inicializa el CRC16 en una variable de 16 bits
    uint16_t i, k;
    for (k = 0; k < longitud; k++)
    {                                  // Aplica a todos los bytes de la matriz ...
        CRC16 ^= (uint16_t)paquete[k]; // Hace la XOR con el CRC16
        for (i = 0; i < 8; i++)
        { // Repite 8 veces ...
            if (((uint16_t)CRC16 & 0x0001) != 0)
            {                    // Si el bit menos significativo es un 1
                CRC16 >>= 1;     // Desplaza el CRC16 un bit hacia la derecha
                CRC16 ^= 0xA001; // y hace XOR con 0xA001
            }
            else             // Si no es un 1 ...
                CRC16 >>= 1; // Sólo desplaza
        }
    }
    paquete[longitud] = CRC16 & 0xFF;
    paquete[longitud + 1] = CRC16 >> 8;
    // Añade el CRC16 al final del paquete. Primero la parte baja y luego la alta.
}
void Modbus::escribeRegistro(uint8_t nodo, uint16_t registro, uint16_t valor)
{
    // Envía al nodo número 'nodo' una orden para que modifique su registro interno 'registro'
    // guardando ahí el dato indicado en 'valor'
    _paquete[0] = (uint8_t)nodo;              // Primero hay que poner el nodo
    _paquete[1] = 0x10;                       // Código de operación de escritura de registros
    _paquete[2] = (uint8_t)(registro >> 8);   // Parte alta
    _paquete[3] = (uint8_t)(registro & 0xFF); // y baja de la dirección del primer registro
    _paquete[4] = 0;                          // Parte alta
    _paquete[5] = 1;                          // y parte baja del número de registros
    _paquete[6] = 2;                          // Un byte que indica el número de bytes a enviar
    _paquete[7] = (uint8_t)(valor >> 8);      // Luego los 8 bits más significativos del dato
    _paquete[8] = (uint8_t)(valor & 0xFF);    // y a continuación los 8 menos significativos
    anadeCRC(_paquete, 9);                    // Calcula el CRC16 y lo añade al paquete
    while (millis() - _t < _msReposoEntrePaquetes)
        ;
    // Espera a que haya transcurrido el período de reposo entre transmisiones
    _canal.write(_paquete, 11); // Envía el paquete por el canal
    _canal.flush();             // Espera a que se vacíe el buffer de salida en el canal
    while (_canal.available() < 8)
        ;          // Espera a haber recibido 8 bytes como respuesta
    _t = millis(); // Marca este instante para empezar a contar el período de reposo
    for (int i = 0; i < 8; i++)
        _paquete[i] = _canal.read();
    // Extrae del objeto de comunicaciones los 8 bytes recibidos como respuesta
}

/***************************************************************************************/
/***************************** Lectura de registro *************************************/
/***************************************************************************************/

uint16_t Modbus::leeRegistro(uint8_t nodo, uint16_t registro)
{
    // Envía al nodo número 'nodo' una orden para que devuelva el valor almacenado en
    // su registro interno 'registro'. Este método devuelve el valor recibido.
    _paquete[0] = (uint8_t)nodo;              // Número del esclavo, está configurado para ser un 1 siempre
    _paquete[1] = 0x03;                       // Código para leer registros
    _paquete[2] = (uint8_t)(registro >> 8);   // Parte alta
    _paquete[3] = (uint8_t)(registro & 0xFF); // y baja de la dirección del primer registro
    _paquete[4] = 0;                          // Parte alta
    _paquete[5] = 1;
    anadeCRC(_paquete, 6);
    while (millis() - _t < _msReposoEntrePaquetes)
        ;
    // Espera a que haya transcurrido el período de reposo entre transmisiones
    _canal.write(_paquete, 8); // Envía el paquete por el canal
    _canal.flush();            // Espera a que se vacíe el buffer de salida en el canal
    while (_canal.available() < 7)
        ;          // Espera a haber recibido 8 bytes como respuesta
    _t = millis(); // Marca este instante para empezar a contar el período de reposo
    for (int i = 0; i < 7; i++)
    {
        _paquete[i] = _canal.read();
    }
    uint16_t auxiliar;
    auxiliar = (_paquete[3]<<8) + (_paquete[4]); // Desplazamos la parte alta 8 bits a la izquierda y sumamos directamente la parte baja
    return auxiliar;
}