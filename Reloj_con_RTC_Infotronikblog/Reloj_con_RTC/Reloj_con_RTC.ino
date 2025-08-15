// -------------------------------------------------------
// Reloj digital simple con calendario
// Developed by Darren Yates for APC magazine
// Traducido y modificado por Peyutron para Infotronikblog
// Se han sustituido los botones un modo analógico por 
// dos pulsadores
// -------------------------------------------------------
#include <LiquidCrystal.h>
#include <TimedAction.h>
#include <Wire.h>
#include <EEPROM.h>

LiquidCrystal lcd(8,9,4,5,6,7);
int keyIn;
int pul_select = 11;
int pul_conf= 12;

int key=-1;
int oldkey=-1;
int adjHours;
int address = 0;
byte savedMode;

char dia[7][10] = { "Lun","Mar","Mie","Jue","Vie","Sab","Dom"};
char mes[12][11] = { "Enero    ","Febrero ","Marzo ","Abril ","Mayo ","Junio ","Julio ","Agosto","Septiembre","Octubre   ","Noviembre","Diciembre" };
char hourTime [24][5] = { 
  "12am","1am","2am","3am","4am","5am","6am","7am","8am","9am","10am","11am",
  "12pm","1pm","2pm","3pm","4pm","5pm","6pm","7pm","8pm","9pm","10pm","11pm" };
byte DiaMes[12] = {
  31,28,31,30,31,30,31,31,30,31,30,31};
char pregunta [6][16] = { "Selec. ano:","Selec. mes:","Sel. dia/fecha:","Selec. dia:","Selec. hora:","Selec. minuto:" };

int qNum = 0;
int tempVar = 0;
int maxVar = 0;
int minVar = 0;
int delayTiempo = 1500;
int startClock = 0;

int selDia = 0;
int selDiaSem = 1;
int selMes = 0;
int selAno = 15;
int selHora = 0;
int selMin = 0;
int secs = 0;
String strMin = "";
String strSecs = "";
String strHora = "";
String hora12 = "";
String topLine = "";
String botLine = "";
String spaceChar = " ";


void setup() {
pinMode (pul_select,INPUT);
pinMode (pul_conf,INPUT);  
Wire.begin();

  lcd.begin (16,2);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Infotronikblog");
  lcd.setCursor(0,1);
  lcd.print("Reloj Calendario");
  delay(delayTiempo);
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.print("Poner en hora");
  lcd.setCursor(0,1);
  lcd.print("usando el boton");
  delay(delayTiempo);
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.print("Selct y presiona");
  lcd.setCursor(0,1);
  lcd.print("el boton Conf ");
  delay(delayTiempo);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("para continuar");
  delay(delayTiempo);
  //PonerHora();
  }
  TimedAction timer = TimedAction(1000,clockUpdate);
  void PonerHora(){
  delay(1000);  
  for (qNum = 0; qNum < 6; qNum++)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(pregunta[qNum]);

    keyIn = 0;
    waitForSelect();
  } 
  lcd.clear();
}

void loop() {
  get_time();
  get_date();
timer.check();
 // lcd.clear(); // clear LCD screen    //do I need this? is this causing the 'blink'?
//if (startClock ==1 ) {timer.check();} 
  
  if (digitalRead(pul_select)==LOW && digitalRead(pul_conf)==LOW){
  PonerHora();
  }

} 

void waitForSelect() {
  switch (qNum) {
  case 0: 
    tempVar = selAno;  
    maxVar = 50; 
    minVar = 15; 
    break;
  case 1:
    tempVar = selMes; 
    maxVar = 11; 
    minVar = 0; 
    break;
  case 2:
    tempVar = selDiaSem; 
    maxVar = DiaMes[selMes]; 
    minVar = 1; 
    break;
  case 3:
    tempVar = selDia; 
    maxVar = 6; 
    minVar = 0; 
    break;
  case 4:
    tempVar = selHora; 
    maxVar = 23; 
    minVar = 0; 
    break;
  case 5:
    tempVar = selMin; 
    maxVar = 59; 
    minVar = 0;
  }

  lcd.setCursor(0,1);
  displayVars();

  while (keyIn != 4) { 

  
if (digitalRead(pul_conf)==LOW) {
  delay(300);
      switch (qNum) {
      case 0:
        selAno = tempVar;        
        set_date(); 
        break;
      case 1:
        selMes = tempVar; 
        set_date();
        break;
      case 2:
        selDiaSem = tempVar; 
        set_date();
        break;
      case 3:
        selDia = tempVar; 
        set_date();
        break;
      case 4:
        selHora = tempVar; 
        set_time();
        break;
      case 5:
        selMin = tempVar;
        set_time();
        //startClock = 1;
        
      }
      keyIn = 4;
    } 

if (digitalRead(pul_select) ==LOW) {
  tempVar++;
  if (tempVar> maxVar){
    tempVar = minVar;
    }
    delay(200);
  }      
      lcd.setCursor(0,1);
      displayVars();
    }
    delay(50);
  }


