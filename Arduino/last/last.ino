#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Servo steering;

#define LEFT_MOTOR_FORWARD   7
#define LEFT_MOTOR_BACKWARD  6
#define RIGHT_MOTOR_FORWARD  12
#define RIGHT_MOTOR_BACKWARD 8

#define LEFT_MOTOR_PWM   5
#define RIGHT_MOTOR_PWM  3
#define SERVO_PIN 11
const int DEFAULT_SPEED = 90; // Default speed when using G commands

// OLED configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String command = ""; // Buffer for incoming commands
bool commandComplete = false;

void setup() {
  Serial.begin(9600); // Bluetooth communication

  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);

  steering.attach(SERVO_PIN);
  steering.write(90); // Center steering

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true); // Halt if OLED fails
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("BT Car Ready");
  display.display();
  delay(1000);
}

void loop() {
  readSerialCommand();
  
  if (commandComplete) {
    processCommand(command);
    displayCommand(command);
    command = "";
    commandComplete = false;
  }
}

void readSerialCommand() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      commandComplete = true;
      return;
    } else {
      command += c;
    }
    // Prevent buffer overflow
    if (command.length() > 32) {
      command = "";
      commandComplete = false;
    }
  }
}

void displayCommand(String cmd) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("CMD: ");
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.println(cmd);
  display.display();
}

void processCommand(String cmd) {
  if (cmd.length() < 1) return;

  char commandType = cmd.charAt(0);
  int value = cmd.substring(1).toInt();

  switch (commandType) {
    case 'T': // Steering
      value = constrain(value, 0, 180);
      steering.write(value);
      break;

    case 'S': // Speed control
      value = constrain(value, 0, 255);
      analogWrite(LEFT_MOTOR_PWM, value);
      analogWrite(RIGHT_MOTOR_PWM, value);
      break;

    case 'G': // Movement commands
      switch (value) {
        case 0: // Stop
          stop();
          break;
        case 1: // Forward
          moveForward(DEFAULT_SPEED);
          break;
        case 2: // Backward
          moveBackward(DEFAULT_SPEED);
          break;
      }
      break;

    // Add more commands as needed
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
