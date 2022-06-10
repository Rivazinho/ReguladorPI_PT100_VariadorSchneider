#ifndef CONSOLA_H_
#define CONSOLA_H_
#include "PantallaMidas.h"
#include "Teclado4x4.h"
class Consola
{
    // Un objeto de esta clase representa a una consola compuesta por un teclado y una
    // pantalla
private:
    PantallaMidas pantalla;
    Teclado4x4 teclado;
    // Cada objeto Consola contiene objetos para manejar el teclado y la pantalla
public:
    Consola(uint8_t DB4, uint8_t DB5, uint8_t DB6, uint8_t DB7, uint8_t E1, uint8_t E2,
            uint8_t RS, uint8_t F1, uint8_t F2, uint8_t F3, uint8_t F4, uint8_t C1, uint8_t C2,
            uint8_t C3, uint8_t C4, char const *caracteres);
    // Constructor al que le proporcionamos los datos necesarios para inicializar la pantalla
    // y el teclado
    void configura(); // Utilizado en el setup() para configurar estos dispositivos
    void visualizaEntero(uint8_t f, uint8_t c, int e, uint8_t nc);
    // Visualiza el número entero e en la fila f y la columna c de la pantalla,
    // ocupando nc caracteres
    void visualizaReal(uint8_t f, uint8_t c, float r, uint8_t nc, uint8_t nd);
    // Visualiza el número real r en la fila f y la columna c de la pantalla, utilizando
    // un número de caracteres nc de los cuales hay nd decimales
    void visualizaCadena(uint8_t f, uint8_t c, const char *cadena);
    // Visualiza la cadena de caracteres en la fila f y columna c de la pantalla
    void visualizaCadenaFlash(uint8_t f, uint8_t c, const char *cadena);
    // Visualiza una cadena de caracteres almacenada en memoria Flash. Se visualiza
    // en la fila f y columna c de la pantalla.
    void visualizaString(uint8_t f, uint8_t c, String &s);
    // Visualiza el texto del objeto s en la fila f y la columna c de la pantalla
    char introduceCaracter();
    // Espera a que el usuario pulse el teclado. Devuelve el carácter pulsado.
    void introduceString(uint8_t f, uint8_t c, String &v, uint8_t nc, String &s);
    // Guarda en s una cadena de caracteres introducida por teclado hasta que se pulse la
    // tecla ENT. A medida que se introduce la cadena, se va visualizando a partir de la fila
    // f y columna c de la pantalla. Si se pulsa la flecha izquierda en el teclado, se borra
    // el último carácter introducido. En v se indican los caracteres que pueden formar parte
    // de la cadena. Introduce como máximo nc caracteres.
    int introduceEntero(uint8_t f, uint8_t c, uint8_t nc);
    // Devuelve un número entero introducido por teclado que tiene como máximo nc dígitos
    // y que se visualiza en la fila f y columna c de la pantalla
    float introduceReal(uint8_t f, uint8_t c, uint8_t nc);
    // Devuelve un número real introducido por teclado que tiene como máximo nc dígitos
    // y que se visualiza en la fila f y columna c de la pantalla
    void borraPantalla();
    // Para borrar la pantalla
    char getTecla();
    // Para guardar la tecla pulsada
};
#endif