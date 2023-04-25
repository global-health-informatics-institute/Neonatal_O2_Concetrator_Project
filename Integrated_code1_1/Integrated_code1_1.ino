#include <Stepper.h>
#include <LiquidCrystal_I2C.h>


const byte ledPin = 8;        // LED PIN
const byte interruptPin = 7;  // HALL EFFECT SENSOR PIN
const int relayPin = 9;
volatile byte state = HIGH;


float sensorData[2];  // Array to store oxygen purity and flow rate

const int steps_per_rev = 1520;

Stepper motor(steps_per_rev, 2, 3, 4, 5);

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 for a 16 chars and 2 line display

void halt() {
  state = !state;  // Toggle the state of the led when the interrupt occurs
  digitalWrite(ledPin, state);
}

void homefunction() {
  // Move motor until home position reached
  while (digitalRead(interruptPin) == 1) {
    motor.step(50);
  }
}

void relay() {
  pinMode(relayPin, OUTPUT);
  TCCR1B = TCCR1B & B11111000 | B00000100;
  analogWrite(relayPin, 100);
}

void setup() {
  Serial.begin(9600);
  relay();  // Relay PWM

  // display for the LCD
  lcd.init();                                        // Initialize the LCD display
  lcd.backlight();                                   // Turn on the backlight

  // Rotary Stepper motor
  motor.setSpeed(8);
  digitalWrite(ledPin, state);                       //Led has the declared state value
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), halt, RISING);

  homefunction();
  delay(140);
}


void loop() {
  unsigned long currentMillis = millis();

  motor.step(380);
  currentMillis = millis();
  while (millis() - currentMillis < 800) {
    stop();                                        // pause the motor 
    readSensorData();                              // read data
  }
  displaydata();                                   //Display to the lcd and monitor

  motor.step(380);
  currentMillis = millis();
  while (millis() - currentMillis < 140) {
    stop();                                         // pause the motor 
    readSensorData();                               // read data
  }
  displaydata();                                    //Display to the lcd and monitor

  motor.step(380);
  currentMillis = millis();
  while (millis() - currentMillis < 800) {
    stop();                                         // pause the motor 
    readSensorData();                               // read data
  }
  displaydata();                                    //Display to the lcd and monitor

  homefunction();
  currentMillis = millis();
  while (millis() - currentMillis < 140) {
    stop();                                          // pause the motor 
    readSensorData();                                // read data
  }
  displaydata();                                     //Display to the lcd and monitor
}

// function to pause the motor
void stop() {
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
}

// function to read data from oxygen sensor
void readSensorData() {
  byte incomingByte, LowerByte, UpperByte;
  float oxygen1, oxygen2, flow1, flow2;
  static bool stringComplete = false;
  static byte variable[80];
  static byte index = 0;

  while (!Serial.available());                       // wait for a character
  incomingByte = Serial.read();
  if (incomingByte == 0x16) {
    while (!Serial.available());                     // wait for a character
    incomingByte = Serial.read();
    while (!Serial.available());                     // wait for a character
    incomingByte = Serial.read();

    // Get purity
    while (!Serial.available());                     // wait for a character
    UpperByte = Serial.read();
    while (!Serial.available());                     // wait for a character
    LowerByte = Serial.read();
    oxygen1 = (UpperByte * 256);
    oxygen2 = LowerByte;
    sensorData[0] = (oxygen1 + oxygen2) / 10;        // Store oxygen purity
    // Serial.print("Oxygen purity=");
    // Serial.print((oxygen1+oxygen2)/10,1);
    // Serial.print(" %");
    // Serial.print(" ,");

    // Get flow rate
    while (!Serial.available());                      // wait for a character
    UpperByte = Serial.read();
    while (!Serial.available());                      // wait for a character
    LowerByte = Serial.read();
    flow1 = UpperByte * 256;
    flow2 = LowerByte;
    sensorData[1] = (flow1 + flow2) / 10;             // Store flow rate
    // Serial.print("Flow rate=");
    // Serial.print((flow1+flow2)/10,1);
    // Serial.println(" lpm"); // "m" in lpm is not visible

    while (!Serial.available());                       // wait for a character
    incomingByte = Serial.read();
    while (!Serial.available());                       // wait for a character
    incomingByte = Serial.read();
    while (!Serial.available());                       // wait for a character
    incomingByte = Serial.read();
    while (!Serial.available());                       // wait for a character
    incomingByte = Serial.read();
    while (!Serial.available());                       // wait for a character
    incomingByte = Serial.read();
  }
}

// function to display data to the lcd and monitor
void displaydata() {
  static unsigned long lastDisplayMillis = 0;   // Variable to track last display time

  if (millis() - lastDisplayMillis >= 1) {      // Only display once per second
    // print purity to lcd
    lcd.clear();                                // Clear the display
    lcd.setCursor(0, 0);                        // Set the cursor to the beginning of the first line
    lcd.print("O2 purity=");                    // Print "O2 purity=" on the first line
    lcd.print(sensorData[0], 1);                // Print purity on the first line
    lcd.print("%");                             // Print "%" on the first line

    // print purity to monitor
    Serial.print("Oxygen purity=");
    Serial.print(sensorData[0], 1);
    Serial.print("%, ");

    // Buzzer on when purity is less than 30
    if (sensorData[0] < 30) {                    // check if the O2 purity value is less than 30
    digitalWrite(6, HIGH);                       // sound the buzzer at 880Hz for 1 second
    }
    else {
      digitalWrite(6, LOW);
    }

    // print flowrate to lcd
    lcd.setCursor(0, 1);                           // Set the cursor to the beginning of the second line
    lcd.print("Flow rate=");                       // Print "Flow rate=" on the second line
    lcd.print(sensorData[1], 1);                   // Print "flow rate" on the second line
    lcd.print("lpm");                              // Print "lpm" on the second line

    // print flowrate to monitor
    Serial.print("Flow rate=");
    Serial.print(sensorData[1], 1);
    Serial.println("lpm");
    lastDisplayMillis = millis();                  // Update last display time
  }
}
