#include "heltec.h"
#include "PushButton.h"

#define BAND 915E6
#define pinRele 25

#define pinBotOk 13

PushButton botOk(pinBotOk);
char st;

void setup () {
  Heltec.begin(true, true, true, true, BAND);

  Serial.begin(9600);
  pinMode(pinRele, OUTPUT);

  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0, 0,  "LoRa Iniciado!");
  Heltec.display->drawString(0, 10, "Esperando...");
  Heltec.display->display();
  delay(1000);

  LoRa.onReceive(recebido);
  LoRa.receive();
}

void loop () {
  botOk.button_loop();

  if (st == '1') {
    Heltec.display->clear();
    Heltec.display->drawString(0, 0, "RECEBIDO!!!");
    Heltec.display->drawString(0, 10, "Alerta recebido do galpão");
    Heltec.display->display();

    digitalWrite(pinRele, bitRead(millis(), 10));

    if (botOk.pressed()) {
      digitalWrite(pinRele, LOW);

      st = 0;

      LoRa.beginPacket();
      LoRa.print("ok");
      LoRa.endPacket();

      Heltec.display->clear();
      Heltec.display->drawString(0, 0,  "LoRa Iniciado!");
      Heltec.display->drawString(0, 10, "Esperando...");
      Heltec.display->display();

      delay(1000);
      LoRa.receive();
    }

  }

}

void recebido(int packetSize) {
  st = '1';
}


//  int packetSize = LoRa.parsePacket();
//
//  if (packetSize) {
//    Serial.print("Pacote recebido '");
//
//    while (LoRa.available ()) {
//      st = (char)LoRa.read ();
//      Serial.println(st);
//    }
//
//    if (st == '1')
//      digitalWrite(25, HIGH);
//    delay(1000);
//    digitalWrite(25, LOW);
//
//    Serial.print("'com RSSI");
//    Serial.println(LoRa.packetRssi());
//  }
