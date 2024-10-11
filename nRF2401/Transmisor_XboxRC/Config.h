
#define CE_PIN  9 
#define CSN_PIN 10

//  Establezca el nivel del amplificador de potencia (PA) en uno de los cuatro niveles.


//#define LEVELRF RF24_PA_MIN  // -18db
//#define LEVELRF RF24_PA_LOW  //  -12db  //Prueba 1 ok
//#define LEVELRF RF24_PA_HIGH //  - 6db //Prueba 1 no funciona...??
#define LEVELRF RF24_PA_MAX  //    0db //Prueba 1 no funciona...??

//  Establecer la velocidad de transmisión de datos.
//  Advertencia: la configuración de RF24_250KBPS fallará para las unidades que no sean más
#define SPEEDRF RF24_250KBPS  //250kbs
//#define SPEEDRF RF24_1MBPS  //1Mbps
//#define SPEEDRF RF24_2MBPS  //2Mbps
//  nota: boolean cambio = radio.setDataRate(RF24_1MBPS); //Devuelve true si el cambio fue exitoso



//  Habilita o deshabilita los paquetes de reconocimiento automático setAutoAck (bool)

#define ACK false

//  Habilite cargas personalizadas en los paquetes de confirmación.
//  Las cargas útiles de Ack son una forma práctica de devolver datos a los remitentes 
//  sin cambiar manualmente los modos de radio en ambas unidades. enableAckPayload

#define ACKPAYLOAD  false

//  Obtiene la velocidad de transmisión de datos.
//  Devuelve la tasa de datos configurada actualmente del hardware. 
//  El valor es uno de 250kbs, RF24_1MBPS para 1Mbps o RF24_2MBPS, como se define en la enumeración rf24_datarate_e.
  //  '10' = 250KBPS
  //  result = RF24_250KBPS ;
  //  '01' = 2MBPS
  //  result = RF24_2MBPS ;
  // '00' = 1MBPS
  //  result = RF24_1MBPS ;


//  Establezca el número y el retraso de los reintentos en caso de envío fallido

//  delayRF  Cuánto tiempo esperar entre cada reintento, en múltiplos de 250us, max es 15. 0 significa 250us, 15 significa 4000us.
//  countRF  Cuántos intentos antes de darse por vencido, máximo 15

#define DELAYRF 5 //15
#define COUNTRF 5 //15

//  radio.setChannel  ( uint8_t   canal ) 
//  Establecer canal de comunicación RF.
//  En qué canal de RF comunicarse, 0-127
//  Establecer en 108 - 2.508 Ghz que esta por encima de la mayoría de los canales Wifi
//  Por defecto viene en el 76
//  uint8_t radio.getChannel  //devuelve el canal actual.

#define CHANNELRF 108

////////////////////////////////////////////////////////////////////////////////////////
//  bool a = radio.isPVariant();
//  devuelve verdadero si el hardware es nRF24L01 + (o compatible) y falso si no lo es.  


//  getPALevel 
//  Obtiene el nivel de PA actual. Recuerde que todos los valores representados 
//  por la enum mnemónica son dBm negativos.
