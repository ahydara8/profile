/*
  Arduino Bluetooth/Serial Monitor Controlled Car

  This project controls a 4-wheel Arduino car using both
  a Bluetooth mobile app (HC-05) and the Arduino IDE Serial Monitor.
  
  The system uses:
  - L293D Motor Driver Shield
  - 4 DC Motors
  - HC-05 Bluetooth Module
  - Ultrasonic Sensor
  - Servo Motor

  Features:
  - Forward, backward, left, and right movement
  - Bluetooth and Serial Monitor control
  - Automatic obstacle detection and avoidance

Created by Balagie Hydara

You can watch the video here: https://youtu.be/1xQAdOWvVt4
*/


#include <AFMotor.h>
#include <NewPing.h>
#include <Servo.h>

AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);
Servo myservo;

int valSpeed = 255;

// Ultrasonic
#define TRIG_PIN A0
#define ECHO_PIN A1
#define MAX_DISTANCE 200
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

int stopDistance = 20;

// movement state
char currentMode = 'S';

void setup() {
  Serial.begin(9600);

  myservo.attach(10);
  myservo.write(115);
  delay(500);

  SetSpeed(valSpeed);
  stopMotors();

  Serial.println("F=forward auto avoid, B=back, L=left, R=right, S=stop");
}

void loop() {
  // read command from Serial Monitor
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    if (cmd == 'f' || cmd == 'F') {
      currentMode = 'F';
      Serial.println("Forward with auto obstacle avoidance");
    }
    else if (cmd == 'b' || cmd == 'B') {
      currentMode = 'B';
      moveBackward();
      Serial.println("Backward");
    }
    else if (cmd == 'l' || cmd == 'L') {
      currentMode = 'L';
      turnLeft();
      Serial.println("Left");
    }
    else if (cmd == 'r' || cmd == 'R') {
      currentMode = 'R';
      turnRight();
      Serial.println("Right");
    }
    else if (cmd == 's' || cmd == 'S') {
      currentMode = 'S';
      stopMotors();
      Serial.println("Stop");
    }
  }

  // automatic obstacle detection only when going forward
  if (currentMode == 'F') {
    int d = readDistance();

    if (d > stopDistance) {
      moveForward();
    } else {
      obstacleAvoid();
    }
  }
}

void SetSpeed(int val) {
  valSpeed = val;
  motor1.setSpeed(val);
  motor2.setSpeed(val);
  motor3.setSpeed(val);
  motor4.setSpeed(val);
}

void moveForward() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void moveBackward() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void turnLeft() {
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(FORWARD);
}

void turnRight() {
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
}

void stopMotors() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

int readPingOnly() {
  delay(50);
  int cm = sonar.ping_cm();
  if (cm == 0) cm = 250;
  return cm;
}

int readDistance() {
  myservo.write(115);
  delay(200);
  return readPingOnly();
}

int lookLeft() {
  myservo.write(170);
  delay(400);
  int d = readPingOnly();
  myservo.write(115);
  delay(200);
  return d;
}

int lookRight() {
  myservo.write(50);
  delay(400);
  int d = readPingOnly();
  myservo.write(115);
  delay(200);
  return d;
}

void obstacleAvoid() {
  stopMotors();
  delay(200);

  moveBackward();
  delay(400);
  stopMotors();
  delay(200);

  int rightD = lookRight();
  int leftD  = lookLeft();

  if (rightD >= leftD) {
    turnRight();
    delay(500);
  } else {
    turnLeft();
    delay(500);
  }

  stopMotors();
  delay(200);
}