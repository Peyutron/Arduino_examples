/*Configurador simplificado para el modulo HC-06 mediante comandos AT
   Con este sencillo menu con el que podras:
   *Comprobar el estado del modulo HC-06
   *Cambiar el nombre del modulo
   *cambiar el pin del modulo
   *Configurar el baudrate
   *Cambiar el tipo de paridad
   *informacion sobre el firmware
   *ayuda
   Tienes toda la informacion sobre el tema en:
   http://www.infotronikblog.com
   Si tienes algun problema con el sketch puedes ponerte en contacto conmigo en el
   siguiente Email: peyutron@gmail.com
*/
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // Tx , Rx

int8_t currIn = -1;
int8_t currSt = -1;
String serialStr, tmpName, Name;

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600); //Recuerda poner aquí el Baudrate al que esta configurado el modulo
  serialStr = "";
  currIn = 0;
  currSt = 0;
  delay(1000);
  Serial.println(F("Configurador para modulo HC-06 Mediante menu por Infotronikblog"));
  }
void loop()
{
  if (Serial.available()) {
    serialStr = (Serial.readStringUntil('\n'));
    serialStr.trim();

    Serial.println(serialStr);
    currIn = serialStr.toInt();  //Cualquier tecla no numerica resultara "0"
  }

  // En caso de respuesta o entrada de de datos del modulo desde el modulo
  if (mySerial.available()) {
    while (mySerial.available()) {
      tmpName = mySerial.readString();//(mySerial.readStringUntil('\n'));
      tmpName.trim();
      Serial.println(tmpName);
      delay(150);
    }
  }

  if (currIn >= 0) {  // entra en el estado solo si es una entrada valida
    switch (currSt) { // Menu de seleccion
      case 0: {       // Este es el menu principal
          switch (currIn) { //selecciona una entrada del menu
            case 0:
              delay (1000);
              Serial.println(F("----------------------------------------------------------"));
              Serial.println(F("Selecciona una opcion de la lista:"));
              Serial.println(F("  1- Estado del Modulo HC-06:"));
              Serial.println(F("  2- Cambiar nombre del modulo HC-06"));
              Serial.println(F("  3- Cambiar el pin (por defecto '1234'"));
              Serial.println(F("  4- Configurar el Baudrate (por defecto '9600'"));
              Serial.println(F("  5- Cambiar paridad (por defecto NONE"));
              Serial.println(F("  6- Firmware"));
              Serial.println(F("  7- Ayuda"));
              Serial.println(F("     Pulsa la tecla 0 o escribe #menu para volver a este menu"));
              Serial.print(F("Opcion No. "));
             
              currIn = -1;  // Mandamos un -1 para entrar en un estado indefinido y permanecer en el menu.
              break;
            case 1:
              currSt = 1;
              currIn = 0;
              break;
            case 2:
              currSt = 2;
              currIn = 0;
              break;
            case 3:
              currSt = 3;
              currIn = 0;
              break;
            case 4:
              currSt = 4;
              currIn = 0;
              break;
            case 5:
              currSt = 5;
              currIn = 0;
              break;
            case 6:
              currSt = 6;
              currIn = 0;
              break;
            case 7:
              currSt = 7;
              currIn = 0;
              break;
            case 8:
              currSt = 8;
              currIn = 0;
              break;
            default:
              Serial.println (F("No es una entrada valida, por favor elige una de la lista"));
              currSt = 0;
              currIn = 0;
              break;
          }
          break;
        }
      //Muestra el estado del modulo  (AT OK).
      case 1: {
          Serial.println(F("----------------------------------------------------------"));
          Serial.println(F("Estado del Modulo HC-06:"));
          delay(500);
          mySerial.print("AT");
          delay(3000);
          currSt = 0;
          currIn = 0;
          break;
        }
      //Cambia el nombre del modulo (AT+NAMEnombre OKSetname)
      case 2: {
          Serial.println(F("----------------------------------------------------------"));
          Serial.println(F("Escriba el nombre o escriba 'salir'"));
          delay(500);
          while (!Serial.available());
          Name = (Serial.readStringUntil('\n'));
          Name.trim();
          if (Name == "") {
            Serial.println(F("no se ha escrito ningun nombre"));
            currSt = 2;
            currIn = 0;
            break;
          }
          if (Name == "salir") {
            currSt = 0;
            currIn = 0;
            break;
          }

          tmpName = "AT+NAME" + Name;
          mySerial.print(tmpName);
          delay(1500);
          Serial.print("Nuevo nombre: ");
          Serial.println(Name);
          delay(2500);
          currSt = 0;
          currIn = 0;
          break;
        }
              //Menu para cambiar el PIN del dispositivo (AT+PINxxxx OKSetpin).
      case 3: {
          Serial.println(F("----------------------------------------------------------"));
          Serial.println(F("Escriba el nuevo PIN o 'salir'"));
          delay(500);
          while (!Serial.available());
          Name = (Serial.readStringUntil('\n'));
          Name.trim();
          if (Name == "") {
            Serial.println(F("no se ha escrito ningun PIN"));
            currSt = 2;
            currIn = 0;
            break;
          }
          if (Name == "salir") {
            currSt = 0;
            currIn = 0;
            break;
          }

          tmpName = "AT+PIN" + Name;
          mySerial.print(tmpName);
          delay(1500);
          Serial.print("Nuevo pin: ");
          Serial.println(Name);
          delay(2500);
          currSt = 0;
          currIn = 0;
          break;
        }
      //Submenu para cambiar el Baudrate actual (AT+BAUD1,2,3,4,5,6,7,8, OK9600)
      case 4: {
          switch (currIn) {
            case 0:
              delay(1000);
              Serial.println(F("----------------------------------------------------------"));
              Serial.println(F("Elija un nuevo Baudrate o dejelo en blanco para omitir el cambio. Posibles opciones:"));
              Serial.println(F(""));
              //Serial.println(F("(1) Muestra el Baudrate actual"));
              Serial.println(F("(1) 1200      (2) 2400      (3) 4800      (4) 9600 (por defecto)"));
              Serial.println(F("(5) 19200     (6) 38400     (7) 57600     (8) 115200"));
              Serial.println(F("(9) Para salir de este menu"));
              Serial.print(F("Opcion No. "));
              currIn = -1;// Ponemos -1 para quedarnos indefinidamente en el menu.
              break;
            case 1:
              currSt = 41;
              currIn = 0;
              break;
            case 2:
              currSt = 42;
              currIn = 0;
              break;
            case 3:
              currSt = 43;
              currIn = 0;
              break;
            case 4:
              currSt = 44;
              currIn = 0;
              break;
            case 5:
              currSt = 45;
              currIn = 0;
              break;
            case 6:
              currSt = 46;
              currIn = 0;
              break;
            case 7:
              currSt = 47;
              currIn = 0;
              break;
            case 8:
              currSt = 48;
              currIn = 0;
              break;
            case 9:
              currSt = 49;
              currIn = 0;
              break;
            default:
              Serial.println (F("No es una entrada valida, por favor elige una de la lista"));
              Serial.println (F("O pulsa '9' para volver al menu principal"));
              currSt = 3;
              currIn = 0;
              break;
          }
          break;
        }
      //Submenu para cambiar la paridad del modulo
      case 5: {
          switch (currIn) { 
            case 0:
              delay (1000);
              Serial.println(F("----------------------------------------------------------"));
              Serial.println(F("Selecciona una opcion de la lista:"));
              Serial.println(F("  1- Paridad NONE"));
              Serial.println(F("  2- Paridad ODD"));
              Serial.println(F("  3- Paridad EVEN"));
              Serial.println(F("  4- Atras..."));
              Serial.println(F("Opcion No. "));
              currIn = -1;  // Ponemos -1 para quedarnos indefinidamente en el menu.
              break;
            case 1:
              currSt = 51;
              currIn = 0;
              break;
            case 2:
              currSt = 52;
              currIn = 0;
              break;
            case 3:
              currSt = 53;
              currIn = 0;
              break;
            case 4:
              currSt = 0;
              currIn = 0;
              break;
            default:
              Serial.println (F("No es una entrada valida, por favor elige una de la lista"));
              Serial.println (F("O pulsa '4' para volver al menu principal"));
              currSt = 0;
              currIn = 5;
              break;
          }
          break;
        }
      // Muestra el firmware del modulo (AT+VERSION, OKLinvoVr1.n)
      case 6: {
          Serial.println(F("----------------------------------------------------------"));
          Serial.println(F("El Firmware del modulo es:"));
          delay(500);
          mySerial.print("AT+VERSION");
          delay(1000);
          currSt = 0;
          currIn = 0;
          break;

        }
        //Informacion Sobre el programa.
      case 7: {
          Serial.println(F("----------------------------------------------------------"));
          Serial.println(F("Sobre este programa:"));
          Serial.println(F("Configurador de modulo HC-06 con comandos AT"));
          Serial.println(F("mediante una simple interfaz de menus."));
          Serial.println(F("Para un correcto funcionamiento recuerda poner el "));
          Serial.println(F("monitor serial en 'Ambos NL & CR'."));
          Serial.println(F("Pagina web: www.infotronikblog.com"));
          Serial.println(F("Email: peyutron@gmail.com"));
          Serial.println(F("Pulsa 'enter' para continuar."));
          currSt = 0;
          currIn = -1;
          break;
        }
      //Cambia el Baudrate actual a 1200 (AT+BAUD1 OK1200)
      case 41: {
          Serial.println (F("Cambiando el baudrate a 1200..."));
          delay(500);
          mySerial.print ("AT+BAUD1");
          delay(1500);
          mySerial.begin(1200);
          currSt = 4;
          currIn = 0;
          break;
        }
        //Cambia el Baudrate actual a 2400 (AT+BAUD2 OK2400)
      case 42: {
          Serial.println (F("Cambiando el baudrate a 2400..."));
          delay(500);
          mySerial.print ("AT+BAUD2");
          delay(1500);
          mySerial.begin(2400);
          currSt = 4;
          currIn = 0;
          break;
        }
        //Cambia el Baudrate actual a 4800 (AT+BAUD3 OK4800)
      case 43: {
          Serial.println (F("Cambiando el baudrate a 4800..."));
          delay(500);
          mySerial.print ("AT+BAUD3");
          delay(1500);
          mySerial.begin(4800);
          currSt = 4;
          currIn = 0;
          break;
        }
        //Cambia el Baudrate actual a 9600 (AT+BAUD4 OK9600) (configuracion por defecto)
      case 44: {
          Serial.println (F("Cambiando el baudrate a 9600..."));
          delay(500);
          mySerial.print ("AT+BAUD4");
          delay(1500);
          mySerial.begin(9600);
          currSt = 4;
          currIn = 0;
          break;
        }
        //Cambia el Baudrate actual a 19200 (AT+BAUD5 OK19200)
      case 45: {
          Serial.println (F("Cambiando el baudrate a 19200..."));
          delay(500);
          mySerial.print ("AT+BAUD5");
          delay(1500);
          mySerial.begin(19200);
          currSt = 4;
          currIn = 0;
          break;
        }
        //Cambia el Baudrate actual a 38400 (AT+BAUD6 OK38400)
      case 46: {
          Serial.println (F("Cambiando el baudrate a 38400..."));
          delay(500);
          mySerial.print ("AT+BAUD6");
          delay(1500);
          mySerial.begin(38400);
          currSt = 4;
          currIn = 0;
          break;
        }
        //Cambia el Baudrate actual a 57600 (AT+BAUD7 OK57600)
      case 47: {
          Serial.println (F("Cambiando el baudrate a 57600..."));
          delay(500);
          mySerial.print ("AT+BAUD7");
          delay(1500);
          mySerial.begin(57600);
          currSt = 4;
          currIn = 0;
          break;
        }
        //Cambia el Baudrate actual a 115200 (AT+BAUD8 OK115200)
      case 48: {
          Serial.println (F("Cambiando el baudrate a 115200..."));
          delay(500);
          mySerial.print ("AT+BAUD8");
          delay(1500);
          mySerial.begin(115200);
          currSt = 4;
          currIn = 0;
          break;
        }
      case 49: {
          currSt = 0;
          currIn = 0;
          break;
        }
        //Cambia la paridad actual a NONE (AT+PN, OK None)
      case 51: {
          Serial.println (F("Cambiando la paridad a NONE..."));
          delay(500);
          mySerial.print ("AT+PN");
          delay(1500);
          currSt = 5;
          currIn = 0;
          break;
        }
        //Cambia la paridad actual a ODD (AT+PO, OK Odd)
        case 52: {
          Serial.println (F("Cambiando la paridad a ODD..."));
          delay(500);
          mySerial.print ("AT+PO");
          delay(1500);
          currSt = 5;
          currIn = 0;
          break;
        }
        //Cambia la paridad actual a EVEN (AT+PE OK Even)
        case 53: {
          Serial.println (F("Cambiando la paridad a EVEN..."));
          delay(500);
          mySerial.print ("AT+PE");
          delay(1500);
          currSt = 5;
          currIn = 0;
          break;
        }
      default: {
          Serial.println(F("no es una entrada valida, elige una de la lista"));
          currSt = 0;
          currIn = 0;
          break;
        }
    }
  }
  serialStr = "";
//////////////////////////////////////////////////////////////
}
