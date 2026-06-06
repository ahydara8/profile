/***
FSM_RGB_LED_Modified.ino
This program implements a Finite State Machine.
Button press moves to next state.
Potentiometer on A0 controls LED brightness.
Programe name:    Balagie Hydara
Date modified:     3/31/2026
You can watch the video on YouTube here: https://youtu.be/BxjJdxoj_4k
***/

#include <FiniteStateMachine.h>
#include <Button.h>

const byte NUMBER_OF_STATES = 4;

// create states
State One   = State(One_fn);
State Two   = State(Two_fn);
State Three = State(Three_fn);
State Four  = State(Four_fn);

FSM FSM_RGB_LED = FSM(One);

// button
Button button = Button(A3, INPUT);
byte buttonPresses = 0;

// LED pins
#define ledRed    9
#define ledGreen 10
#define ledBlue  11

// potentiometer pin
#define potPin A0

// global variables
int potValue = 0;
int redBrightness = 0;
int greenBrightness = 0;
int blueBrightness = 0;

void setup() {
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (button.uniquePress()) {
    buttonPresses = ++buttonPresses % NUMBER_OF_STATES;

    switch (buttonPresses) {
      case 0: FSM_RGB_LED.transitionTo(One);   break;
      case 1: FSM_RGB_LED.transitionTo(Two);   break;
      case 2: FSM_RGB_LED.transitionTo(Three); break;
      case 3: FSM_RGB_LED.transitionTo(Four);  break;
    }
  }

  FSM_RGB_LED.update();
}

// State 1: vary Red LED brightness with pot
void One_fn() {
  potValue = analogRead(potPin);
  redBrightness = map(potValue, 0, 1023, 0, 255);

  analogWrite(ledRed, redBrightness);
  analogWrite(ledGreen, 0);
  analogWrite(ledBlue, 0);

  Serial.print("State 1 - Pot = ");
  Serial.print(potValue);
  Serial.print("  Red = ");
  Serial.println(redBrightness);
}

// State 2: vary Green LED brightness with pot
void Two_fn() {
  potValue = analogRead(potPin);
  greenBrightness = map(potValue, 0, 1023, 0, 255);

  analogWrite(ledRed, 0);
  analogWrite(ledGreen, greenBrightness);
  analogWrite(ledBlue, 0);

  Serial.print("State 2 - Pot = ");
  Serial.print(potValue);
  Serial.print("  Green = ");
  Serial.println(greenBrightness);
}

// State 3: vary Blue LED brightness with pot
void Three_fn() {
  potValue = analogRead(potPin);
  blueBrightness = map(potValue, 0, 1023, 0, 255);

  analogWrite(ledRed, 0);
  analogWrite(ledGreen, 0);
  analogWrite(ledBlue, blueBrightness);

  Serial.print("State 3 - Pot = ");
  Serial.print(potValue);
  Serial.print("  Blue = ");
  Serial.println(blueBrightness);
}

// State 4: vary all RGB LEDs brightness with pot
void Four_fn() {
  potValue = analogRead(potPin);

  redBrightness = map(potValue, 0, 1023, 0, 255);
  greenBrightness = map(potValue, 0, 1023, 0, 255);
  blueBrightness = map(potValue, 0, 1023, 0, 255);

  analogWrite(ledRed, redBrightness);
  analogWrite(ledGreen, greenBrightness);
  analogWrite(ledBlue, blueBrightness);

  Serial.print("State 4 - Pot = ");
  Serial.print(potValue);
  Serial.print("  Red = ");
  Serial.print(redBrightness);
  Serial.print("  Green = ");
  Serial.print(greenBrightness);
  Serial.print("  Blue = ");
  Serial.println(blueBrightness);
}
