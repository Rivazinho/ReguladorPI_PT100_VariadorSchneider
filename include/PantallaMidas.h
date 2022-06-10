#ifndef PANTALLAMIDAS_H_
#define PANTALLAMIDAS_H_
#include <Arduino.h>
class PantallaMidas
{
    // Clase para el manejo de una pantalla LCD alfanumérica de 4 filas y 40 columnas modelo
    // Midas MC44005A6W-FPTLW desde una placa Arduino. Se configura un bus de datos de 4 bits.
private: // Recursos privados a esta clase
    uint8_t _DB4, _DB5, _DB6, _DB7, _E1, _E2, _RS;
    // Número de señales Arduino utilizadas para la conexión a la pantalla
    uint8_t _fila;
    // Última fila posicionada 1...4
    void enviaOrden1(uint8_t orden);
    // Envía una orden al controlador 1 de la pantalla
    void enviaOrden2(uint8_t orden);
    // Envía una orden al controlador 2 de la pantalla
    void enviaOrden12(uint8_t orden);
    // Envía una orden a los controladores 1 y 2 de la pantalla
    void busDatos4Bits(uint8_t dato);
    // Establece un valor para el bus de datos, utilizando los 4 bits
    // menos significativos del parámetro
    void pulsoE1();
    // Pulso a nivel alto en señal E1 para transferencia con el controlador 1
    void pulsoE2();
    // Pulso a nivel alto en señal E1 para transferencia con el controlador 1
public: // Miembros públicos de esta clase
    PantallaMidas(uint8_t DB4, uint8_t DB5, uint8_t DB6, uint8_t DB7, uint8_t E1,
                  uint8_t E2, uint8_t RS);
    // Constructor al que se le indica qué señales Arduino están conectadas a las
    // señales de la pantalla
    void configura();
    // Método que hay que utilizar en setup() para configurar e inicializar los
    // controladores de la pantalla
    void borra();
    // Borra toda la información mostrada en la pantalla
    void posiciona(uint8_t fila, uint8_t columna);
    // Posiciona el cursor en una fila (1...4) y columna (1...40)
    void escribeCadena(const char *cadena);
    // Escribe una cadena de caracteres en la posición actual del cursor
    void escribeCaracter(char caracter);
    // Escribe un carácter en la posición actual del cursor
    void muestraCursor(uint8_t muestralo);
    // Hace que el cursor sea visible en función del buleano que se pasa por parámetro
};
#endif