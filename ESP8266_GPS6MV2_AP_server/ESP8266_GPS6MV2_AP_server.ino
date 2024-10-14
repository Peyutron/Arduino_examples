/* ESP8266 GPS-6MV2 Access point
 * Comunicación con módulo GPS-6MV2 por librería software 
 * Crea un punto de acceso con dirección 192.168.4.1 y transmite los datos
 * recibidos del módulo GPS-6MV2. Los datos se pueden mostrar en una interfaz tipo:
 * 
 * 
 * ESP8266 NodeMCU
 * Módulo GPS-6MV2 
 * Librería SoftwareSerial
 * Autor: Carlos Muñoz
 * Información: https://www.infotronikblog.com/2024/10/esp8266-ap-con-gps-6mv2-socket-interface.html
 * Web: https://www.infotronikblog.com
 */

#include "ESP8266WiFi.h"
// Librerías para GPS6MV2
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

const char* ssid = "gpsServer";
const char* password =  "!2345678";
WiFiServer wifiServer(1314);

/*
    Este ejemplo demuestra el uso de TinyGPSPlus, requiere el uso 
    de SoftwareSerial, para el dispositivo GPS-6MV2 usaremos 9600-baud 
    y conectado al pin GPIO 0 (Rx) y el GPIO 2(Tx) del microcontrolador
    ESP8266 NodeMCU.
*/
static const int RXPin = 0, TXPin = 2;
static const uint32_t GPSBaud = 9600;
String gpsdate = "";
String gpstime = "";

// Objeto TinyGPSPlus 
TinyGPSPlus gps;

// Conexión serial con el dispositivo GPS
SoftwareSerial ss(RXPin, TXPin);

unsigned long previousMillis = 0;
const long interval = 500;

// Inicia el Serial, GPS-6MV2 y comunicación como server

void setup() {
	Serial.begin(115200);
	
  delay(10);

	ss.begin(GPSBaud);
 
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("Starting GPS-6MV2 in AP mode on IP: ");
  Serial.println(IP);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  wifiServer.begin();
}

// Inicia la función principal
void loop() {
  
 WiFiClient client = wifiServer.available();
 
  if (client) {
    Serial.println("Client connected");
    while (client.connected()) {
    
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval){
        previousMillis = currentMillis;
        Serial.println(neo6mv2data());
        client.print(neo6mv2data());
      }
    smartDelay(500);
    if (millis() > 5000 && gps.charsProcessed() < 10)  Serial.println(F("No GPS data, please check wiring!"));
    } 
  client.stop();
  Serial.println("Client disconnected");
  } 
}

static void smartDelay(unsigned long ms) {
	unsigned long start = millis();
	do {
		while (ss.available()) {
			gps.encode(ss.read());
		}
	} while (millis() - start < ms);
}
