#include "heltec.h"
#include "PushButton.h"

#define BAND 915E6
#define pinBotAlerta 13
#define pinLed 21

PushButton botAlerta(pinBotAlerta);
bool aguardando = false;

void setup () {
  Heltec.begin(false, true, true, true, BAND);
  Serial.begin(9600);

  pinMode(pinLed, OUTPUT);
  botAlerta.button_loop();

  LoRa.onReceive(recebido);
}

void loop () {
  botAlerta.button_loop();

  if (botAlerta.pressed()) {
    Serial.print("Alerta enviado!");

    LoRa.beginPacket();
    LoRa.print("galpao");
    LoRa.endPacket();

    aguardando = true;
    LoRa.receive();
  }

  if(aguardando){
    digitalWrite(pinLed, bitRead(millis(), 8));
  }
  
}

void recebido(int packetSize) {
  aguardando = false;
  digitalWrite(pinLed, LOW);
  LoRa.sleep();
}
