/*
   Receptor 7 CH para Camión RC WPL-B24 Gaz-66 controlado con arduino NANO
   Direccion controlada con servo.
   Pines para nRF24L01: 9, 10, 11, 12, 13.
   PWM ESC :          3
   Servo direccion:   4
   Luces traseras:    5
   Luces delanteras:  6
   Servo marchas :    7
   Luces Cabina :     8
   Autor: Carlos Muñoz
   https://www.infotronikblog.com
*/
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "Servo.h"

// Pines de Arduino
#define ACELERADOR  3 // Servo acelerador 
#define SERVO1      4  // Servo dirección
#define LUCESDEL    6  // Pin luces delanteras (PWM)
#define LUCESTRAS   5 // Pin luces traseras (PWM)
#define SERVO2      7  // Servo caja de marchas
#define LUCESINT    8 //Pin Luces Interiores
#define BATERIA     A7  // Pin analogico lectura batería
// Valores para el cambio de marcha
#define MLARGA   80
#define MCORTA   24
// Valores Servo direccion
#define DIFERENCIAL  9
#define DIRCENTER 90
#define TOPDIRDER 122  // Tope direccion ( max 130)
#define MINDIRIZQ 80   // Tope direccion Minima ( min 70)
// Valores velocidad
#define TOPSPEED 150  // Tope velocidad Maxima ( max 160)
#define MINSPEED 60   // Tope velocidad Minima ( min 40)
// Valores para las luces
#define LMAX 250
#define LMID 127
#define LMIDTRAS 128
#define LMIN 45

// definimos los objetos servo
Servo Sdir;
Servo Marchas;
Servo servo_acelerador;
float nivel_bateria = 0;
boolean estadoLuces = false;
boolean freno = false;
int Lfun = 1;

// Declaramos las variables con sus valores.
int pos = DIRCENTER; // Variable para la posicion del servo.
int acc = 90;      // Variable para aceleracion.
int accT0 = 90;
int accT1 = 90;
int armado = 0; // Variable para detectar si hemos armado el variador del motor principal. 0 = No armado. 1 = Armado
int sentido = 0; // 0 = parado 1 = Avanzar. 2 = Retroceder
int dcc;      // Variable para freno y marcha atras.
int Marcha = 1;
int topspeed = 0;
int bitdir = 0;
bool conexion = false;

RF24 radio(9, 10);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

typedef struct {
  int dec  = 0; //T1
  int ace  = 0; //T2
  int LXdata = 90; //Eje LX
  int LYdata = 90; // Eje LY
  int RXdata = 90; // Eje RX
  int RYdata = 90; // Eje RY
  int ButA = 0; //Boton A
  int ButB = 0; //Boton B
  int ButX = 0; //Boton X
  int ButY = 0; //Boton Y
  //  int Butb = 0; //Boton Back
  //  int ButS = 0; //Boton Start
}
dataReceive;
dataReceive dataR;
typedef struct {
  int dataS0  = 0; //dato 0
  int dataS1  = 0; //dato 1
  int dataS2  = 0; // dado 2
  float dataS3  = 0; // dato 3
}
dataSend;
dataSend dataS;
unsigned long lastRecvTime = 0;


