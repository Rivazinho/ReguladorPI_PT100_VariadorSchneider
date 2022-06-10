#include "Consola.h"
#include <string.h> // Para utilizar la clase String
Consola::Consola(uint8_t DB4, uint8_t DB5, uint8_t DB6, uint8_t DB7,
                 uint8_t E1, uint8_t E2, uint8_t RS, uint8_t F1, uint8_t F2, uint8_t F3, uint8_t F4,
                 uint8_t C1, uint8_t C2, uint8_t C3, uint8_t C4, char const *caracteres)
    : // Constructor al que le proporcionamos los datos necesarios para inicializar la
      // pantalla y el teclado
      pantalla(DB4, DB5, DB6, DB7, E1, E2, RS),
      // Construye el objeto pantalla llamando a su constructor
      teclado(F1, F2, F3, F4, C1, C2, C3, C4, caracteres) // Construye el objeto teclado
{
    // No añade ninguna instrucción
}
void Consola::configura()
{ // Utilizado en el setup() para configurar estos dispositivos
    pantalla.configura();
    teclado.configura();
    // Llama a los mismos métodos de las clases de la pantalla y el teclado
}
void Consola::visualizaEntero(uint8_t f, uint8_t c, int e, uint8_t nc)
{
    // Visualiza el número entero e en la fila f y la columna c de la pantalla,
    // ocupando nc caracteres
    String cadena = String(e);
    pantalla.posiciona(f, c);               // Sitúa el cursor en la pantalla
    pantalla.escribeCadena(cadena.c_str()); // Convierte el entero a String y lo muestra
    for (unsigned int i = cadena.length(); i < nc; i++)
        pantalla.escribeCaracter(' '); // Borra caracteres adicionales
}
void Consola::visualizaReal(uint8_t fila, uint8_t columna, float real, uint8_t nCaracteres,
                            uint8_t nDecimales)
{
    // Visualiza el número real en una fila y columna de la pantalla, utilizando
    // un número de caracteres de los cuales hay un cierto número de decimales
    pantalla.posiciona(fila, columna); // Sitúa el cursor en la pantalla
    // Serial.println(String(real, nDecimales));
    pantalla.escribeCadena(String(real, nDecimales).substring(0, nCaracteres).c_str());
    // Convierte el número real a String y lo muestra en pantalla
}
char Consola::introduceCaracter()
{
    // Espera a que el usuario pulse el teclado y devuelve el carácter pulsado
    char caracter;
    do                                  // Repite ...
        caracter = teclado.comprueba(); // Comprueba si se está pulsando el teclado
    while (caracter == 0);              // hasta que se haya pulsado algo
    return caracter;                    // Devuelve el carácter pulsado
}
void Consola::visualizaString(uint8_t fila, uint8_t columna, String &cadena)
{
    // Visualiza el texto de un objeto String en una fila y columna de la pantalla
    pantalla.posiciona(fila, columna); // Sitúa el cursor en la pantalla
    pantalla.escribeCadena(cadena.c_str());
}
void Consola::introduceString(uint8_t fila, uint8_t columna, String &validos,
                              uint8_t nCaracteres, String &resultado)
{
    // Introduce el texto de un objeto String en el teclado compuesta de caracteres
    // presentes en la cadena 'validos', mostrándola en la pantalla a partir de la
    // fila y columna indicadas.
    // Se introducen como máximo 'nCaracteres' caracteres. Finaliza cuando se pulse la
    // tecla ENT
    uint8_t i;
    char caracter;
    resultado = "";                    // Inicializa la cadena
    pantalla.posiciona(fila, columna); // Sitúa el cursor en la posición inicial
    for (i = 0; i < nCaracteres; i++)
        pantalla.escribeCaracter(' ');
    // Limpia la zona de la pantalla donde se va a visualizar el texto introducido
    pantalla.posiciona(fila, columna);
    pantalla.muestraCursor(true);
    // Vuelve a posicionar el cursor y lo muestra
    i = 0; // Contador de caracteres introducidos
    do
    {                                   // Repite ...
        caracter = introduceCaracter(); // Otro carácter
        if (validos.indexOf(caracter) >= 0 && i < nCaracteres)
        {
            // Si es válido y no se han introducido todos ...
            resultado += caracter;              // Lo añade a la cadena
            i++;                                // Incrementa contador de caracteres
            pantalla.escribeCaracter(caracter); // Lo visualiza en la pantalla
        }
        if (caracter == 'i')
        {                                          // Si es la tecla de borrado ...
            i--;                                   // Elimina un carácter
            resultado.remove(i);                   // y lo quita del resultado
            pantalla.posiciona(fila, columna + i); // Cursor una posición a la izquierda
            pantalla.escribeCaracter(' ');         // Escribe un espacio
            pantalla.posiciona(fila, columna + i); // De nuevo a la izquierda
        }
    } while (caracter != 'e');     // Repite mientras no se pulse ENT
    pantalla.muestraCursor(false); // Oculta el cursor en la pantalla
}
void Consola::visualizaCadena(uint8_t fila, uint8_t columna, const char *cadena)
{
    // Visualiza la cadena en una fila y columna de la pantalla
    pantalla.posiciona(fila, columna);
    pantalla.escribeCadena(cadena);
}
void Consola::visualizaCadenaFlash(uint8_t fila, uint8_t columna, const char *cadena)
{
    // Visualiza la cadena en una fila y columna de la pantalla
    char c;
    pantalla.posiciona(fila, columna);
    do
    {
        c = pgm_read_byte(cadena);
        if (c)
            pantalla.escribeCaracter(c);
        cadena++;
    } while (c);
}
int Consola::introduceEntero(uint8_t fila, uint8_t columna, uint8_t nCaracteres)
{
    // Devuelve un número entero introducido por teclado que se muestra en la fila
    // y columna indicadas por parámetro. 'nCaracteres' indica el número máximo de
    // dígitos.
    String cadena;
    String validos = "0123456789";
    introduceString(fila, columna, validos, nCaracteres, cadena);
    // Introduce el valor como una cadena de caracteres
    return cadena.toInt(); // Convierte la cadena a entero y lo devuelve
}
float Consola::introduceReal(uint8_t fila, uint8_t columna, uint8_t nCaracteres)
{
    // Devuelve un número real introducido por teclado que se muestra en la fila
    // y columna indicadas por parámetro. 'nCaracteres' indica el número máximo de
    // dígitos.
    String cadena;
    String validos = "0123456789.";
    do
    {
        introduceString(fila, columna, validos, nCaracteres, cadena);
        // Introduce el valor como una cadena de caracteres
    } while (cadena.indexOf('.') != cadena.lastIndexOf('.'));
    // Repite si hay más de un punto
    return cadena.toFloat(); // Convierte la cadena a real y la devuelve
}
void Consola::borraPantalla()
{
    // Borra la pantalla
    pantalla.borra();
}

char Consola::getTecla()
{
    // Obtiene lo que se ha pulsado por teclado
    return teclado.comprueba();
}