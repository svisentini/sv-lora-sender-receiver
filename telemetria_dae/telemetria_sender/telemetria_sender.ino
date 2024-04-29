#include "heltec.h"
#define BAND 433E6

int SENSOR = 52;
const int ACTIVE = 0;
unsigned long contador;

void setup () {
  Heltec.begin(true, true, true, true, BAND);

  pinMode(SENSOR, INPUT);
  contador = 0UL;

  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0, 0,  "Sender Iniciado!");
  Heltec.display->drawString(0, 10, "Esperando...");
  Heltec.display->display();
  delay(1000);

}

void loop () {

  if (digitalRead(SENSOR) == ACTIVE){
    contador++;
    LoRa.beginPacket();
    LoRa.print('1');
    LoRa.endPacket();

    Heltec.display->init();
    Heltec.display->flipScreenVertically();
    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->drawString(0, 0,  "Telemetria - SENDER");
    Heltec.display->drawString(0, 10, "Enviados: " + String(contador) );
    Heltec.display->display();
    delay(1000);

  }


}
