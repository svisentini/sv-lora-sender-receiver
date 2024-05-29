#include "heltec.h"
 
#define BAND 915E6
#define pinRele 12

char st;
unsigned long contador;

void setup () {
  Heltec.begin(true, true, true, true, BAND);

  Heltec.display->clear();
  delay(100);
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(250E3);
  LoRa.setCodingRate4(5);
  LoRa.setPreambleLength(6);
  LoRa.setSyncWord(0x12);
  LoRa.crc();
  delay(100);


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
    Heltec.display->drawString(0, 20, "RSSI: " + String(LoRa.packetRssi(), DEC) );
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
