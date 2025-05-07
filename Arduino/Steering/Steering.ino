#include <Servo.h>

char data; // Variable to store received data

// Motor control pins (PWM capable)
#define LEFT_MOTOR_FORWARD   7
#define LEFT_MOTOR_BACKWARD  6
#define RIGHT_MOTOR_FORWARD  12
#define RIGHT_MOTOR_BACKWARD 8

#define LEFT_MOTOR_PWM   5   // PWM pin for left motor
#define RIGHT_MOTOR_PWM 3   // PWM pin for right motor

#define SERVO_PIN 11         // Servo pin
const int SPEED = 90;       // Motor speed (0-255)

Servo steering;

void setup() {
  Serial.begin(9600); // Start serial communication

  // Set motor direction pins as output
  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);

  // Set motor PWM pins as output
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);



  // Attach servo
  steering.attach(SERVO_PIN);
  steering.write(95); // Start at center

  Serial.println("Bluetooth Car Ready");
}

void loop() {
  if (Serial.available() > 0) {
    data = Serial.read();
    Serial.println(data); // Optional: Debug

    switch (data) {
      case 'F': // Move Forward
    
        digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
        digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
        digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
        digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
        analogWrite(LEFT_MOTOR_PWM, SPEED);  // Always write PWM first
        analogWrite(RIGHT_MOTOR_PWM, SPEED);

        break;

      case 'B': // Move Backward
        digitalWrite(LEFT_MOTOR_FORWARD, LOW);
        digitalWrite(LEFT_MOTOR_BACKWARD, HIGH);
        digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
        digitalWrite(RIGHT_MOTOR_BACKWARD, HIGH);
        analogWrite(LEFT_MOTOR_PWM, SPEED);
        analogWrite(RIGHT_MOTOR_PWM, SPEED);
        break;

      case 'L': // Turn wheels Left
        steering.write(55); // Adjust if needed
        break;

      case 'R': // Turn wheels Right
        steering.write(125); // Adjust if needed
        break;

      case 'C': // Center steering
        steering.write(90);
        break;

      case 'S': // Stop
        digitalWrite(LEFT_MOTOR_FORWARD, LOW);
        digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
        digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
        digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
        analogWrite(LEFT_MOTOR_PWM, 0);
        analogWrite(RIGHT_MOTOR_PWM, 0);
        break;
    }
  }
}