/////////////SETUP/////////////////////////////
void setup() {
  Serial.begin(115200);  //Comunicacion serial para debug en el monitor serial
  pinMode(LUCESDEL, OUTPUT);
  pinMode(LUCESTRAS, OUTPUT);
  pinMode(LUCESINT, OUTPUT);
  Sdir.attach(SERVO1);   // Pin 4 para el servo
  Marchas.attach(SERVO2);
  servo_acelerador.attach(ACELERADOR);
  Sdir.write(DIRCENTER);  // Situamos el servo en la posicion inicial
  Marchas.write(MLARGA);
  digitalWrite(LUCESDEL, HIGH);
  digitalWrite(LUCESTRAS, HIGH);
  digitalWrite(LUCESINT, HIGH);
  delay (1000);
  digitalWrite(LUCESDEL, LOW);
  digitalWrite(LUCESTRAS, LOW);
  digitalWrite(LUCESINT, LOW);
  setupRadio(); // Va la pestaña NRF2401 y configura la radio

}
/////////////LOOP/////////////////////////////
void loop() {
  receiver();
  unsigned long now = millis();
  //Comprobamos si se ha perdido la señal en caso de se pierda encendemos el led de aviso
  //y reseteamos los valoresl and if we did we reset the values unsigned long now = millis();
  if ( now - lastRecvTime > 1000 ) {
    Serial.println("Senal perdida");  // si se pierde la señal vamos a la subrutina 
    resetData();                      // que resetea los parametros
  } else {
    //receiverDebug();
    botones();
    conversion();

    if (armado == 1 && (pos < 89) || (pos > 91)) { //Enviamos datos sólo si el proceso de armado se ha realizado y descartamos un margen de error de 10 unidades.
      
      if (pos > 0 && pos < 179) { //Evitamos valores negativos.
         //if (bitdir !=4) Sdir.attach(SERVO1);
         Sdir.write(pos);//Giramos el servo.
      }

    } else {

      //Sdir.write(DIRCENTER - DIFERENCIAL); //Enviamos a posición central
      Sdir.write(pos);
    }
    if (armado == 1 && freno == false && (acc > 90 && acc < TOPSPEED + 1)) { //Confirmamos armado y sentido avanzar

      if (sentido == 1 || sentido == 0) { //Evitamos avanzar si estábamos retrocediendo para evitar daños en la mecánica
        
        servo_acelerador.write(acc); //Avanzamos
        sentido = 1;

      } else { //Si el usuario intenta hacer un cambio de sentido primero frenamos y esperamos.
        
        acc = 90;
        servo_acelerador.write(acc);
        sentido = 0;
      }

    } else if (armado == 1 && freno == false && (acc < 89 && acc > MINSPEED)) { //Confirmamos armado y sentido retroceder

      if (sentido == 2 || sentido == 0) { //Evitamos retroceder si estábamos avanzando para evitar daños en la mecánica
      
        servo_acelerador.write(acc);
        sentido = 2;

      } else { //Si el usuario intenta hacer un cambio de sentido primero frenamos y esperamos.

        acc = 90;
        servo_acelerador.write(acc);
        sentido = 0;
        delay(250);
      }

    } else { //No hay ordenes.
      sentido = 0;
      acc = 90;
      servo_acelerador.write(acc);
    }
    // if (freno) servo_acelerador.write(VFRENO);
  
    if (estadoLuces) Luces(Lfun);


  }
}
/////////////Conversion de datos recibidos////////////////////////////////////////////////
void conversion()
{
  accT1 = map(dataR.ace, 0, 1023, 90, TOPSPEED);
  accT0 = map(dataR.dec, 0, 1023, 90, MINSPEED);
  if (accT1 > 91 ) acc = accT1;
  else acc = accT0;
  pos = map(dataR.LXdata, 0, 1023, MINDIRIZQ, TOPDIRDER);    
  pos = pos - DIFERENCIAL;

}
void resetData() {
  dataR.dec  = 0; //T1
  dataR.ace  = 0; //T2
  dataR.LXdata = 90; //Eje LX
  dataR.LYdata = 90; // Eje LY
  dataR.RXdata = 90; // Eje RX
  dataR.RYdata = 90; // Eje RY
//  dataR.ButA = 0; //Boton A
  dataR.ButB = 0; //Boton B
  dataR.ButX = 0; //Boton X
  dataR.ButY = 0; //Boton Y

  Luces(4);

}
void Sensores() {
  //Nivel de bateria (Vbat * 5) / 1023 = 28.816
  nivel_bateria = analogRead(BATERIA); //////Voltage divider is 10k and 20K so 1/3
  dataS.dataS3 = nivel_bateria;
  return;
}


