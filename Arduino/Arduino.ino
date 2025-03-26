#include <SPI.h>

// Motor Control Pins
#define IN1 5
#define IN2 6
#define IN3 9
#define IN4 3

volatile char command = 0;

void setup() {
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    SPI.begin();
    SPCR |= _BV(SPE);  // Enable SPI Slave
    SPCR &= ~_BV(MSTR); // Set as Slave

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);

    SPI.attachInterrupt(); // Enable SPI interrupt
}

ISR(SPI_STC_vect) { // SPI interrupt routine
    command = SPDR; // Read received byte
}

void loop() {
    if (command == 'F') {
        // Move Forward
        Serial.print("F");
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
    } 
    else if (command == 'L') {
        // Turn Left
        Serial.print("L");
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
    } 
    else if (command == 'R') {
        // Turn Right
        Serial.print("R");
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
    } 
    else {
        // Stop
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
    }
}
