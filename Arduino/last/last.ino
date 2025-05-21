#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Servo for steering
Servo steering;

// Motor control pins
#define LEFT_MOTOR_FORWARD   7
#define LEFT_MOTOR_BACKWARD  6
#define RIGHT_MOTOR_FORWARD  12
#define RIGHT_MOTOR_BACKWARD 8
#define LEFT_MOTOR_PWM   5
#define RIGHT_MOTOR_PWM  3

// Servo pin
#define SERVO_PIN 11

// Default speed for G1 and G2 commands
const int DEFAULT_SPEED = 90;

// OLED configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Serial command buffer
String command = "";
bool commandComplete = false;

void setup() {
  Serial.begin(9600); // Initialize Bluetooth serial communication

  // Set motor pins as outputs
  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);

  // Attach and center steering servo
  steering.attach(SERVO_PIN);
  steering.write(90);

  // Initialize OLED display
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
  static bool shown = false;
  while (Serial.available()) {
    if (!shown) {
      displayCommand("Receiving...");
      shown = true;
    }

    char c = Serial.read();

    // Only allow printable characters or newline
    if ((c >= 32 && c <= 126) || c == '\n' || c == '\r') {
      if (c == '\n' || c == '\r') {
        commandComplete = true;
        shown = false;
        return;
      } else {
        command += c;
      }

      // If command is too long, reset it
      if (command.length() > 32) {
        command = "";
        commandComplete = false;
        shown = false;
        displayCommand1("Error: Overflow");
        delay(1000); // Allow message to be readable
        displayCommand("BT Car Ready");
        return;
      }
    } else {
      // Ignore unprintable garbage characters
      continue;
    }
  }
}


void displayCommand(String cmd) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(cmd);
  display.display();
}
void displayCommand1(String cmd) {
  display.clearDisplay();
 
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.println(cmd);
  display.display();
}
void processCommand(String cmd) {
  if (cmd.length() < 1) return;

  char commandType = cmd.charAt(0);
  int value = cmd.substring(1).toInt();

  switch (commandType) {
    case 'T': // Steering control
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
        default:
          displayCommand("G? Invalid");
          break;
      }
      break;

    default: // Unknown command
      displayCommand("Invalid CMD");
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

