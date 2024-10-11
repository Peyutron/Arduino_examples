/* XboxRC24 v0.6
 * Mando transmisor multifuncion para RC en banda 2,4GHz
 * Arduino NANO
 * LCD 16x2 i2C
 * Módulo nRF2401 con antena
 * Pines analógicos: A0, A1, A2, A3, A6, A7
 * Pines digitales: 3, 4, 5, 6, 7, 8
 * Pines nrf2401: 9, 10, 11, 12, 13
 * Pines LCD i2C: A4, A5
 * Direccion i2C; 0x3F
   **V.05**
     Añadido el modo coche
     Añadido Debug.ino para la visualizacion en el monitor serial
     --Datos enviados/recibidos.
     --Estado de la memoria: valores minimos y maximos.
     Mejora en la funcion Calibracion
 * pitch Cabeceo
 * rudder Timon
 * Autor: Carlos Muñoz
 * Información: https://www.infotronikblog.com/2017/12/arduino-coche-rc-maisto-drift-xboxrc24.html
 * Web: https://www.infotronikblog.com
 */
   #include <SPI.h>
   #include <nRF24L01.h>
   #include <RF24.h>
   #include <EEPROM.h>  // Libreria de la memoria EEPROM
   #include <Wire.h>    // Libreria de comunicacion i2C
   #include <LiquidCrystal_I2C.h>  // Libreria para el LCD i2C
   #include <printf.h>
   #include "Config.h"

   #define AVION 0
   #define CAMION 1
   #define COCHE 2
   #define TANQUE 3
   #define CONFIG 4

// Ponemos la direccion del LCD i2C en este caso 0x0F con 16 caracteres y 2 lineas.
LiquidCrystal_I2C lcd(0x3F, 16, 2);

// Definimos los pines de transmision/recepcion del nRF2401
// "crear" el objeto radio en los pines 9 y 10 para Arduino NANO/UNO
RF24 radio(9, 10);
// Radio RF24 (9,53); // para Mega

bool recepcion = true;
bool errorRecepcion = false;
//unsigned long _startTime = 0;
const uint64_t pipes[] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL, 0xF0F0F0F0D3LL, 0xF0F0F0F0D4LL };
const int maxNumPipes = (sizeof(pipes) / (sizeof(uint64_t)));

// Estructura de datos donde estan las variables que enviaremos
typedef struct {
  int T0data  = 0; //T1
  int T1data  = 0; //T2
  int LXdata = 0; // Eje LX
  int LYdata = 0; // Eje LY
  int RXdata = 0; // Eje RX
  int RYdata = 0; // Eje RY
  int ButA = 0; //Boton A
  int ButB = 0; //Boton B
  int ButX = 0; //Boton X
  int ButY = 0; //Boton Y
  int Butb = 0; //Boton Back
  int ButS = 0; //Boton Start
}
dataSend;
dataSend dataS;
//Escructura de datos con las variables que vamos a recibir
typedef struct {
  int dataR0  = 0;
  int dataR1  = 0;
  int dataR2  = 0;
  float dataR3  = 0;
  int dataR4  = 0;
}
dataReceive;
dataReceive dataR;

int estadoButA = 0;
int estadoButB = 0;
int estadoButX = 0;
int estadoButY = 0;

float bat = 0.00;

// Definimos los pines analogicos:
#define pinJoyT0 A0 // Gatillo izquierdo en el pin analogico A0
#define pinJoyT1 A1 // Gatillo derecho en el pin analogico A1
#define pinJoyLX A2 // Joystick izquierdo eje X en el pin analogico A2
#define pinJoyLY A3 // Joystick izquierdo eje Y en el pin analogico A3
#define pinJoyRX A7 // Joystick derecho eje X en el pin analogico A7
#define pinJoyRY A6 // Joystick derecho eje Y en el pin analogico A6
const byte numeroDePin = 7;
char analogPin[numeroDePin][9] = {"pinJoyT0", "pinJoyT1", "pinJoyLX", "pinJoyLY", "pinJoyRX", "pinJoyRY"};

// Definimos los ejes
#define T0  0 // Gatillo izquierdo
#define T1  1 // Gatillo derecho
#define LX  2 // Eje izquierdo (derecha/izquierda)
#define LY  3 // Eje izquierdo (arriba/abajo)
#define RX  5 // Eje derecho (derecha/izquierda)
#define RY  4 // Eje derecho (arriba/abajo)
// Definimos los pines digitales:
#define butA  8   // Boton A en el pin 8
#define butB  7   // Boton A en el pin 7
#define butX  6   // Boton X en el pin 6
#define butY  5   // Boton Y en el pin 5
#define butStart  3 // Boton Start en el pin 3
#define butSelect 4 // Boton Select en el pin 4
#define MAXANIMACION 1023


int varCal;
//int varJoy[8];     // Array de variables donde se almacenara la lectura de los potenciometros
int varJoyMin[10];   // Array de variables donde se almacenara la lectura minima de los potenciometros
int varJoyTop[10];   // Array de variables donde se almacenara la lectura maxima de los potenciometros
int indexEEPROM = 0;
bool inversionJoy[10];


//const uint8_t buffer_size = sizeof(varJoy);

// variables para los menu:
int xmenu = 0 ;       // Variables para
int btnmenu = 0 ;     // Los menus donde se
bool flagmenu = true; // almacenan la posicion
bool menuPrincipal = true;
const int numeroDeMenu = 5;
const char tituloMenu[numeroDeMenu][16] = {" Avion", " Camion", " Coche", " Tanque"};
const int numeroDeMenuCal = 10;  //Numero de menus de calibracion
const char tituloMenuCal[numeroDeMenuCal][16] = {" Radio info", " Gatillo izq. ", " Gatillo der. ", " Eje LX ", " Eje LY ", " Eje RX ", " Eje RY ", " Canal", " Borrar todo "};

// Array de variables de control de los botones.
int varBut[6];
int varButA  = 0; int varButB  = 0; int varButX  = 0; int varButY  = 0; int varButStart  = 0; int varButSelect = 0;
int modo_uso = 0;

// SETUP
void setup() {
  // Iniciamos la conexion Serial
  Serial.begin(115200);

  // Iniciamos el LCD
  lcd.begin();
  printf_begin();

  // Configuramos los pulsadores como entradas digirales
  for (int x = 3; x < 9; x++) {
    pinMode(x, INPUT_PULLUP);
    int z = digitalRead(x);
  }

  // Inicia el módulo nRF2401
  radio.begin();
  // radio.enableAckPayload ();    // permite enviar datos en respuesta a una señal entrante
  // radio.setPayloadSize ( 32 );     // tamaño del paquete, en bytes
  radio.setChannel(CHANNELRF);
  radio.setPALevel(LEVELRF);    // transmitir nivel de potencia. Opcional: RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate(SPEEDRF);    // tasa de cambio. Opcional RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  // radio.setAutoAck (1);             // recibir el modo de confirmación, 1 en 0 desactivado
  // radio.setRetries(2, 15);          // Maximos reintentos
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);

  // presentacion(); // vamos a la funcion presentacion en la pestaña LCD.
  readEeprom(); //  Vamos a la funcion readEeprom en la pestaña "memoria"
  // debugMemoria();
  //radio.printDetails();
  lcd.clear();
}

// Loop principal del programa
void loop() {
  menu();  
}

void gomenu(){
  xmenu = 0;
  flagmenu = true;
  menu();
}
