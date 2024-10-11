
void calibracion (int xpin, int varCal) {

  lcd.noBlink();
  String deb = (String)xpin + " " + (String)varCal;
  Serial.println(deb);

  bool inver;
  //Serial.println("Menu calibracion");
  bool  salida = false;
  int  z = 0;
  int joy = 0;

  while  (salida == false) {
    // Configura el minimo
    if (z == 0) {
      //    varJoy[varCal] = analogRead(xpin);
      joy = analogRead(xpin);
      //      varJoy[varCal] = joy;
      lcd.clear();
      String Linea0 = "Eje " + (String)analogPin[xpin] + " Min";
      printLine(Linea0, 0, 0);
      String Linea1 = "Valor: " + (String)joy;
      printLine(Linea1, 2, 1);
      delay(50);

      // Configura el valor mínimo
      // Si el el valor del joystick es menor de 250 y presionamos el boton start...
      if ((digitalRead(butStart) == LOW) && (z == 0) && (joy < 350)) {
        int memMin = (joy / 4);
        EEPROM.write( varCal, memMin);
        delay(20);
        varJoyMin[varCal] = EEPROM.read(varCal) * 4;
        //varJoyMin[varCal] = joy;
        Serial.println(EEPROM.read(varCal));
        lcd.clear();
        String Linea0 = "Min.Guardado";
        printLine(Linea0, 2, 0);  // Seleccionamos posicion y linea
        printLine((String)EEPROM.read(varCal), 4, 1);
        String Linea1 = "OK";
        printLine(Linea1, 9, 1);
        z = 1;
        delay (2000);
      }
    }
    if (z  ==  1) {
      // Configura el valor máximo
      joy  = analogRead(xpin); // analogPin tiene que ser una variable tipo char como la de los menu.
      lcd.clear();
      String Linea0 = "Eje " + (String)analogPin[xpin] + " Max";
      printLine(Linea0, 0, 0);
      String Linea1 = "Valor: " + (String)joy;
      printLine(Linea1, 2, 1);
      //varJoy[varCal] = joy;
      delay(100);
      if ((digitalRead(butStart) == LOW) && (z == 1) && (joy > 700)) {
        int memMax = joy  / 4;
        if (memMax >= 255) {
          memMax = 254;
          Serial.println("Top mayor que 255");
        }
        EEPROM.write(10 + varCal, memMax);

        delay(20);
        varJoyTop[varCal] = EEPROM.read(10 + varCal) * 4;
        Serial.print("Guardando en posicion la memoria.. ");
        Serial.print (10 + varCal);
        Serial.print(" de la memoria. Valor: ");
        Serial.print(EEPROM.read(10 + varCal));
        Serial.print(" VarJoyTOP: ");
        Serial.println((String)varJoyTop[varCal]);


        lcd.clear();
        printLine("Max.Guardado", 2, 0);
        int resultado = EEPROM.read(10 + varCal) * 4;
        printLine((String)resultado, 4, 1);
        printLine("OK", 9, 1);
        z = 2;
        delay (2000);
        lcd.clear();

      }
    }
    if (z == 2) {   //
      // Invierte el canal analógico
      printLine(inversion(inversionJoy[varCal]), 2, 1);
      String modo;
      int cambio = analogRead(pinJoyRY); // analogPin tiene que ser una variable tipo char como la de los menu.
      if (cambio <= 200 || cambio >= 700) { // && cambio <= 400){
        delay (200);
        lcd.clear();
        inversionJoy[varCal] = !inversionJoy[varCal];
        if (inversionJoy[varCal]) {
          modo = "Invertido";
        } else {
          modo = "Normal";
        }
        printLine (modo, 2, 1);
      }

      printLine("Invertir eje", 2, 0);  // Posicion y linea
      if ((digitalRead(butStart) == LOW) ) {
        EEPROM.write(20 + varCal, inversionJoy[varCal]);
        delay(20);
        inversionJoy[varCal] = EEPROM.read(20 + varCal);
        Serial.print(" Inversion: ");
        Serial.println(EEPROM.read(20 + varCal));

        lcd.noBlink();
        lcdClear();
        flagmenu = true;
        salida = true;
      }
    }
    if (digitalRead(butSelect) == LOW) {
      delay(200);
      debug00();
      lcd.clear();
      lcd.noBlink();
      flagmenu = true;
      salida = true;
    }
    delay(100);
  }
}

// Función que retorna un String con el estado del canal analógico
String inversion (bool estado) {
  String modo;
  if (estado) return "Invertido";
  else return "Normal";
}