/////////////BOTONES////////////////////////////////////////////////
void botones() {
  if (dataR.ButY == 1 && armado == 0) {
    servo_acelerador.attach(ACELERADOR);
    Sdir.attach(SERVO1);
    servo_acelerador.write(90);
    delay(2000);
    Serial.println("Armado");
    Luces(2); // Luces parpadean simulando arranque
    armado = 1;
  }
  if (dataR.ButY == 0 && armado == 1) {
    servo_acelerador.detach();
    Sdir.detach();
    armado = 0;
    Serial.println("Desarmado");
    Lfun = 3;
    Luces(Lfun);
    delay(1000);
  }
  //Enciende las luces
  if (dataR.ButX == 1) {
    estadoLuces = true;
  } else {
    digitalWrite(LUCESDEL, LOW);
    digitalWrite(LUCESTRAS, LOW);
    digitalWrite(LUCESINT, LOW);
    estadoLuces = false;
  }

  //////Recibe boton A (Caja de marchas)
  if (dataR.ButA == 1 && Marcha == 0) {
    Marcha = 1;
    Serial.println("MARCHA REDUCTORA");
    for (int a = MLARGA; a >= MCORTA; a--) {
      Marchas.write(a);
      delay(20);
    }
    Luces(2);
  }
  if (dataR.ButA == 0 && Marcha == 1) {
    Marcha = 0;
    Serial.println("MARCHA LARGA");
    for (int b = MCORTA; b <= MLARGA; b++) {
      Marchas.write(b);
      delay(20);
    }
    Luces(2);
  }

  //////Recibe freno Boton B
  if (dataR.ButB == 1) {
    freno = true;
    analogWrite(LUCESTRAS, LMAX);
    servo_acelerador.write(90);
  } else {
    freno = false;
  }

}
void Luces(int estado) {
  bitdir = estado;
  switch (estado) {
    
    case 1: //Luces Encendidas posición normal
      analogWrite(LUCESDEL, LMAX);
      analogWrite(LUCESTRAS, LMIDTRAS);
      analogWrite(LUCESINT, LMAX);
      break;
    case 2: //ARRANQUE y CAMBIO DE MARCHA
      Serial.println("LUCES CASE 2");

      for (int x = 0; x < 6; x++) {
        analogWrite(LUCESDEL, LMID);
        analogWrite(LUCESTRAS, LMID);
        analogWrite(LUCESINT, LMID);
        //        digitalWrite(LUCESTRAS, HIGH);
        delay (50);
        analogWrite(LUCESDEL, LMIN);
        analogWrite(LUCESTRAS, LMIN);
        analogWrite(LUCESINT, LMIN);
        //        digitalWrite(LUCESTRAS, LOW);

        delay(50);
      }

      Lfun = 1;
      break;
    case 3: // Luces antes de encender el motor
      //  Serial.println("LUCES CASE 3");
      analogWrite(LUCESDEL, LMIN);
      analogWrite(LUCESTRAS, LMIN);
      analogWrite(LUCESINT, LMIN);

      break;
    case 4: //ERROR
      Serial.println("LUCES CASE 4");
      Sdir.detach();
      analogWrite(LUCESDEL, LMID);
      analogWrite(LUCESTRAS, LMID);
      //digitalWrite(LUCESTRAS, HIGH);
      delay (500);
      analogWrite(LUCESDEL, LMIN);
      analogWrite(LUCESTRAS, LMIN);
      //digitalWrite(LUCESTRAS, LOW);

      delay(500);
      break;
      case 5:
      digitalWrite(LUCESDEL, HIGH);
      digitalWrite(LUCESTRAS, LOW);
      digitalWrite(LUCESINT, LOW);

      //digitalWrite(LUCESTRAS, HIGH);
      delay (250);
      digitalWrite(LUCESDEL, LOW);
      digitalWrite(LUCESTRAS, HIGH);
      digitalWrite(LUCESINT, HIGH);  
      break;
  }
}
