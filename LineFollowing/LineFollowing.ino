#include <SPI.h>

#define IN1 5
#define IN2 6
#define IN3 9
#define IN4 10

void setup() {
    Serial.begin(9600);
    
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    // Set SPI
    pinMode(MISO, OUTPUT);
    SPI.begin();
    SPCR |= _BV(SPE); // Enable SPI as Slave
}

volatile char command = 'S'; // Default stop command

// SPI interrupt routine
ISR(SPI_STC_vect) {
    command = SPDR; // Read received data
}

void loop() {
    Serial.print("Command: ");
    Serial.println(command);

    if (command == 'L') {
        moveLeft();
    } else if (command == 'R') {
        moveRight();
    } else if (command == 'F') {
        moveForward();
    } else {
        stopMotors();
    }

    delay(100);
}

void moveLeft() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void moveRight() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void moveForward() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void stopMotors() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}
