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

int currentSteeringValue = 50;  // Midpoint
unsigned long lastServoUpdate = 0;

void setup() {
  Serial.begin(9600);

  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);

  steering.attach(SERVO_PIN);
  steering.write(95);  // Slightly right-centered
}

void loop() {
  // Read steering command if available
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd.length() > 0 && cmd.charAt(0) == 'T') {
      int value = cmd.substring(1).toInt();
      currentSteeringValue = constrain(value, 0, 100); // Save for smooth updates
    }
  }

  // Move forward always
  moveForward(DEFAULT_SPEED);

  // Update steering every 5ms
  unsigned long now = millis();
  if (now - lastServoUpdate > 5) {
    int angle = map(currentSteeringValue, 0, 100, 70, 135);
    steering.write(angle);
    lastServoUpdate = now;
  }
}

void moveForward(int pwm) {
  digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
  digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
  analogWrite(LEFT_MOTOR_PWM, pwm);
  analogWrite(RIGHT_MOTOR_PWM, pwm);
}
