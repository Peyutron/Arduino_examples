
/* 
* Ejemplo de funcionamiento de un paso a nivel básico con un Arduino UNO/NANO
* Componentes:
* 1 Arduino UNO/NANO
* 2 Sensores LDR
* 1 Buzzer
* 2 Leds rojos 
* Más ejemplos en https://www.infotronikblog.com
*/

int ldr1 = 0;         // Pin analógico para LDR 1
int ldr2 = 1;         // Pin analógico para LDR 2
int ldr1_value = 0;   // Variable para almacenar el valor de LDR 1
int ldr2_value = 0;   // Variable para almacenar el valor de LDR 1
int led1 = 2;         // Led 1 en el puerto 2
int led2 = 3;         // Led 2 en el puerto 3
int campana = 4;      // Puerto para el Buzzer


void setup()
{
  // Inicia el monitor serial
  Serial.begin(115200);   
 
  // Pines de leds son salidas
  pinMode (led1, OUTPUT);
  pinMode (led2, OUTPUT);

}

void loop()
{
  // Lee el valor de LDR 1
  ldr1_value = analogRead(ldr1);  

  // Imprime el valor de LDR 1 en el monitor serial
  Serial.println(ldr1_value);     
  
  
  // Lee el valor de LDR 2
  ldr2_value = analogRead(ldr2);

  // Imprime el valor de LDR 2 en el monitor serial
  Serial.println(ldr2_value);     
  

  // Si el valor de LDR1 o LDR2 superan 800
  if ((ldr1_value >= 800) or (ldr2_value >= 800))
    {
      crossing_on();
    }
  else 
  {
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  }
  
  delay(300);
  
}




void crossing_on()
{
  for (int x  = 0; x  < 25;  x++)
  {
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
    tone (campana, 3500, 314);
    delay(300);
    digitalWrite(led2, LOW);
    digitalWrite(led1, HIGH);
    tone (campana, 3500, 314);
    delay (300);
  }
}
