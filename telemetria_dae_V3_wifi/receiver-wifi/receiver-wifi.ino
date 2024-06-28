#include <WiFi.h>
#include <WiFiServer.h>
#include <Wire.h>         // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"  // legacy: #include "SSD1306.h"
#include "pins_arduino.h"

const char* ssid = "DIEGO";
const char* password = "internet353";

unsigned int receiverCount = 0;

WiFiServer server(12345);

SSD1306Wire display(0x3c, SDA_OLED, SCL_OLED);  // ADDRESS, SDA, SCL  -  SDA_OLED and SCL_OLED are the pins from pins_arduino.h where the Heltec connects the OLED display

void VextON(void) {
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
}

void VextOFF(void)  //Vext default OFF
{
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, HIGH);
}

void displayReset(void) {
  // Send a reset
  pinMode(RST_OLED, OUTPUT);
  digitalWrite(RST_OLED, HIGH);
  delay(1);
  digitalWrite(RST_OLED, LOW);
  delay(1);
  digitalWrite(RST_OLED, HIGH);
  delay(1);
}

String ipToString(IPAddress ip) {
  String s = "";
  for (int i = 0; i < 4; i++) {
    s += String(ip[i]);
    if (i < 3) s += ".";
  }
  return s;
}


void setup() {
  Serial.begin(115200);

  VextON();
  displayReset();
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);


  display.clear();
  display.drawString(0, 0, "Receiver");
  display.display();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());

  server.begin();

  display.clear();
  display.drawString(0, 0, "Receiver >> OK");
  display.drawString(0, 20, "IP: " + ipToString(WiFi.localIP()));
  display.display();

}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    String valor = client.readStringUntil('\n');
    Serial.print("Valor recebido: ");
    Serial.println(valor);
    client.stop();

    receiverCount++;
    display.clear();
    display.drawString(0, 0, "Receiver >> OK");
    display.drawString(0, 20, "IP: " + ipToString(WiFi.localIP()));
    display.drawString(0, 40, "S:" + String(valor) + " - R:" +  String(receiverCount) );
    display.display();
    
  }
}
