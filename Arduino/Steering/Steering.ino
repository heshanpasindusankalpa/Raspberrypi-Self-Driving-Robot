#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

char data;
int angle;

// Motor control pins
#define LEFT_MOTOR_FORWARD   7
#define LEFT_MOTOR_BACKWARD  6
#define RIGHT_MOTOR_FORWARD  12
#define RIGHT_MOTOR_BACKWARD 8

#define LEFT_MOTOR_PWM   5
#define RIGHT_MOTOR_PWM  3

#define SERVO_PIN 11
const int SPEED = 90;

Servo steering;

void setup() {
  Serial.begin(9600);

  // Setup motor pins
  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);

  // Setup servo
  steering.attach(SERVO_PIN);
  steering.write(90);

  // Setup OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true); // Don't continue if OLED fails
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Bluetooth Car Ready");
  display.display();

  Serial.println("Bluetooth Car Ready");
}

void loop() {
  if (Serial.available() > 0) {
    data = Serial.read();
    displayCommand(data);  // Show received command on OLED

    switch (data) {
      case 'F':
        digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
        digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
        digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
        digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
        analogWrite(LEFT_MOTOR_PWM, SPEED);
        analogWrite(RIGHT_MOTOR_PWM, SPEED);
        break;

      case 'B':
        digitalWrite(LEFT_MOTOR_FORWARD, LOW);
        digitalWrite(LEFT_MOTOR_BACKWARD, HIGH);
        digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
        digitalWrite(RIGHT_MOTOR_BACKWARD, HIGH);
        analogWrite(LEFT_MOTOR_PWM, SPEED);
        analogWrite(RIGHT_MOTOR_PWM, SPEED);
        break;

      case 'L':
        steering.write(70);
        break;

      case 'R':
        steering.write(135);
        break;

      case 'C':
        steering.write(90);
        break;

      case 'S':
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

void displayCommand(char command) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("Received Command:");
  display.setTextSize(2);
  display.setCursor(10, 30);
  display.print(command);
  display.display();
}
