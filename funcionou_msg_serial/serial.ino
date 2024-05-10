#include <SPI.h>
#include <LoRa.h>



int counter = 0;



void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender - V3");

  Serial.println(">>>>>>>>>>>>>>>>>>> INICIO");
  
  Serial.println(">>>>>>>>>>>>>>>>>>> FIM");

} 
void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  counter = counter + 1;

  delay(2000);
}
