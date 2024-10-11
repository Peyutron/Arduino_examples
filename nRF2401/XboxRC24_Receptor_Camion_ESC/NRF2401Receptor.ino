void setupRadio() {

  radio.begin();
  // Asignamos un canal, el 108 por defecto
  radio.setChannel(108);
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);
  
  // transmitir nivel de potencia. Opcional: RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setPALevel (RF24_PA_MAX); 

  // tasa de cambio. Opcional RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  // debe ser el mismo en el receptor y el transmisor!
  // ¡ a la velocidad más baja, tenemos la mayor sensibilidad y rango!
  radio.setDataRate (RF24_250KBPS); 
  Serial.println("\nCONFIGURACION CARGADA");

  radio.powerUp(); 
  radio.startListening();
}

// Comprueba si hay datos disponibles en la transmisión
// Si hay datos se cargan en la variable "dataR"
void receiver(){
    // Si hay datos disponibles
    if ( radio.available()) {
    
    // Espera en bucle hasta recibir el mensaje
    while (radio.available()) {              
      radio.read( &dataR, sizeof(dataR) );  
      lastRecvTime = millis();              
    }
    
    // Dejamos de escuchar para poder envíar
    radio.stopListening();

    radio.write( &dataS, sizeof(dataS) );
    
    // Volvemos a la escucha para recibir más paquetes
    radio.startListening();
  }
}
void receiverDebug() {
 /* Serial.print("Dato Recibido = ");
  Serial.print("Canal 0 = ");
  Serial.print(dataR.dec);
  Serial.print(" Canal 1 = ");
  Serial.print(dataR.ace);
  Serial.print(" Canal 2 = ");
  Serial.print(dataR.LXdata);*/
/*  Serial.print(" Canal 3 = ");
  Serial.print(dataR.LYdata);
  Serial.print(" Canal 4 = ");
  Serial.print(dataR.RXdata);
  Serial.print(" Canal 5 = ");
  Serial.print(dataR.RYdata);*/
  /*Serial.print(" Canal 6 = ");
  Serial.print(dataR.ButA);
  Serial.print(" Canal 7 = ");
  Serial.print(dataR.ButB);
  Serial.print(" Canal 8 = ");
  Serial.print(dataR.ButX);
  Serial.print(" Canal 9 = ");
  Serial.println(dataR.ButY);*/
    /*  Serial.print("T0: ");
    Serial.print(accT0);
    Serial.print(" T1: ");
    Serial.print(accT1);
    Serial.print(" Aceleracion: ");
    Serial.print(acc);*/
    Serial.print(" Señal: ");
    Serial.print(dataR.LXdata);
    Serial.print(" Servo: ");
    Serial.print(pos);
    Serial.print( " Conectado? ");
    Serial.println(conexion);
    //Serial.print(" T1: ");
    //Serial.println(accT1);

 
}
/*void sendDebug(){
    Serial.println(dataS.dataS0);
    Serial.println(dataS.dataS1);
    Serial.println(dataS.dataS2);
    Serial.println(dataS.dataS3);
    Serial.println(dataS.dataS4);
  }*/
