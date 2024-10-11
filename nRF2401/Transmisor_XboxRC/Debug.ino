void sendDebug() {
  Serial.print("Envio: Ok...");
  Serial.print("Enviando: T0: ");
  Serial.print(dataS.T0data);  
  Serial.print(" T1: ");
  Serial.print(dataS.T1data);
  Serial.print(" LX: ");
  Serial.print(dataS.LXdata);
  Serial.print(" LY: ");
  Serial.print(dataS.LYdata);
  Serial.print(" RX: "); 
  Serial.print(dataS.RXdata);
  Serial.print(" RY: ");
  Serial.println(dataS.RYdata);

}
///////////////////////////////////

void receiverDebug() {
  Serial.println(dataR.dataR0);
  Serial.println(dataR.dataR1);
  Serial.println(dataR.dataR2);
  Serial.println(dataR.dataR3);
  Serial.println(dataR.dataR4);
}

///////////////////////////////////

// Cuando se descomenta la linea debugMemoria nos muestra en el terminal Serial
// el banco donde esta almacenado el dato y el dato almacenado
/////////////////////SOLO PARA PRUEBAS DE MEMORIA////////////////////
/*
void debugMemoria() {
  Serial.println ("Leyendo segundo bloque de datos, valores Minimos, Leyendo datos: ");
  for (int a = 0 ; a <= 9 ; a++) {
    delay(20);
    varJoyMin[a] = EEPROM.read(a) * 4;
    Serial.print ("Posicion: ");
    Serial.print (a);
    Serial.print(" ");
    Serial.println ((String)varJoyMin[a]);
    delay (20);
  }

  Serial.println("Estado de la memoria: valores Maximos, Leyendo datos: ");
  for (int b = 10 ; b <= 19 ; b++) {
    delay(20);
    varJoyTop[b] = EEPROM.read(b) * 4;
    Serial.print ("Posicion: ");
    Serial.print (b);
    Serial.print(" ");
    Serial.println ((String)varJoyTop[b]);
    delay (20);
  }
    Serial.println("Estado de la memoria: Inversion de ejes, Leyendo datos: ");
  for (int c = 20 ; c <= 29 ; c++) {
    delay(20);
    inversionJoy[c] = EEPROM.read(c);
    Serial.print ("Posicion: ");
    Serial.print (c); 
    Serial.print(" ");
    Serial.println ((String)inversionJoy[c]);
    delay (20);
  }
}*/
/////////////////////SOLO PARA PRUEBAS DE MEMORIA////////////////////
void debug00(){
    Serial.print("gatillo 0 min: ");
    Serial.println(varJoyMin[T0]);
    Serial.print("gatillo 0 Top: ");
    Serial.println(varJoyTop[T0]);
    
    Serial.print("gatillo 1 min: ");
    Serial.println(varJoyMin[T1]);
    Serial.print("gatillo 1 Top: ");
    Serial.println(varJoyTop[T1]);
    
    Serial.print("eje LX min: ");
    Serial.println(varJoyMin[2]);
    Serial.print("eje LX Top: ");
    Serial.println(varJoyTop[2]);
    
/*    Serial.print("eje LY min: ");
    Serial.println(varJoyMin[3]);
    Serial.print("eje LY Top: ");
    Serial.println(varJoyTop[3]);
    
    Serial.print("eje RX min: ");
    Serial.println(varJoyMin[4]);
    Serial.print("eje RX top: ");
    Serial.println(varJoyTop[4]);
    
    Serial.print("eje RY min: ");
    Serial.println(varJoyMin[5]);
    Serial.print("eje RY top: ");
    Serial.println(varJoyTop[5]);*/
}
