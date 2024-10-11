// Funcion principal de modelo
/*
   Botones A,B,X, Y, Start y Select.
   Start: Entra en menu Calibración
   Select:Sale al menu seleccion modelo
*/
void model() {

  int modoButA = 0;
  int modoButB = 0;
  int modoButX = 0;
  int x = 1;    // Condicion para while, si es 0 sale del bucle
  recepcion = true;
  int LXmidAvion0 = 0;
  int LXmidAvion1 = 0;
  int LXmidAvion2 = 0;
  int LXmidAvion3 = 0;
  int LXmidTruck0 = 0;
  int LYmidTruck1 = 0;
  int LXmidCar = 0;
  int LXmidTank0 = 0;
  int LYmidTank1 = 0;
  debug00();
  while  (x == 1) {
    switch (modo_uso) {
      case AVION:
        // T1 Acelerador
        dataS.T1data = mapeo_gatillos(analogRead(pinJoyT1),  varJoyMin[T1], varJoyTop[T1], inversionJoy[T1]);
        // dataS.T1data = mapeo_gatillos(analogRead(pinJoyT1),  0, varJoyTop[1] * 4, inversionJoy[1]);

        // LX Eje izquierdo X
        LXmidAvion0 = ((varJoyTop[LX]) / 2 );
        dataS.LXdata =  mapeo_normal(analogRead(pinJoyLX), varJoyMin[LX] , LXmidAvion0, varJoyTop[LX], inversionJoy[LX]);

        // LY ALERRONES ALAS
        //LXmidAvion1 = ((varJoyTop[3]) / 2);
        //dataS.LYdata =  mapeo_normal(analogRead(pinJoyLY), varJoyMin[3] , LXmidAvion1, varJoyTop[3] * 4, inversionJoy[3]);

        // LY TIMON DE COLA
        LXmidAvion2 = ((varJoyTop[RX]) / 2);
        dataS.RXdata =  mapeo_normal(analogRead(pinJoyRX), varJoyMin[RX] , LXmidAvion2, varJoyTop[RX], inversionJoy[RX]);
        // Serial.print((String)dataS.RXdata);

        // LY CABECEO
        LXmidAvion3 = ((varJoyTop[RY]) / 2);
        dataS.RYdata =  mapeo_normal(analogRead(pinJoyRY), varJoyMin[RY] , LXmidAvion3, varJoyTop[RY], inversionJoy[RY]);
        // Serial.println((String)dataS.RYdata);
        break;

      case CAMION:
        // T0 Freno
        dataS.T0data = mapeo_gatillos(analogRead(pinJoyT0),  varJoyMin[T0], varJoyTop[T0], inversionJoy[T0]);

        // T1 Acelerador
        dataS.T1data = mapeo_gatillos(analogRead(pinJoyT1),  varJoyMin[T1], varJoyTop[T1], inversionJoy[T1]);

        // LY Dirección
        LXmidTruck0 = (varJoyTop[LX] / 2 );
        dataS.LXdata =  mapeo_normal(analogRead(pinJoyLX), varJoyMin[LX] , LXmidTruck0, varJoyTop[LX], inversionJoy[LX]);

        // ponemos a 0 en este punto para que actue como un pulsador
        estadoButB = 0;
        break;

      case COCHE:
        // T0 Freno
        dataS.T0data = mapeo_gatillos(analogRead(pinJoyT0),  varJoyMin[T0], varJoyTop[T0], inversionJoy[T0]);

        // T1 Acelerador
        dataS.T1data = mapeo_gatillos(analogRead(pinJoyT1),  varJoyMin[T1], varJoyTop[T1], inversionJoy[T1]);

        // LX Dirección
        LXmidCar = ((varJoyTop[LX]) / 2 );
        dataS.LXdata =  mapeo_normal(analogRead(pinJoyLX), varJoyMin[LX] , LXmidCar, varJoyTop[LX], inversionJoy[LX]);
        break;

      case TANQUE:
        // T1 Acelerador
        dataS.T1data = mapeo_gatillos(analogRead(pinJoyT1),  varJoyMin[T1], varJoyTop[T1], inversionJoy[T1]);
        //dataS.T1data = mapeo_gatillos(analogRead(pinJoyT1),  0, varJoyTop[1] * 4, inversionJoy[1]);

        // LX Eje izquierdo (X)
        LXmidTank0 = ((varJoyTop[LX]) / 2 );
        dataS.LXdata =  mapeo_normal(analogRead(pinJoyLX), varJoyMin[LX] , LXmidTank0, varJoyTop[LX], inversionJoy[LX]);

        // LY Eje Izquierdo Y
        LYmidTank1 = ((varJoyTop[LY]) / 2);
        dataS.LYdata =  mapeo_normal(analogRead(pinJoyLY), varJoyMin[LY] , LYmidTank1, varJoyTop[LY], inversionJoy[LY]);
        break;
    }

    // Pulsador SELECT para salir al menu principal
    if ((digitalRead(butSelect) == LOW)) {
      lcd.clear();
      lcd.blink();
      xmenu = 0;
      btnmenu = 0;
      flagmenu = true;
      x = 0;
      menu();
    }
    if (digitalRead(butStart) == LOW) {
      delay(200);
      // Pulsador para entrar en calibracion
      switch (modo_uso) {
        case AVION:
          calibracion(1, T1);  // Calibracion analogico pin A1 Acelerador
          calibracion(2, LX);  // Calibracion analogico pin A2 LX
          calibracion(3, LY);  // Calibracion analogico pin A3 LY
          calibracion(7, RX);  // Calibracion analogico pin A5 RY
          calibracion(6, RY);  // Calibracion analogico pin A6 RX
          break;
        case CAMION:
          calibracion(0, T0);  // Calibracion analogico pin A0 Marcha atras / Freno
          calibracion(1, T1);  // Calibracion analogico pin A1 Acelerador
          calibracion(2, LX);  // Calibracion analogico pin A2 LX
          debug00();
          break;
        case COCHE:
          calibracion(0, T0); // Calibracion analogico pin A0 Marcha atras / Freno
          calibracion(1, T1); // Calibracion analogico pin A1 Acelerador
          calibracion(2, 2);  // Calibracion analogico pin A2 LX
          calibracion(7, 4);  // Calibracion analogico pin A3 LY
          break;
        case TANQUE:
          calibracion(1, 1);  // Calibracion analogico pin A1 Acelerador
          calibracion(2, 2);  // Calibracion analogico pin A2 LX
          calibracion(3, 3);  // Calibracion analogico pin A3 LY
          break;
      }
      lcdClear();

    }

    // Pulsadores
    varButA = digitalRead(butA);
    if (varButA == LOW) {
      estadoButA = !estadoButA;
      delay (200);
    }

    varButB = digitalRead(butB);
    if (varButB == LOW) {
      estadoButB = !estadoButB;
      delay (200);
    }

    varButX = digitalRead(butX);
    if (varButX == LOW) {
      estadoButX = !estadoButX;
      delay (200);
    }

    varButY = digitalRead(butY);
    if (varButY == LOW) {
      estadoButY = !estadoButY;
      delay (200);
    }

    // Carga el estado de los botones en el array de datos
    dataS.ButA  = estadoButA;
    dataS.ButB  = estadoButB;
    dataS.ButX  = estadoButX;
    dataS.ButY  = estadoButY;

    int dataold0;
    int dataold1;
    int dataold2;
    int dataold3;
    int dataold4;
    int dataold5;

    ////////////////////////////////////////////////////

    if ((dataS.T0data != dataold0)
        || (dataS.T1data != dataold1)
        || (dataS.LXdata != dataold2)
        || (dataS.LYdata != dataold3)
        || (dataS.RXdata != dataold4)
        || (dataS.RYdata != dataold5)) {
      lcdClear();
    }
    dataold0 = dataS.T0data;
    dataold1 = dataS.T1data;
    dataold2 = dataS.LXdata;
    dataold3 = dataS.LYdata;
    dataold4 = dataS.RXdata;
    dataold5 = dataS.RYdata;

    PantallaPrincipal();
    comunicacion();

    Serial.print(dataR.dataR1);
    bat = (((dataR.dataR3 * 5.00 / 1024.00) * 4) + 0.20);
  }

}
