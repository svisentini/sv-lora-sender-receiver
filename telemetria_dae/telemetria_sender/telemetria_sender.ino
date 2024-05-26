#include "heltec.h"
#define BAND 433E6

int SENSOR = 12;
const int ACTIVE = 1;
int lastState = 1;
int currentState = 1;
unsigned long contador;

void setup () {
  Heltec.begin(true, true, true, true, BAND);

  pinMode(SENSOR, INPUT);
  contador = 0UL;

  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_16);
    Heltec.display->drawString(0, 0,  "SENDER");
    Heltec.display->drawString(0, 20, "Telemetria");
    Heltec.display->drawString(0, 40, "Esperando ..." );
  Heltec.display->display();
  delay(1000);

}

void loop () {
  currentState = digitalRead(SENSOR);

  if (lastState == 0 && currentState == 1){
    contador++;
    LoRa.beginPacket();
    LoRa.print(contador);
    LoRa.endPacket();

    Heltec.display->init();
    Heltec.display->flipScreenVertically();
    Heltec.display->setFont(ArialMT_Plain_16);
    Heltec.display->drawString(0, 0,  "SENDER");
    Heltec.display->drawString(0, 20, "Telemetria");
    Heltec.display->drawString(0, 40, "Contador: " + String(contador) );
    Heltec.display->display();
    delay(100);

  }
  lastState = currentState;


}
