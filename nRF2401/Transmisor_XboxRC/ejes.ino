

// Mapeo de la posicion de los ejes

int mapeo_normal(int valor, int bajo, int medio, int alto, bool inversion)
{
  valor = constrain(valor, bajo, alto);
  if ( valor < medio )
    valor = map(valor, bajo, medio, 0, 511);
  else
    valor = map(valor, medio, alto, 512, 1023);
  return ( inversion ? 1023 - valor : valor );
}


// Retorna el valor correcto de la posición del joiystick.
int mapeo_gatillos(int valor, int bajo, int alto, bool inversion) {
  valor = constrain(valor, bajo, alto);
  valor = map(valor, bajo, alto, 0, 1023);
  
  // float cube = ((pow((valor - 512),3)/520200) + 258.012) / 2;
  return ( inversion ? 1023 - valor : valor );
}

int mapeo_normal(int boton, bool estado, bool inversion)
{
  /*
  valor = constrain(valor, bajo, alto);
  if ( valor < medio )
    valor = map(valor, bajo, medio, 0, 511);
  else
    valor = map(valor, medio, alto, 512, 1023);
    return ( inversion ? 1023 - valor : valor );
  */
}
