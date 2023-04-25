#include <Stepper.h>

const byte ledPin = 2; // LED PIN
const byte interruptPin = 3; // HALL EFFECT SENSOR PIN
volatile byte state = HIGH;

const int steps_per_rev = 1520;

Stepper motor(steps_per_rev, 8, 9, 10, 11);

void homefunction() {

  // Move motor until home position reached
  while (digitalRead(interruptPin) == 1) {


    motor.step(1);
  }
}

void setup() {
  motor.setSpeed(8);
  digitalWrite(ledPin, state); //Led has the declared state value

  pinMode(ledPin, OUTPUT);

  pinMode(interruptPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(interruptPin), halt, RISING);

  homefunction();
  delay(140) ;
}

void loop() {
  motor.step(380) ; // 90 DEGREES
  delay(800);
  motor.step(380) ;
  delay(140) ;
  motor.step(380) ;
  delay(800) ;
  homefunction();
  delay(140) ;
}

void halt() {

  state = !state ;  // Toggle the state of the led when the interrupt occurs
  digitalWrite(ledPin, state) ;
  /*while (digitalRead(interruptPin) == 0) { // switch off motor when magnet is detected
    digitalWrite(stepPin, HIGH) ;
    }*/
  // delay(2000);
  // val++;
}
