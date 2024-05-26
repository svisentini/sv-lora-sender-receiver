#include "heltec.h"
 
#define BAND 433E6
#define pinRele 12

char st;
unsigned long contador;

void setup () {
  Heltec.begin(true, true, true, true, BAND);
  pinMode(pinRele, OUTPUT);
  contador = 0UL;
  
  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_16);
    Heltec.display->drawString(0, 0,  "RECEIVER");
    Heltec.display->drawString(0, 20, "Telemetria");
    Heltec.display->drawString(0, 40, "Esperando ..." );
  Heltec.display->display();
  delay(1000);

  LoRa.onReceive(recebido);
  LoRa.receive();
}
 
void loop () {

  if (st == '1') {
    digitalWrite(pinRele, 1);
    delay(200);
    digitalWrite(pinRele, 0);
    
    Heltec.display->init();
    Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_16);
    Heltec.display->drawString(0, 0,  "RECEIVER");
    Heltec.display->drawString(0, 20, "Telemetria");
    Heltec.display->drawString(0, 40, "Recebidos: " + String(contador) );
    Heltec.display->display();
    delay(1000);
    
    st = '0';
  }

}

void recebido(int packetSize) {
  st = '1';
  contador++;
}
