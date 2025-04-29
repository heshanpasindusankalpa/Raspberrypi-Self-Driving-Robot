// Define motor control pins
#define ENA 9
#define IN2 8
#define IN1 7
#define ENB 6
#define IN4 4
#define IN3 2
#define t 120  // PWM speed (0-255)

void setup() {
    Serial.begin(9600);  // For monitoring in Serial Monitor

    // Initialize motor control pins
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);

    Serial.println("Motor Test Starting...");
}

void loop() {
    // Move Forward
    Serial.println("Moving Forward");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, t);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, t);
    delay(2000);

    // Turn Left
    Serial.println("Turning Left");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, t);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, t);
    delay(2000);

    // Turn Right
    Serial.println("Turning Right");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, t);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, t);
    delay(2000);

    // Stop
    Serial.println("Stopping");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    delay(2000);
}
