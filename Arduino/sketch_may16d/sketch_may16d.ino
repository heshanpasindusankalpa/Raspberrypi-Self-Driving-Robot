#include <Servo.h>

Servo steering;

#define LEFT_MOTOR_FORWARD   7
#define LEFT_MOTOR_BACKWARD  6
#define RIGHT_MOTOR_FORWARD  9
#define RIGHT_MOTOR_BACKWARD 8
#define LEFT_MOTOR_PWM   5
#define RIGHT_MOTOR_PWM  3
#define SERVO_PIN 4

const int DEFAULT_SPEED = 110;

unsigned long lastSteeringTime = 0;
const unsigned long STEERING_TIMEOUT = 500; // 500ms timeout
bool steeringActive = false;

void setup() {
  Serial.begin(9600);

  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);

  steering.attach(SERVO_PIN);
  steering.write(90);
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd.length() > 0) {
      processCommand(cmd);
    }
  }

  // Auto-center steering after timeout
  if (steeringActive && (millis() - lastSteeringTime > STEERING_TIMEOUT)) {
    steering.write(90);  // Return to center
    steeringActive = false;
  }
}

void processCommand(String cmd) {
  if (cmd.length() < 1) return;

  char commandType = cmd.charAt(0);
  int value = cmd.substring(1).toInt();

  if (commandType == 'T') {
    int servoAngle = map(value, 0, 100, 70, 135);
    steering.write(servoAngle);
    lastSteeringTime = millis();  // Update last time steering was used
    steeringActive = true;
  }

  else if (commandType == 'G') {
    switch (value) {
      case 0: stop(); break;
      case 1: moveForward(DEFAULT_SPEED); break;
      case 2: moveBackward(DEFAULT_SPEED); break;
    }
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

void moveBackward(int pwm) {
  digitalWrite(LEFT_MOTOR_FORWARD, LOW);
  digitalWrite(LEFT_MOTOR_BACKWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
  digitalWrite(RIGHT_MOTOR_BACKWARD, HIGH);
  analogWrite(LEFT_MOTOR_PWM, pwm);
  analogWrite(RIGHT_MOTOR_PWM, pwm);
}

void stop() {
  digitalWrite(LEFT_MOTOR_FORWARD, LOW);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
  digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
  analogWrite(LEFT_MOTOR_PWM, 0);
  analogWrite(RIGHT_MOTOR_PWM, 0);
}
