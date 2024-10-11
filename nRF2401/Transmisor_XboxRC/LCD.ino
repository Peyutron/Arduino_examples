
void PantallaPrincipal() {
  switch (modo_uso) {
    case AVION:
      lcd.setCursor(0, 0);
      lcdWrite(potenciometroBar2(dataS.LXdata, MAXANIMACION));
      printLine(("X" + String(dataS.LXdata)) , 0, 0);
      
      lcd.setCursor(6, 0);
      lcdWrite(potenciometroBar2(dataS.T1data, MAXANIMACION));
      printLine(("V" + String(dataS.T1data)) , 5, 0);
      
      lcd.setCursor(0, 1);
      lcdWrite(potenciometroBar0(dataS.RXdata, MAXANIMACION));
      printLine(("X" + String(dataS.RXdata)) , 11, 0);

      lcd.setCursor(6, 1);
      lcdWrite(potenciometroBar0(dataS.RYdata, MAXANIMACION));
      printLine(("Y" + String(dataS.RYdata)) , 0, 1);
      break;

      
   case CAMION:
      lcd.setCursor(0, 0);
      lcdWrite(potenciometroBar0(dataS.LXdata-1, MAXANIMACION));
      printLine(("X" + String(dataS.LXdata)) , 1, 0);
      lcd.setCursor(6, 0);
      lcdWrite(potenciometroBar0(dataS.T1data-1, MAXANIMACION));
      printLine(("V" + String(dataS.T1data)) , 7, 0);
      lcd.setCursor(0, 1);
      lcdWrite(potenciometroBar0(dataS.T0data-1, MAXANIMACION));
      printLine(("R" + String(dataS.T0data)) , 1, 1);
      break;
          
    case COCHE:
      lcd.setCursor(0, 0);
      lcdWrite(potenciometroBar0(dataS.LXdata-1, MAXANIMACION));
      printLine(("X" + String(dataS.LXdata)) , 1, 0);
      lcd.setCursor(6, 0);
      lcdWrite(potenciometroBar0(dataS.T1data-1, MAXANIMACION));
      printLine(("V" + String(dataS.T1data)) , 7, 0);
      lcd.setCursor(0, 1);
      lcdWrite(potenciometroBar0(dataS.T0data-1, MAXANIMACION));
      printLine(("R" + String(dataS.T0data)) , 1, 1);
      break;
    case TANQUE:
      ////Linea 0////
      lcd.setCursor(0, 0);
      lcdWrite(potenciometroBar0(dataS.LXdata-1, MAXANIMACION));
      printLine("X" + (String)dataS.LXdata, 1, 0);
      lcd.setCursor(6, 0);
      lcdWrite(potenciometroBar1(dataS.T1data-1, MAXANIMACION));
      printLine("V" + (String)dataS.T1data, 7, 0);
      ////Linea 1////
      lcd.setCursor(0, 1);
      lcdWrite(potenciometroBar0(dataS.LYdata-1, MAXANIMACION));
      printLine("Y" + (String)dataS.LYdata, 1, 1);
      break;
  }

  if (estadoButA == 1) printLine("A", 12, 0);
  else printLine(" ", 12, 0);
    if (estadoButB == 1) printLine("B", 13, 0);
  else printLine(" ", 13, 0);
    if (estadoButX == 1) printLine("X", 14, 0);
  else printLine(" ", 14, 0);
    if (estadoButY == 1) printLine("Y", 15, 0);
  else printLine(" ", 15, 0);
  printLine((String)bat, 7, 1);
  printLine("V", 11, 1);
  printLine("T", 12, 1);
  printLine("R", 14, 1);

}
// Funcion encargada de mostrar los datos en la pantalla LCD
// de la funcion tankmode en la pestaña "Modo_Tanque"


unsigned char potenciometroBar0(float valor, float maximo) {
  double temp;
  temp = 5 * valor / maximo;
  return (int)temp;
}

unsigned char potenciometroBar1(float valor, float maximo) {
  double temp;
  temp =  5 * valor / maximo;
  return (int)temp;
}
// Funcion encargada de mostrar los datos en la pantalla LCD
// de la funcion modeCar en la pestaña "Modo_Coche"


unsigned char potenciometroBar2(float valor, float maximo) {
  double temp;
  temp = 5 * valor / maximo;
  return (int)temp;
}

unsigned char potenciometroBar3(float valor, float maximo) {
  double temp;
  temp =  8 * valor / maximo - 1;
  return (int)temp;
}

// Muestra la presentacion del inicio en el LCD
void presentacion() {
  printLine("Infotronikblog", 17, 0);
  printLine("RCXbox24 V0.6", 17, 1);
  for (int Contador = 0; Contador < 16; Contador++) {
    // scroll una posicion a la izquierda
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(20);
  }
  delay(1000);
  return;
}

void lcdWrite (int line1) {
  lcd.write(line1);
  line1 = 0;
}


void printLine(String line0, int pos, int linea) {
  lcd.setCursor(pos, linea);
  lcd.print(line0);
  line0 = "";
}

void lcdClear() {
  delay(25);
  lcd.clear();
}
