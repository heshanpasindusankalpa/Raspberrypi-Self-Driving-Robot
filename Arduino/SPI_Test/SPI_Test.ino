#include <SPI.h>

volatile byte dataToSend = 'A';

void setup() {
  pinMode(MISO, OUTPUT);       // MISO is used to send data to master
  pinMode(53, OUTPUT);         // SS must be output to keep SPI active
  digitalWrite(53, LOW);       // Stay in SPI slave mode

  SPCR |= _BV(SPE);            // Enable SPI in slave mode
  SPI.attachInterrupt();       // Enable SPI interrupt

  Serial.begin(9600);
  Serial.println("Arduino Mega SPI Slave Ready");
}

ISR(SPI_STC_vect) {
  SPDR = dataToSend;           // Send 'A' on SPI
}

void loop() {
  // Optional: Allow user to change data via serial
  if (Serial.available()) {
    dataToSend = Serial.read();
    Serial.print("Now sending: ");
    Serial.println((char)dataToSend);
  }
}
