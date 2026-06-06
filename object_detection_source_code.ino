/*
Name: Smart Distance Indicator with LCD.

This project uses an ultrasonic sensor and Arduino to measure distance and display the value on an LCD screen.
Based on the measured distance, red, yellow, and green LEDs turn ON to indicate close, medium, or safe distance ranges. 
A push button is used to turn the system ON/OFF, and a potentiometer controls LED brightness.

This is Part 1. No Serial Plotter

Created by Balagie Hydara
Date: 5/10/2026

You can watch the video here: https://youtu.be/6sOthn_fJXs

*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define POT_PIN A0
#define BUTTON_PIN A1
#define TRIG_PIN A2
#define ECHO_PIN A3

#define RED_LED 8
#define YELLOW_LED 9
#define GREEN_LED 10

LiquidCrystal_I2C lcd(0x27, 16, 2);

bool systemOn = true;
bool lastButtonState = HIGH;

void setup() {
  Serial.begin(9600);

  pinMode(POT_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Parking System");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(1500);
  lcd.clear();
}

void loop() {
  bool buttonState = digitalRead(BUTTON_PIN);

  // Push button turns system ON/OFF
  if (lastButtonState == HIGH && buttonState == LOW) {
    systemOn = !systemOn;
    delay(250);
  }

  lastButtonState = buttonState;

  if (systemOn == false) {
    allLEDsOff();

    lcd.setCursor(0, 0);
    lcd.print("System: OFF     ");
    lcd.setCursor(0, 1);
    lcd.print("Press button    ");

    Serial.println("System OFF");
    delay(300);
    return;
  }

  int potValue = analogRead(POT_PIN);
  int brightness = map(potValue, 0, 1023, 50, 255);

  long distance = getDistanceInches();
  distance = constrain(distance, 2, 22);

  allLEDsOff();

  String statusText = "";

  if (distance >= 2 && distance <= 8) {
    analogWrite(RED_LED, brightness);
    statusText = "RED CLOSE";
  }
  else if (distance >= 9 && distance <= 15) {
    analogWrite(YELLOW_LED, brightness);
    statusText = "YELLOW MID";
  }
  else if (distance >= 16 && distance <= 22) {
    analogWrite(GREEN_LED, brightness);
    statusText = "GREEN SAFE";
  }

  lcd.setCursor(0, 0);
  lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print(" in  ");

  lcd.setCursor(0, 1);
  lcd.print(statusText);
  lcd.print("        ");

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" in | Brightness: ");
  Serial.print(brightness);
  Serial.print(" | ");
  Serial.println(statusText);

  delay(300);
}

long getDistanceInches() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);

  long inches = duration / 74 / 2;
  return inches;
}

void allLEDsOff() {
  analogWrite(RED_LED, 0);
  analogWrite(YELLOW_LED, 0);
  analogWrite(GREEN_LED, 0);
}