void displayVars() {
  switch (qNum) {
  case 0:
    lcd.print(tempVar + spaceChar); 
    break;
  case 1:
    lcd.print(mes[tempVar] + spaceChar); 
    break;
  case 2:
    lcd.print(tempVar + spaceChar); 
    break;
  case 3:
    lcd.print(dia[tempVar] + spaceChar); 
    break;
  case 4:
    lcd.print(hourTime[tempVar] + spaceChar); 
    break;
  case 5:
    lcd.print(tempVar + spaceChar);  
  }        
}

void clockUpdate() {
  secs++;
  if (secs == 60) {
    secs = 0;
    selMin++;
  }
  if (selMin == 60) {
    selMin = 0;
    selHora++;
  }
  if (selHora == 24) {
    selHora = 0;
    selDia++;
    if (selDia ==7) {
      selDia = 0;
    }
    selDiaSem++;
  }
  if (selDiaSem > DiaMes[selMes]) {
    selDiaSem = 1;
    selMes++;
    if (selMes == 12) {
      selMes = 0; 
      selAno++;
    }
  }    
  if (secs < 10) {
    strSecs = "0"+String(secs);
  } 
  else {
    strSecs = String(secs);
  }
  if (selMin < 10) {
    strMin = "0"+String(selMin);
  } 
  else {
    strMin = String(selMin);
  }
  if (selHora < 12) {
    hora12 = "am"; 
    if (selHora==0) {
      strHora = "12";
    } 
    else {
      strHora = String(selHora);
    }
  } 
  else { 
    hora12 = "pm"; 
    strHora = String(selHora); 
  }
  if (selHora > 12) {
    strHora = String(selHora-12);
  }

  topLine = String(dia[selDia]).substring(0,3)+","+spaceChar+selDiaSem+spaceChar+String(mes[selMes]).substring(0,3)+spaceChar
    +selAno+spaceChar;
  botLine = strHora+":"+strMin+":"+strSecs+hora12+spaceChar;
  lcd.setCursor(0,0);
  lcd.print(topLine);
  lcd.setCursor(3,1);
  lcd.print(botLine);
}


void initChrono()
{
  set_time();
  set_date();
}

void set_date()
{
  Wire.beginTransmission(104);
  Wire.write(3);
  Wire.write(decToBcd(selDia));
  Wire.write(decToBcd(selDiaSem));
  Wire.write(decToBcd(selMes));
  Wire.write(decToBcd(selAno));
  Wire.endTransmission();
}

void get_date()
{
  Wire.beginTransmission(104); 
  Wire.write(3);//set register to 3 (day)
  Wire.endTransmission();
  Wire.requestFrom(104, 4); //get 5 bytes(day,date,month,year,control);
  selDia   = bcdToDec(Wire.read());
  selDiaSem  = bcdToDec(Wire.read());
  selMes = bcdToDec(Wire.read());
  selAno  = bcdToDec(Wire.read());
}

void set_time()
{
  Wire.beginTransmission(104);
  Wire.write(0);
  Wire.write(decToBcd(secs));
  Wire.write(decToBcd(selMin));
  Wire.write(decToBcd(selHora));
  Wire.endTransmission();
}

void get_time()
{
  Wire.beginTransmission(104); 
  Wire.write(0);//set register to 0
  Wire.endTransmission();
  Wire.requestFrom(104, 3);//get 3 bytes (seconds,minutes,hours);
  secs = bcdToDec(Wire.read() & 0x7f);
  selMin = bcdToDec(Wire.read());
  selHora = bcdToDec(Wire.read() & 0x3f);
}

///////////////////////////////////////////////////////////////////////

byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
clockUpdate();
}

