#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Servo steering;

// Motor control pins (unchanged)
#define LEFT_MOTOR_FORWARD   7
#define LEFT_MOTOR_BACKWARD  6
#define RIGHT_MOTOR_FORWARD  12
#define RIGHT_MOTOR_BACKWARD 8
#define LEFT_MOTOR_PWM   5
#define RIGHT_MOTOR_PWM  3
#define SERVO_PIN 11
const int DEFAULT_SPEED = 110;

// OLED configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Display history management
String commandHistory[8]; // Stores up to 8 lines (64px height / 8px per line)
int historyIndex = 0;
bool screenFull = false;

void setup() {
  Serial.begin(9600);

  // Motor/servo setup (unchanged)
  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);
  steering.attach(SERVO_PIN);
  steering.write(90);

  // OLED setup
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  addToHistory("System Ready");
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    
    if (cmd.length() > 0) {
      addToHistory("CMD: " + cmd);
      processCommand(cmd);
    }
  }
}

void addToHistory(String message) {
  // Shift history up if screen is full
  if (screenFull) {
    for (int i = 0; i < 7; i++) {
      commandHistory[i] = commandHistory[i + 1];
    }
    commandHistory[7] = message;
  } else {
    commandHistory[historyIndex++] = message;
    if (historyIndex >= 8) {
      screenFull = true;
      historyIndex = 7;
    }
  }
  
  // Update display
  display.clearDisplay();
  for (int i = 0; i < 8; i++) {
    if (commandHistory[i].length() > 0) {
      display.setCursor(0, i * 8);
      display.println(commandHistory[i]);
    }
  }
  display.display();
}

// Rest of your existing code (processCommand, motor control functions) remains unchanged
void processCommand(String cmd) {
  if (cmd.length() < 1) return;

  char commandType = cmd.charAt(0);
  int value = cmd.substring(1).toInt();

  switch (commandType) {
    case 'T':
      value = constrain(value, 0, 180);
      steering.write(value);
      break;

    case 'S':
      value = constrain(value, 0, 255);
      analogWrite(LEFT_MOTOR_PWM, value);
      analogWrite(RIGHT_MOTOR_PWM, value);
      break;

    case 'G':
      switch (value) {
        case 0: stop(); break;
        case 1: moveForward(DEFAULT_SPEED); break;
        case 2: moveBackward(DEFAULT_SPEED); break;
        default: addToHistory("Invalid G command");
      }
      break;

    default:
      addToHistory("Unknown command");
      break;
  }
}

// Motor control functions remain unchanged from original code
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