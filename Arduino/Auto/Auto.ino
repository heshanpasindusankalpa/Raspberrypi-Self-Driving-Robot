#include <Servo.h>
#include <SPI.h>

Servo steering;

#define LEFT_MOTOR_FORWARD   7
#define LEFT_MOTOR_BACKWARD  6
#define RIGHT_MOTOR_FORWARD  9
#define RIGHT_MOTOR_BACKWARD 8
#define LEFT_MOTOR_PWM   5
#define RIGHT_MOTOR_PWM  3
#define SERVO_PIN 4

const int DEFAULT_SPEED = 55;
volatile char current_cmd = 'F';

void setup() {
  // Motor setup
  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);

  steering.attach(SERVO_PIN);
  steering.write(95);  // Straight

  // SPI setup
  pinMode(MISO, OUTPUT);       // Required for SPI slave mode
  SPCR |= _BV(SPE);            // Enable SPI
  SPI.attachInterrupt();       // Attach interrupt

  Serial.begin(9600);
  Serial.println("SPI Slave Ready");
}

ISR(SPI_STC_vect) {
  current_cmd = SPDR;  // Receive byte from Pi
}

void loop() {
  // Motor Direction
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
  digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
  analogWrite(LEFT_MOTOR_PWM, DEFAULT_SPEED);
  analogWrite(RIGHT_MOTOR_PWM, DEFAULT_SPEED);

  // Set servo direction based on command
  switch (current_cmd) {
    case 'F': steering.write(95); break;
    case 'L': steering.write(70); break;
    case 'R': steering.write(135); break;
    case 'I': steering.write(110); break;
    case 'G': steering.write(80); break;
    default:  steering.write(95); break;
  }

  delay(10);
}
