/*
 * nRF24L01 + DHT11
 * Receptor 3 CH para vehiculo oruga RC controlado con arduino NANO
 * 2 canales para Avance y retroceso progresivo con PWM
 * La radio es capaz de recibir telemetría como puede ser un sensor DHT11 o el estado 
 * de la batería
 * Pines para nRF24L01: 9, 10, 11, 12, 13.
 * Pin PWM : 3
 * Pines canal A: 4 y 5
 * Pines canal B: 6 y 7 
 * Autor: Carlos Muñoz
 * Información: https://www.infotronikblog.com/2017/12/arduino-coche-rc-maisto-drift-xboxrc24.html
 * Web: https://www.infotronikblog.com
*/
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <DHT.h>

RF24 radio(9, 10);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

// Estructura de los datos de entrada
typedef struct {
  int dec  = 0; // T1
  int ace  = 0; // T2
  int LXdata = 0; // Eje LX
  int LYdata = 0; // Eje LY
  int RXdata = 0; // Eje RX
  int RYdata = 0; // Eje RY
  int ButA = 0; // Boton A
  int ButB = 0; // Boton B
  int ButX = 0; // Boton X
  int ButY = 0; // Boton Y
  int Butb = 0; // Boton Back
  int ButS = 0; //  Boton Start
}

dataReceive;
dataReceive dataR;

// Estructura de los datos de salida
typedef struct {
  int dataS0  = 0; // dato 0
  int dataS1  = 0; // dato 1
  int dataS2  = 0; // dado 2
  int dataS3  = 0; // dato 3
  int dataS4  = 0; // dato 5
}

dataSend;
dataSend dataS;

// Pin digital donde se conectamos el sensor.
#define DHTPIN 8

// Definimos el tipo de sensor (DHT11, DHT22).
#define DHTTYPE DHT11

// Iniciamos el sensor DHT11.
DHT dht(DHTPIN, DHTTYPE);

int acc;         // Variable para aceleracion.
int motor11 = 4; //
int motor12 = 5; // Salidas para driver L293N
int motor21 = 6; //
int motor22 = 7; //
int enable1 = 3; // Incluidos el enable

int luces = 14;
int estado;
int flag = 0;
int estadoStop = 0;

/////////////SETUP/////////////////////////////

void setup() {
  Serial.begin(115200);  // Comunicacion serial para el monitor serial

  // Configuramos los pines de salida:
  pinMode(motor11, OUTPUT);
  pinMode(motor12, OUTPUT);
  pinMode(enable1, OUTPUT);
  pinMode(motor21, OUTPUT);
  pinMode(motor22, OUTPUT);
  pinMode(luces, OUTPUT);
  digitalWrite (luces, HIGH);
  delay (3000);
  digitalWrite (luces, LOW);
  
  // Inicia el sensor DHT11
  dht.begin();
  
  // Inicia la comunicación con nRF24L01
  radio.begin();
  radio.setRetries(15, 15);
  radio.startListening();
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);
}

// Inicia el loop principal
void loop() {
  receiver();
  botones();
  conversion();
  
  flag = 0;

  if (estado == 0 || estadoStop == 1) { // PARADO
    digitalWrite(motor11, LOW);
    digitalWrite(motor12, LOW);
    digitalWrite(motor21, LOW);
    digitalWrite(motor22, LOW);
    if (flag == 0) {
      Serial.println("STOP!");
      flag = 1;
    }
    estadoStop = 0;
  } else if (estado == 1) {  // AVANCE:
    analogWrite(enable1, acc);
    digitalWrite(motor11, HIGH);
    digitalWrite(motor12, LOW);
    digitalWrite(motor21, HIGH);
    digitalWrite(motor22, LOW);
    if (flag == 0) {
      Serial.println("Avance!!!");
      flag = 1;
    }
  } else if (estado == 2) {  // RETROCESO:
    analogWrite(enable1, acc);
    digitalWrite(motor11, LOW);
    digitalWrite(motor12, HIGH);
    digitalWrite(motor21, LOW);
    digitalWrite(motor22, HIGH);
    if (flag == 0) {
      Serial.println("REVERSA!!!");
      flag = 1;
    }
  } else if (estado == 3) {  // IZQUIERDA
    analogWrite(enable1, acc);
    digitalWrite(motor11, LOW);
    digitalWrite(motor12, HIGH);
    digitalWrite(motor21, HIGH);
    digitalWrite(motor22, LOW);
    if (flag == 0) {
      Serial.println("GIRA IZQUIERDA");
      flag = 1;
    }
  } else if (estado == 4) { // DERECHA
    analogWrite(enable1, acc);
    digitalWrite(motor11, HIGH);
    digitalWrite(motor12, LOW);
    digitalWrite(motor21, LOW);
    digitalWrite(motor22, HIGH);
    if (flag == 0) {
      Serial.println("GIRA DERECHA");
      flag = 1;
    }
  }
  
  // Leemos el estado  del sensor DHT11 cada 2 s.
  static unsigned long lastTrigger; 
  if (millis() - lastTrigger >= 2000)
  {
    lastTrigger = millis();
    dht11Read();
  }
}

// Mapeo de datos recibidos para que siempre
// estén en el rango en el que vamos a operar 0 - 255
void conversion()
{
  acc = map(dataR.ace, 0, 100, 0, 254);
  acc = constrain (acc, 0 , 240);

  if (dataR.LYdata == dataR.LXdata) estado = 0;
  else if ((dataR.LYdata > 50) && (dataR.LXdata <= 50)) estado = 1; // AVANCE
  else if ((dataR.LYdata < 50) && (dataR.LXdata >= 50)) estado = 2; // REVERSA
  else if ((dataR.LXdata > 50) && (dataR.LYdata <= 50)) estado = 3;
  else if ((dataR.LXdata < 50) && (dataR.LYdata >= 50)) estado = 4;
}

// Enciende o apaga las luces con el botón A
void botones(){
  if (dataR.ButA == 0) digitalWrite(luces, HIGH);
  else digitalWrite (luces, LOW);
}

// Obtiene los datos del sensor DHT11 para enviarlos
// por el módulo nRF24L01 a modo de telemetría.
void dht11Read() {
  
  // Lee la humedad relativa.
  float h = dht.readHumidity();

  // Lee la temperatura en ºC (por defecto).
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;   // Si tiene error vuelve a la función principal
  }
  
  // Calcula el índice de calor en ºC
  float hic = dht.computeHeatIndex(t, h, false);  
  
  // Cargamos los datos en las variables para mandar los datos con el módulo nRF24L01
  dataS.dataS1 = t;
  dataS.dataS2 = h;
  dataS.dataS3 = hic;
  /*
  // Debug DHT11
  Serial.println();
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" ºC");
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.println(" %");
  Serial.print("Indice de calor: ");
  Serial.print(hic);
  Serial.println(" ºC");
  */
}
