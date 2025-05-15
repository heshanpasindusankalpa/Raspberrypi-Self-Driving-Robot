#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Servo steering;

String command = "";
char startChar;
int value;

#define LEFT_MOTOR_FORWARD   7
#define LEFT_MOTOR_BACKWARD  6
#define RIGHT_MOTOR_FORWARD  12
#define RIGHT_MOTOR_BACKWARD 8

#define LEFT_MOTOR_PWM   5
#define RIGHT_MOTOR_PWM  3
#define SERVO_PIN 11
const int SPEED = 90;

#define BT Serial1  // Bluetooth on Mega

// OLED config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  BT.begin(9600);       // Bluetooth module

  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);

  steering.attach(SERVO_PIN);
  steering.write(90); // Center

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    // You can blink an LED here or halt
    while (true); // Stop if OLED not found
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Car Ready");
  display.display();
  delay(1000);
}

void loop() {
  if (BT.available()) {
    char c = BT.read();
    if (c == '\n' || c == '\r') {
      processCommand(command);
      displayCommand(command);
      command = "";
    } else {
      command += c;
    }
  }
}

void displayCommand(String cmd) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.println("Cmd:");
  display.setTextSize(2);
  display.setCursor(0, 25);
  display.println(cmd);
  display.display();
}

void processCommand(String cmd) {
  if (cmd.length() < 3) return;

  startChar = cmd.charAt(0);
  value = cmd.substring(1).toInt();

  switch (startChar) {
    case 'T': {
      value = constrain(value, 0, 99);
      int angle = map(value, 0, 99, 45, 135);
      steering.write(angle);
      break;
    }
    case 'S': {
      value = constrain(value, 0, 99);
      int speedPWM = map(value, 0, 99, 0, 255);
      moveForward(speedPWM);
      break;
    }
    case 'G': {
      if (value == 0) stop();
      else if (value == 3) moveForward(SPEED);
      else if (value == 1) moveBackward(SPEED);
      break;
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
