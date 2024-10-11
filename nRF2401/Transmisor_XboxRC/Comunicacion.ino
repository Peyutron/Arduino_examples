
void comunicacion() {

  dataS.T0data  = constrain(dataS.T0data, 0, 1023);
  dataS.T1data  = constrain(dataS.T1data, 0, 1023);
  dataS.RXdata  = constrain(dataS.RXdata, 0, 1023);
  dataS.RYdata  = constrain(dataS.RYdata, 0, 1023);
  dataS.LXdata  = constrain(dataS.LXdata, 0, 1023);
  dataS.LYdata  = constrain(dataS.LYdata, 0, 1023);



  bool ok = radio.write( &dataS, sizeof(dataS));
  if (ok) {
    printLine("*", 13, 1);
  } else {
    printLine(" ", 13, 1);
  }
  if (recepcion) {

    // Volvemos a la escucha
    radio.startListening();

    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout )  // Esperasmos repsuesta hasta 200ms
      if (millis() - started_waiting_at > 100)timeout = true;
    //if (timeout != !timeout){
    if ( timeout ) {
      // Serial.println("Respuesta: Fallo, respuesta fuera de tiempo");
      printLine(" ", 15, 1);// Caracter RadioOff posicion 15 linea 1

    } else { // Leemos el mensaje recibido
      // Serial.print ("Respuesta: OK ");
      printLine("*", 15, 1);  //Caracter RadioOn posicion 15 linea 1
      radio.read( &dataR, sizeof(dataR));
      // receiverDebug();
    }

    // Paramos la escucha para poder enviar datos
    radio.stopListening();
  }
}

// Muestra el nivel PA accediendo desde el menu configuración
void NRF24INFO() {
  uint8_t a = radio.getPALevel();
  int n = (int)a;
  String palevel = "";
  switch (n) {
    case 0:
      palevel = "MIN";
      break;
    case 1:
      palevel = "LOW";
      break;
    case 2:
      palevel = "HIGH";
      break;
    case 3:
      palevel = "MAX";
      break;
  }
  String line1 = "Nivel PA: " + palevel;

  uint8_t b = radio.getChannel();
  String line2 = "Canal: " + (String)b;

  printLine(line1, 0, 0);
  printLine(line2, 0, 1);
  radio.printDetails();
  delay (5000);

  gomenu();
}

// Cambia el canal de comunicación acceso desde menu configuracion
void SetChannel() {
  lcdClear();
  int x = 1;
  uint8_t b = radio.getChannel();
  String Line1 = "Canal RF: " + (String)b;
  printLine(Line1, 0, 0);
  while  (x == 1) {
    int cambio = analogRead(pinJoyLY); //analogPin tiene que ser una variable tipo char como la de los menu.

    if (cambio <= 200) {
      b++;
      if (b >=125) b = 125;
      delay(150);
      lcd.clear();
      //channel++;
    }
    if (cambio >= 700) { // && cambio <= 400){
      //b--;
      if (b == 0) b = 0;
      else b--;
      delay(150);
      lcd.clear();
      //channel--;
    }
    String Line1 = "Canal RF: " + (String)b;
    printLine(Line1, 1, 0);

    if (digitalRead(butStart) == LOW) {
      radio.setChannel(b);
      printLine("Guardado", 3, 1);

    }
    // Boton SELECT para salir al menu principal
    if (digitalRead(butSelect) == LOW) {
      lcd.clear();
      lcd.blink();
      xmenu = 0;
      btnmenu = 0;
      flagmenu = true;
      x = 0;
      menu();
    }
  }
}
