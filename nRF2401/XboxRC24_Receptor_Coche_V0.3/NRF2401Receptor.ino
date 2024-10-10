// Comprueba si hay datos disponibles en la transmisión
// Si hay datos se cargan en la variable "dataR"
void receiver()
  {
    // Si hay datos disponibles
    if (radio.available())
      {
      bool done = false;

    // Espera en bucle hasta recibir el mensaje
    while (!done)
        {
        done = radio.read( &dataR, sizeof(dataR));
        receiverDebug();
        }
      }
    return;
  }

void receiverDebug()
  {
  Serial.println("Dato Recibido =");
  Serial.print("Canal 0 = ");
  Serial.println(dataR.dec);
  Serial.print("Canal 1 = ");
  Serial.println(dataR.ace);
  Serial.print("Canal 2 = ");
  Serial.println(dataR.LXdata);
  /* Serial.print("Canal 3 = ");
     Serial.println(dataR.LYdata);
     Serial.print("Canal 4 = ");
     Serial.println(dataR.RXdata);
     Serial.print("Canal 5 = ");
     Serial.println(dataR.RYdata);
  */
  Serial.print("Canal 6 = ");
  Serial.println(dataR.ButA);
}
