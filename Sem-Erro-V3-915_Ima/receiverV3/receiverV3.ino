/*
   RadioLib SX126x Receive with Interrupts Example

   This example listens for LoRa transmissions and tries to
   receive them. Once a packet is received, an interrupt is
   triggered. To successfully receive data, the following
   settings have to be the same on both transmitter
   and receiver:
    - carrier frequency
    - bandwidth
    - spreading factor
    - coding rate
    - sync word

   Other modules from SX126x family can also be used.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx126x---lora-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>
#include <Wire.h>         // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"  // legacy: #include "SSD1306.h"
//  This allows predefined pins to be used.  Compatible with Heltec and Espressif libraries.
#include <pins_arduino.h>

#define pinRele 37

int count = 0;
int countErro = 0;
// Initialize the OLED display using Arduino Wire:
SSD1306Wire display(0x3c, SDA_OLED, SCL_OLED);  // ADDRESS, SDA, SCL  -  SDA_OLED and SCL_OLED are the pins from pins_arduino.h where the Heltec connects the OLED display

// SX1262 has the following connections:
// NSS pin:   10
// DIO1 pin:  2
// NRST pin:  3
// BUSY pin:  9
// SX1262 radio = new Module(10, 2, 3, 9);
// This works with the pins defined in pins_arduino.h.
SX1262 radio = new Module(SS, DIO0, RST_LoRa, BUSY_LoRa);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//SX1262 radio = RadioShield.ModuleA;

// or using CubeCell
//SX1262 radio = new Module(RADIOLIB_BUILTIN_MODULE);
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

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(100);
  }

  // initialize SX1262 with default settings
  Serial.print(F("[SX1262] Initializing ... "));
  int state = radio.begin(915.0);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true)
      ;
  }
  // Configurar a potência de transmissão para o máximo permitido pelo módulo
  radio.setOutputPower(22); // Máximo para SX1262 é 22 dBm
  // Configurar a modulação e a taxa de dados para otimizar o alcance
  radio.setSpreadingFactor(12); // SF12 para alcance máximo
  radio.setBandwidth(125.0); // 125 kHz é uma escolha comum
  radio.setCodingRate(5); // CR 4/5 para um bom equilíbrio entre alcance e robustez

  pinMode(pinRele, OUTPUT);
  // set the function that will be called
  // when new packet is received
  radio.setPacketReceivedAction(setFlag);

  // start listening for LoRa packets
  Serial.print(F("[SX1262] Starting to listen ... "));
  state = radio.startReceive();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true)
      ;
  }

    // This turns on and resets the OLED on the Heltec boards
  VextON();
  displayReset();

  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
      display.clear();
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      display.setFont(ArialMT_Plain_16);
      display.drawString(0, 10, "Receiver");
      display.setFont(ArialMT_Plain_16);
      display.drawString(0, 40, "Waiting ...");
      display.display();
      delay(10);
  // if needed, 'listen' mode can be disabled by calling
  // any of the following methods:
  //
  // radio.standby()
  // radio.sleep()
  // radio.transmit();
  // radio.receive();
  // radio.scanChannel();
}

// flag to indicate that a packet was received
volatile bool receivedFlag = false;

// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
ICACHE_RAM_ATTR
#endif
void setFlag(void) {
  // we got a packet, set the flag
  receivedFlag = true;
}

void loop() {
  // check if the flag is set
  if (receivedFlag) {
    // reset flag
    receivedFlag = false;

    // you can read received data as an Arduino String
    String str;
    int state = radio.readData(str);

    // you can also read received data as byte array
    /*
      byte byteArr[8];
      int numBytes = radio.getPacketLength();
      int state = radio.readData(byteArr, numBytes);
    */

    if (state == RADIOLIB_ERR_NONE) {
      // packet was successfully received
      Serial.println(F("[SX1262] Received packet!"));

      // print data of the packet
      Serial.print(F("[SX1262] Data:\t\t"));
      Serial.println(str);

      // print RSSI (Received Signal Strength Indicator)
      Serial.print(F("[SX1262] RSSI:\t\t"));
      Serial.print(radio.getRSSI());
      Serial.println(F(" dBm"));

      // print SNR (Signal-to-Noise Ratio)
      Serial.print(F("[SX1262] SNR:\t\t"));
      Serial.print(radio.getSNR());
      Serial.println(F(" dB"));

      // print frequency error
      Serial.print(F("[SX1262] Frequency error:\t"));
      Serial.print(radio.getFrequencyError());
      Serial.println(F(" Hz"));

      count++;

      digitalWrite(pinRele, 1);
      delay(200);
      digitalWrite(pinRele, 0);
      delay(100);

    } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
      // packet was received, but is malformed
      Serial.println(F("CRC error!"));
      countErro++;

    } else {
      // some other error occurred
      Serial.print(F("failed, code "));
      Serial.println(state);
      countErro++;
    }


      display.clear();
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      display.setFont(ArialMT_Plain_16);
      display.drawString(0, 10, "Rec:" + String(count) + " Erro: " + String(countErro));
      display.setFont(ArialMT_Plain_10);
      display.drawString(0, 30, str);
      display.setFont(ArialMT_Plain_16);
      display.drawString(0, 45, "RSSI: " + String(radio.getRSSI()) + " dBm");
      display.display();
      delay(10);

  }
}
