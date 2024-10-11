/*
 * Funcion con la que leemos la EEPROM y cargamos los valores máximos y mínimos
 * en el array de variables varJoyMin y VarJoyTop
*/
void readEeprom() {

  // Lee los datos de la memoria ROM para los valores minimos
  for ( int a = 0; a <= 9; a++) {
    Serial.print ("Posicion: ");
    Serial.println (a);
    varJoyMin[a] = EEPROM.read(a) * 4;
    Serial.println ((String)varJoyMin[a]);
    delay(10);
    varJoyTop[a] = EEPROM.read(10 + a) * 4;
    Serial.println ((String)varJoyTop[a]);
    delay(10);
    inversionJoy[a] = EEPROM.read(20 + a);
    Serial.println ((String)inversionJoy[a]);
    delay(10);
  }
  // Descomenta esta linea para ver en el monitor serial el banco de memoria y el dato almacenado
  // debugMemoria();
  debug00();
}

// Funcion con la que reseteamos la memoria EEPROM dejando
// todos los valores a 0
void resetEeprom() {
  lcd.setCursor(0, 0);
  lcd.print ("Borrando datos... ");

  // Limpiamos la EEMPROM byte a byte
  for (int a = 0 ; a < EEPROM.length(); a++) {
    EEPROM.write(a, 0);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print ("Memoria EEPROM ");
  lcd.setCursor(0, 1);
  lcd.print ("  borrada!");
  delay (500);
  lcd.clear();
}
