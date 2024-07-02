/*
  RadioLib SX126x Transmit with Interrupts Example

  This example transmits LoRa packets with one second delays
  between them. Each packet contains up to 256 bytes
  of data, in the form of:
  - Arduino String
  - null-terminated char array (C-string)
  - arbitrary binary data (byte array)

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

int SENSOR = 37;
int lastState = 1;
int currentState = 1;

// Initialize the OLED display using Arduino Wire:
SSD1306Wire display(0x3c, SDA_OLED, SCL_OLED);  // ADDRESS, SDA, SCL  -  SDA_OLED and SCL_OLED are the pins from pins_arduino.h where the Heltec connects the OLED display

// SX1262 has the following connections:
// NSS pin:   10
// DIO1 pin:  2
// NRST pin:  3
// BUSY pin:  9
// SX1262 radio = new Module(10, 2, 3, 9);
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

// save transmission state between loops
int transmissionState = RADIOLIB_ERR_NONE;

void setup() {
  Serial.begin(115200);
  while(!Serial){
    delay(100);
  }
  pinMode(SENSOR, INPUT);

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
  display.drawString(0, 10, "SENDER");
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 40, "Waiting ...");
  display.display();
  delay(10);


  // initialize SX1262 with default settings
  Serial.print(F("[SX1262] Initializing ... "));
  int state = radio.begin(915.0);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // Configurar a potência de transmissão para o máximo permitido pelo módulo
  radio.setOutputPower(22); // Máximo para SX1262 é 22 dBm >> Default = 10
  // Configurar a modulação e a taxa de dados para otimizar o alcance
  radio.setSpreadingFactor(12); // SF12 para alcance máximo
  radio.setBandwidth(125.0); // 125 kHz é uma escolha comum
  radio.setCodingRate(5); // CR 4/5 para um bom equilíbrio entre alcance e robustez

  // set the function that will be called
  // when packet transmission is finished
  radio.setPacketSentAction(setFlag);

  // start transmitting the first packet
  Serial.print(F("[SX1262] Sending first packet ... "));

  // you can transmit C-string or Arduino string up to
  // 256 characters long
  transmissionState = radio.startTransmit("Hello World!");

  // you can also transmit byte array up to 256 bytes long
  /*
    byte byteArr[] = {0x01, 0x23, 0x45, 0x67,
                      0x89, 0xAB, 0xCD, 0xEF};
    state = radio.startTransmit(byteArr, 8);
  */
}

// flag to indicate that a packet was sent
volatile bool transmittedFlag = false;

// this function is called when a complete packet
// is transmitted by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
  ICACHE_RAM_ATTR
#endif
void setFlag(void) {
  // we sent a packet, set the flag
  transmittedFlag = true;
}

// counter to keep track of transmitted packets
int count = 0;

void loop() {

  currentState = digitalRead(SENSOR);

  if (lastState == 0 && currentState == 1){

    // check if the previous transmission finished
    if(transmittedFlag) {
      // reset flag
      transmittedFlag = false;

      if (transmissionState == RADIOLIB_ERR_NONE) {
        // packet was successfully sent
        Serial.println(F("transmission finished!"));

        // NOTE: when using interrupt-driven transmit method,
        //       it is not possible to automatically measure
        //       transmission data rate using getDataRate()

      } else {
        Serial.print(F("failed, code "));
        Serial.println(transmissionState);

      }

      // clean up after transmission is finished
      // this will ensure transmitter is disabled,
      // RF switch is powered down etc.
      radio.finishTransmit();

      // WAN: Tempo entre uma transissao e outra
      delay(100);

      // send another one
      Serial.print(F("[SX1262] Sending another packet ... "));

      // you can transmit C-string or Arduino string up to
      // 256 characters long
      String str = "Hello World! #" + String(count++);
      transmissionState = radio.startTransmit(str);

      // clear the display
      display.clear();
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      display.setFont(ArialMT_Plain_16);
      display.drawString(0, 10, "SENDER");
      display.setFont(ArialMT_Plain_16);
      display.drawString(0, 40, "Count: " + String(count));
      display.display();
      delay(10);



      display.setFont(ArialMT_Plain_16);
      display.setTextAlignment(TEXT_ALIGN_RIGHT);
      display.drawString(128, 40, String(count));
      // write the buffer to the display
      display.display();
  
      delay(10);


      // you can also transmit byte array up to 256 bytes long
      /*
        byte byteArr[] = {0x01, 0x23, 0x45, 0x67,
                          0x89, 0xAB, 0xCD, 0xEF};
        transmissionState = radio.startTransmit(byteArr, 8);
      */
    }
  }
  lastState = currentState;
  delay(100);
}
