char data; // Variable to store received data

// Define motor control pins
#define LEFT_MOTOR_FORWARD  7
#define LEFT_MOTOR_BACKWARD 6
#define RIGHT_MOTOR_FORWARD 3
#define RIGHT_MOTOR_BACKWARD 5

void setup() {
  Serial.begin(9600); // Start serial communication

  // Set motor pins as output
  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    data = Serial.read(); // Read received data
    Serial.println(data); // Optional: Print received data

    // Motor control logic
    switch (data) {
      case 'F': // Move Forward
        digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
        digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
        digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
        digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
        break;

      case 'B': // Move Backward
        digitalWrite(LEFT_MOTOR_FORWARD, LOW);
        digitalWrite(LEFT_MOTOR_BACKWARD, HIGH);
        digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
        digitalWrite(RIGHT_MOTOR_BACKWARD, HIGH);
        break;

      case 'R': // Turn Right
        digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
        digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
        digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
        digitalWrite(RIGHT_MOTOR_BACKWARD, HIGH);
        break;

      case 'L': // Turn Left
        digitalWrite(LEFT_MOTOR_FORWARD, LOW);
        digitalWrite(LEFT_MOTOR_BACKWARD, HIGH);
        digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
        digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
        break;

      case 'S': // Stop
        digitalWrite(LEFT_MOTOR_FORWARD, LOW);
        digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
        digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
        digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
        break;
    }
  }
}
