#include <Arduino.h>
#include <TimerFive.h>
#include "Altivar.h"
#include "Consola.h"
#include "ControlPI.h"

Consola consola(32, 33, 34, 35, 36, 37, 38, 22, 23, 24, 25, 26, 27, 28, 29,
                "789s456b123d.0ei");
// Construye un objeto Consola indicándole qué señales de la placa Arduino se utilizan para
// manejar la pantalla y el teclado y los caracteres del teclado
const char textoVelocidad[] PROGMEM = "Velocidad:";
const char textoTemperatura[] PROGMEM = "Temperatura:";
const char textoConsigna[] PROGMEM = "Consigna:";
// Mensajes a visualizar en la pantalla LCD y guardados en memoria Flash
Modbus canal(Serial1, 19200, SERIAL_8N1);
// Objeto para implantar comunicación Modbus RTU sobre el canal serie Serial1 de la placa
// Arduino Mega para comunicación a 19200 baud, 8 bits/byte, sin paridad y 1 bit stop
Altivar28 variador(1, canal);
// Objeto para manejar un variador de velocidad Schneider Altivar 28 que actúa como el esclavo
// con dirección 1 en el canal Modbus RTU
// Altivar31 variador(1, canal);
// Objeto para manejar un variador de velocidad Schneider Altivar 31 que actúa como el esclavo
// con dirección 1 en el canal Modbus RTU
ControlPI controlador(100, 10, -0.75, -0.25, 1);
// Objeto para manejar el controlador, regula el variador entre 100 y 10 Hz, con Kp=-0.75 y Ki=-0.25 con un muestreo de 1 segundo

int Interrupcion = 0;    // Booleano para manejar la interrupción
float Velocidad;         // Variable para manejar la velocidad del motor
float Consigna = 5000.0; // Variable para manejar la consigna
char tecla;              // Variable que almacena la tecla que han pulsado
String cadena = "";      // Cadena vacía para concatenar valores


/***************************************************************************************/
/************************************ Interrupción *************************************/
/***************************************************************************************/

void interrupt() // Interrupción 
{
  Interrupcion = 1; // Booleano para manejar la interrupción
}

/***************************************************************************************/
/*************************************** Setup *****************************************/
/***************************************************************************************/

void setup()
{
  Timer5.initialize(1000000); // Indica la interrupción cada segundo
  Timer5.attachInterrupt(interrupt); // Función que genera la interrupción

  consola.configura(); // Configura la consola
  canal.configura(); // Configura el canal
  variador.configura(); // Configura el variador

  consola.visualizaCadenaFlash(1, 1, textoVelocidad); // Visualiza la velocidad en la consola
  consola.visualizaCadenaFlash(2, 1, textoTemperatura); // Visualiza la temperatura en la consola
  consola.visualizaCadenaFlash(3, 1, textoConsigna); // Visualiza la consigna en la consola
}

/***************************************************************************************/
/*************************************** Loop ******************************************/
/***************************************************************************************/

void loop()
{

  tecla = consola.getTecla(); // Método de la clase consola que almacena la tecla que han pulsado en la consola

  switch (tecla) // En función de la tecla pulsada se hacen las siguientes acciones
  {

  case 0: // Teclas que no se usan
  case 'd':
  case '.':
    // No hacen nada
    break;

  case 's': // Tecla de subida
    // Incrementa en 100 la consigna
    Consigna += 100;
    consola.visualizaEntero(3, 13, Consigna, 4); // Visualiza la consigna en la consola
    break;

  case 'b': // Tecla de bajada
    // Decrementa en 100 la consigna
    Consigna -= 100;
    consola.visualizaEntero(3, 13, Consigna, 4); // Visualiza la consigna en la consola
    break;

  case 'e': // Tecla de Enter
    // Guarda la consigna
    if (cadena != "") // Si la cadena no está vacía
    {
      Consigna = cadena.toInt(); // Convierte la cadena a entero y la guarda en la consigna
      if (Consigna > 20000)
      { // Limitación de la consigna a 20000 μA que es valor máximo de señal de temperatura (20 mA)
        Consigna = 20000;
      }
      consola.visualizaEntero(3, 13, Consigna, 4); // Visualiza la consigna en la consola
      cadena = ""; // Vacía la cadena
    }
    break;

  default:
    // Cualquier otra tecla, concatena el elemento anterior
    cadena = cadena + tecla;
    consola.visualizaEntero(3, 13, cadena.toInt(), 4); // Visualiza la consigna en la consola
    break;
  }

  if (Interrupcion == 1) // Si entra en la interrupción ejecuta la acción de control
  {
    uint16_t TempActual = variador.getEntradaAnalogicaCorriente(); // Lee la temperatura en forma de corriente entre 0 y 20000 μA
    consola.visualizaEntero(2, 13, TempActual, 5); // Visualiza por consola esa temperatura (en realidad lee corriente entre 0 y 20000 μA)

    Velocidad = controlador.velocidadRegulada(Consigna, TempActual); // Ejecuta el PI y devuelve la velocidad ya regulada
    variador.setVelocidad(Velocidad); // Le pasa la velocidad regulada al variador
    consola.visualizaReal(1, 13, Velocidad, 4, 1); // Visualiza la velocidad por consola

    Interrupcion = 0; // Pone el booleano a False
  }
}
