#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
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

// Inicia el Serial y la comunicación GPS-6MV2 
void setup() 
	{
	Serial.begin(115200);
	delay(1000);
  Serial.println("Iniciando GPS-6MV2...");
	ss.begin(GPSBaud);
	}
// Inicia el programa
void loop() 
	{
	Serial.println(neo6mv2data());
    smartDelay(1000);

	if (millis() > 5000 && gps.charsProcessed() < 10) 
    	{
    	Serial.println(F("No se reciben datos GPS, revise las conexiones"));
    	}
    }

static void smartDelay(unsigned long ms) {
	unsigned long start = millis();
	do 
    	{
		while (ss.available()) 
        	{
			gps.encode(ss.read());
            }
		} while (millis() - start < ms);
}

String neo6mv2data() {

  int satellites = -1;
  float hdop = 0.0;
  float location_lat = 0.0;
  float location_lng = 0.0;
  int age = 0;
  char dateBuffer[10]; // 00/00/0000
  char timeBuffer[8]; // 00:00:00
  float altitude = 0;
  float course = 0;
  float speed = 0;

  if (gps.satellites.isValid()) satellites = gps.satellites.value();
  else satellites = -1;
  if (gps.hdop.isValid()) hdop = gps.hdop.hdop();
  else hdop = -1;
  if (gps.location.isValid()) location_lat = gps.location.lat();
  else location_lat = 0.0;    
  if (gps.location.isValid()) location_lng = gps.location.lng();
  else location_lng = -1;
  if (gps.location.isValid()) age = gps.location.age();
  else age = -1;
  if (gps.altitude.isValid()) altitude = gps.altitude.meters();
  else altitude = -1;
  if (gps.course.isValid()) course = gps.course.deg();
  else course = -1;      
  if (gps.speed.isValid()) speed = gps.speed.kmph();
  else speed = -1;

  if (gps.date.isUpdated())
  {
    sprintf(dateBuffer, "%d/%d/%d", gps.date.day(), gps.date.month(), gps.date.year());
    gpsdate = String(dateBuffer);
  }
  
  if (gps.time.isUpdated())
  {
    sprintf(timeBuffer, "%d:%d:%d", gps.time.hour(), gps.time.minute(), gps.time.second());
    gpstime = String(timeBuffer);
  }
 return "<gps" 
            + String(satellites)
            + " " 
            + String(hdop)
            + " " 
            + String(location_lat,6)
            + " "
            + String(location_lng,6)
            + " "
            + String(age)
            + " "
            + gpsdate
            + " "
            + gpstime
            + " "
            + String(altitude, 2)
            + " "
            + String(course, 2)
            + " "
            + String(speed, 2)
            + ">";
}
