/*
 *nRF24L01  
 * Receptor 3 CH para coche RC controlado con arduino NANO
 * Direccion controlada con servo.
 * Avance y retroceso progresivo con PWM
 * Pines para nRF24L01: 9, 10, 11, 12, 13.
 * Pin PWM : 3
 * Pin Avance: 4
 * Pin retroceso: 5
 * Autor: Carlos Muñoz
 * Información: https://www.infotronikblog.com/2017/12/arduino-coche-rc-maisto-drift-xboxrc24.html
 * Web: https://www.infotronikblog.com
*/
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "Servo.h"

Servo servo0;
int pos = 90; // Variable para la posicion del servo.
int acc;      // Variable para aceleracion.
int dcc;      // Variable para freno y marcha atras.
RF24 radio(9, 10);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

typedef struct {
  int dec  = 0; // T1
  int ace  = 0; // T2
  int LXdata = 90; // Eje LX
  int LYdata = 0; // Eje LY
  int RXdata = 0; // Eje RX
  int RYdata = 0; // Eje RY
  int ButA = 0; // Boton A
  int ButB = 0; // Boton B
  int ButX = 0; // Boton X
  int ButY = 0; // Boton Y
  int Butb = 0; // Boton Back
  int ButS = 0; // Boton Start
}
dataReceive;
dataReceive dataR;

// Configuración:
void setup() {
  // Serial.begin(9600);  //Comunicacion serial para debug en el monitor serial
  servo0.attach(2);   // Pin 2 para el servo
  servo0.write(pos);  // Situamos el servo en la posicion inicial
  pinMode(4, OUTPUT); // Pin 4 configurado como salida.
  pinMode(5, OUTPUT); // Pin 5 configurado como salida.

  // Inicia la comunicación con nRF24L01
  radio.begin();
  // radio.setRetries(15, 15);
  radio.startListening();
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);
}

// Loop principal del programa:
void loop() {
  receiver();
  conversion();
  botones();
  if (acc >= 10) {  //AVANCE:
      digitalWrite (4, HIGH);
      digitalWrite (5, LOW);
      analogWrite(3, acc);
      // Serial.println("avance");
    }
  if (dcc >= 20) { //RETROCESO:
      digitalWrite (4, LOW);
      digitalWrite (5, HIGH);
      analogWrite(3, dcc);
      // Serial.println("retroceso");
    }
  if ((acc == 0) && (dcc == 0)) {
    digitalWrite (4, LOW);
    digitalWrite (5, LOW);
    analogWrite(3, 0);
  }

  servo0.write(pos);

}

// Mapeo de datos recibidos para que siempre
// estén en el rango en el que vamos a operar 0 - 255
void conversion()
{
  acc = map(dataR.ace, 0, 100, 0, 254);
  acc = constrain (acc, 0 , 240);

  dcc = map(dataR.dec, 0, 100, 0, 254);
  dcc = constrain (dcc, 0 , 240);
  dcc = dataR.dec;

  pos = map(dataR.LXdata, 100, 0, 40, 175);
  pos = constrain (pos, 40 , 170);
}
// Enciende o apaga las luces con el botón A
void botones() {
  if (dataR.ButA == 0) {
    digitalWrite(luces, HIGH);
  } else {
    digitalWrite (luces, LOW);
  }
}
