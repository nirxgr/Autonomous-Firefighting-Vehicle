// Defining Flame Sensors
#define flameSensorLeft A0
#define flameSensorCenter A1
#define flameSensorRight A2

// Motor A connections
int enA = 6;
int in1 = 8;
int in2 = 7;

// Motor B connections
int enB = 5;
int in3 = 4;
int in4 = 3;

#include <Servo.h>
Servo servoMotor; // Creating a Servo object

// Defining relay pin
#define RELAY_PIN 11

void setup()
{
  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("System Starting...");

  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Turn off motors - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  // Set flame sensor pins as input
  pinMode(flameSensorLeft, INPUT);
  pinMode(flameSensorCenter, INPUT);
  pinMode(flameSensorRight, INPUT);

  // Attach the servo to the pin
  servoMotor.attach(12);
  servoMotor.write(90);

  // Attach relay pin to output
  pinMode(RELAY_PIN, OUTPUT);    // Set relay pin as output
  digitalWrite(RELAY_PIN, HIGH); // Ensure the relay is OFF initially
}

void loop()
{

  // Read flame sensor intensities
  int flameLeft = analogRead(flameSensorLeft);
  int flameCenter = analogRead(flameSensorCenter);
  int flameRight = analogRead(flameSensorRight);

  // For deugging
  Serial.print("Flame Sensor States: ");
  Serial.print("Left: ");
  Serial.print(flameLeft);
  Serial.print(", Center: ");
  Serial.print(flameCenter);
  Serial.print(", Right: ");
  Serial.println(flameRight);

  // conditions
  if (flameLeft < 100 || flameCenter < 100 || flameRight < 100)
  {
    Serial.println("All less than boundary.");
    stopMotors();
    startRelay();
    servo();
  }
  else if (flameCenter >= 150 && flameCenter < 100 && flameCenter < flameLeft && flameCenter < flameRight)
  {
    Serial.println("Flame detected in Center. Moving Forward.");
    moveForward();
    startRelay();
    servo();
  }
  else if (flameLeft >= 150 && flameLeft < 100 && flameLeft < flameCenter && flameLeft < flameRight)
  {
    Serial.println("Flame detected on Left. Turning Left.");
    turnLeft();
    startRelay();
    servo();
  }
  else if (flameRight >= 150 && flameRight < 100 && flameRight < flameCenter && flameRight < flameLeft)
  {
    Serial.println("Flame detected on Right. Turning Right.");
    turnRight();
    startRelay();
    servo();
  }

  else
  {
    Serial.println("No Flame detected. Stopping Motors.");
    stopMotors();
    stopRelay();
    stopServo();
  }

  // Delay for stability
  delay(100);
}

void moveForward()
{
  analogWrite(enA, 50);
  analogWrite(enB, 50);
  // Turn on motor A & B
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnLeft()
{
  analogWrite(enA, 60);
  analogWrite(enB, 0);
  // Turn on motor A
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void turnRight()
{
  analogWrite(enA, 0);
  analogWrite(enB, 60);
  // Turn on motor B
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void stopMotors()
{
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  // Turn on motor A
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

// servo motor function
void servo()
{
  // Sweep the servo clockwise from 40 to 170 degrees
  for (int angle = 40; angle <= 170; angle++)
  {
    servoMotor.write(angle); // Move servo to the current angle
    delay(8);                // Wait 8ms for smooth motion
  }
  for (int angle = 170; angle >= 40; angle--)
  {
    servoMotor.write(angle); // Move servo to the current angle
    delay(8);                // Wait 8ms for smooth motion
  }
  // Return to the initial position (90 degrees) after the sweep
  servoMotor.write(90);
}

// to make servo motor at 90
void stopServo()
{
  servoMotor.write(90); // Stop the servo
}

// starting relay
void startRelay()
{
  digitalWrite(RELAY_PIN, LOW); // Turn ON the relay
  delay(1000);
}

// stopping relay
void stopRelay()
{
  digitalWrite(RELAY_PIN, HIGH); // Turn OFF the relay
}
