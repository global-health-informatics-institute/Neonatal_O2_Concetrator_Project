const int relayPin = 9;

void setup() {
  // put your setup code here, to run once:
  pinMode(relayPin, OUTPUT);
  TCCR1B = TCCR1B & B11111000 | B00000100;
  analogWrite(relayPin, 100);
}

void loop() {
  // put your main code here, to run repeatedly:

}
