
// Funcion del menu de seleccion de vehiculo
// Con esta funcion navegaremos entre las diferentes opciones (coche, tanque, barco, etc...)

// MENU PRINCIPAL
void menu() {
  int cambio = 0;
  int x = 1;
  while  (x == 1) {
    lcd.setCursor(0, 0); // el cursor en la posicion 0 linea 0
    lcd.blink();         // Activa el parpadeo del cursor

    cambio = analogRead(pinJoyRY);  // Joystick izquierdo eje Y en el pin analogico A3
    int menuMax;
    if (menuPrincipal) {          // Selecciona el menu: 0 para menu principal
      menuMax = numeroDeMenu;                   // 1 para menu calibracion
    } else {
      menuMax = numeroDeMenuCal;
    }
    if (btnmenu > menuMax - 1 ) btnmenu = 0;
    //  Serial.println(String(btnmenu));
    delay(200);
    //lee el Joystic para subir el menu
    if ((cambio >= 800) && (xmenu < (menuMax - 2))) {  // Subir una linea
      Serial.print ( xmenu);
      Serial.print ( menuMax);
      Serial.println(" baja linea");
      if ( btnmenu <= menuMax - 2) {
        xmenu++;
        btnmenu++;
        flagmenu = true;
        lcd.noBlink();
      }
    }
    if ((cambio <= 250) && (xmenu > 0)) {  // Bajar una linea
      Serial.print ( xmenu);
      Serial.print ( menuMax);
      Serial.println(" Sube linea");

      xmenu--;
      btnmenu--;
      flagmenu = true;
      lcd.noBlink();
    }

    ////////////Menu Principal//////////////////////
    if (flagmenu && menuPrincipal) {
      lcdClear();
      printLine(tituloMenu[xmenu], 0, 0);
      printLine(tituloMenu[xmenu + 1], 0, 1);
    }

    ////////////Menu Configuracion//////////////////////
    if (flagmenu && !menuPrincipal) {
      lcdClear();
      printLine(tituloMenuCal[xmenu], 0, 0);
      printLine(tituloMenuCal[xmenu + 1], 0, 1);
    }
    flagmenu = false; // Ponemos flagmenu en false para que no este escribiendo todo el rato en la pantalla

    // Boton select para cambiar entre Menu Prin. y Menu Conf
    if (digitalRead(butSelect) == LOW) {
      delay(300);
      menuPrincipal = !menuPrincipal;
      lcd.clear();
      lcd.noBlink();
      flagmenu = true;
      xmenu = 0;
      btnmenu = 0;
      menu();
    }
    if (digitalRead(butStart) == LOW) {
      delay(300);
      if (menuPrincipal) {
        switch (btnmenu) {
          case AVION:
            modo_uso = AVION;
            chmode2();
            break;
            case CAMION:
            modo_uso = CAMION;
            chmode2();
            break;
          case COCHE:
            modo_uso = COCHE;
            chmode2();
            break;
          case TANQUE:
            modo_uso = TANQUE;
            chmode2();
            break;
          default:
            break;
        }
        lcd.clear();
        lcd.noBlink();
        model(); // Entra en el modo seleccionado

        // MENU CALIBRAR
      } else {
        // Funcion del menu de seleccion para la calibracion de los joysticks
        // y gatillos. Con esta funcion navegaremos entre las diferentes
        // opciones ( Gatillo 0, Gatillo 1, RX, RY,LX, LY). calibracion(int Npin, int IndiceArray);
        switch (btnmenu) {
          case 0:
            NRF24INFO();  // LLama a la funcion que muestra en pantalla informacion
                          // sobre el canal y nivel de RF 
            break;
          case 1:
            calibracion(0, 0);    // LLama a la funcion calibrar eje RX
            break;
          case 2:
            calibracion(1, 1);    // LLama a la funcion calibrar eje RY
            break;
          case 3:
            calibracion(2, 2);    // LLama a la funcion calibrar eje LX
            break;
          case  4:
            calibracion(3, 3);    // LLama a la funcion calibrar eje LY
            break;
          case 5:
            calibracion(7, 4);    // LLama a la funcion calibrar gatillo 0
            break;
          case 6:
            calibracion (6, 5);   // LLama a la funcion calibrar gatillo 1
            break;
          case 7:
            SetChannel();
            break;
          case 8:
            resetEeprom();
            break;
          default:
            break;
        }
        lcd.clear();
        lcd.noBlink();
      }
    }
  }
}
