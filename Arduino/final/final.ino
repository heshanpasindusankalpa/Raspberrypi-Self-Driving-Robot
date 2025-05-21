#include <SPI.h>
#include <Servo.h>

// Servo and motor pins
Servo steering;

#define LEFT_MOTOR_FORWARD   7
#define LEFT_MOTOR_BACKWARD  6
#define RIGHT_MOTOR_FORWARD  9
#define RIGHT_MOTOR_BACKWARD 8
#define LEFT_MOTOR_PWM   5
#define RIGHT_MOTOR_PWM  3
#define SERVO_PIN 4

const int DEFAULT_SPEED = 55;
volatile char current_cmd = 'F';  // Default command

void setup() {
  // Motor setup
  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);
  pinMode(53, OUTPUT);

  steering.attach(SERVO_PIN);
  steering.write(95);  // Straight

  // Move forward
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
  digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
  analogWrite(LEFT_MOTOR_PWM, DEFAULT_SPEED);
  analogWrite(RIGHT_MOTOR_PWM, DEFAULT_SPEED);

  // SPI setup
  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);        // Enable SPI in slave mode
  SPI.attachInterrupt();   // Enable SPI interrupt

  Serial.begin(9600);
  Serial.println("Arduino Ready");
}

ISR(SPI_STC_vect) {
  // Simply load the current command into the data register
  SPDR = current_cmd;
}

void loop() {
  // Read command from Bluetooth or Serial
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 'F' || cmd == 'L' || cmd == 'R' || cmd == 'I' || cmd == 'G') {
      current_cmd = cmd;
      Serial.print("New command: ");
      Serial.println(current_cmd);
    }
  }

  // Set servo angle
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
