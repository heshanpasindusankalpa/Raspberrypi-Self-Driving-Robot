#include <SPI.h>

// Define motor control pins
#define ENA 9
#define IN2 8
#define IN1 7
#define ENB 6
#define IN4 4
#define IN3 2
#define t 80

// SPI communication variable
volatile byte command = 0;
volatile bool newData = false;

void setup() {
    Serial.begin(9600);  // Initialize serial communication
    
    // Initialize SPI in slave mode
    pinMode(MISO, OUTPUT);
    SPCR |= _BV(SPE);  // Enable SPI in Slave Mode
    SPI.attachInterrupt();  // Enable SPI interrupt
    
    // Initialize motor control pins
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    
    Serial.println("Arduino SPI receiver ready");
}

// SPI interrupt routine
ISR(SPI_STC_vect) {
    command = SPDR;  // Get received byte
    newData = true;  // Flag that new data is available
    SPDR = command;  // Echo back received byte for debugging
}

void loop() {
    if (newData) {
        Serial.print("Command: ");
        Serial.println(command);

        // Control motors based on received command
        if (command == 'F') {
            Serial.println("Moving Forward");
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            analogWrite(ENA, t); 
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
            analogWrite(ENB, t); 
        } else if (command == 'L') {
            Serial.println("Turning Left");
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            analogWrite(ENA, t); 
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
            analogWrite(ENB, t); 
        } else if (command == 'R') {
            Serial.println("Turning Right");
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            analogWrite(ENA, t); 
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);
            analogWrite(ENB, t); 
        } else {
            Serial.println("Stopping");
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, LOW);
        }
        
        // Add delay to allow for smoother motion response
        delay(2000);  // Adjust delay time to your needs (200 milliseconds here)

        newData = false;  // Reset new data flag
    }
}
