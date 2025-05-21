#include <Servo.h>

// Servo for steering
Servo steering;

// Motor control pins
#define LEFT_MOTOR_FORWARD   7
#define LEFT_MOTOR_BACKWARD  6
#define RIGHT_MOTOR_FORWARD  9
#define RIGHT_MOTOR_BACKWARD 8
#define LEFT_MOTOR_PWM   5
#define RIGHT_MOTOR_PWM  3
#define SERVO_PIN 4

// Default speed
const int DEFAULT_SPEED = 55;

void setup() {
  Serial.begin(9600);

  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);

  steering.attach(SERVO_PIN);
  steering.write(95); // Default to forward
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read(); // Read one character

    switch (cmd) {
      case 'F':  // Forward
        steering.write(95);
        break;
      case 'L':  // Left
        steering.write(70);
        break;
      case 'R':  // Right
        steering.write(135);
        break;
      case 'I':  // Forward-right
        steering.write(110);
        break;
      case 'G':  // Forward-left
        steering.write(80);
        break;
      default:
        // Ignore unknown characters
        break;
    }
  }

  moveForward(DEFAULT_SPEED); // Always move forward
}

void moveForward(int pwm) {
  digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
  digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
  analogWrite(LEFT_MOTOR_PWM, pwm);
  analogWrite(RIGHT_MOTOR_PWM, pwm);
}

