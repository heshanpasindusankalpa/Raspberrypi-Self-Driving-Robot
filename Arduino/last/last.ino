#include <Servo.h>

Servo steering;

String command = "";  // To store full command like "T35" or "S90"
char startChar;       // First character in command
int value;            // Number part like 35

// Motor pins
#define LEFT_MOTOR_FORWARD   7
#define LEFT_MOTOR_BACKWARD  6
#define RIGHT_MOTOR_FORWARD  12
#define RIGHT_MOTOR_BACKWARD 8

#define LEFT_MOTOR_PWM   5
#define RIGHT_MOTOR_PWM  3

#define SERVO_PIN 11
const int SPEED = 90;

#define BT Serial1  // For Arduino Mega (use Serial1)

void setup() {
  Serial.begin(9600);   // Serial Monitor
  BT.begin(9600);       // Bluetooth module on Serial1

  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);

  steering.attach(SERVO_PIN);
  steering.write(90); // Center

  Serial.println("Bluetooth Car Ready (Mega)");
}

void loop() {
  if (BT.available()) {
    char c = BT.read();
    Serial.print(c);
    if (c == '\n' || c == '\r') {
      processCommand(command);
      command = "";
    } else {
      command += c;
    }
  }
}

void processCommand(String cmd) {
  if (cmd.length() < 3) return;

  startChar = cmd.charAt(0);
  value = cmd.substring(1).toInt(); // Convert "35" to 35

  switch (startChar) {
    case 'T':  // Steering
      value = constrain(value, 0, 99);  // Optional safety
      int angle = map(value, 0, 99, 45, 135);  // Adjust as needed
      steering.write(angle);
      Serial.print("Steering angle: ");
      Serial.println(angle);
      break;

    case 'S':  // Throttle
      value = constrain(value, 0, 99);
      int speedPWM = map(value, 0, 99, 0, 255);
      moveForward(speedPWM);
      break;

    case 'G':  // Gear
      if (value == 0) stop();
      else if (value == 3) moveForward(SPEED);
      else if (value == 1) moveBackward(SPEED);
      break;
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
