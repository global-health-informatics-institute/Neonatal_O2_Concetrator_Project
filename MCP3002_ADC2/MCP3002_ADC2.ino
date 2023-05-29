#include <SPI.h>

const int slaveSelect = 6; // Slave Select
const int LED = 7;
const byte READ = 0x60; // Channel 0 Read, MSBF
byte MSB, LSB;
word ChannelA;

void setup() {
  pinMode(slaveSelect, OUTPUT); // Sets slave pin as output
  pinMode(LED, OUTPUT);

  Serial.begin(9600); // Start serial monitor

  SPI.begin(); // Starts SPI
  SPI.setBitOrder(MSBFIRST); // Sets MSB first
  PORTD = 0xFF;
}

void loop() {
  digitalWrite(slaveSelect, LOW); // Pulls slaveSelect pin low, readying the ADC to read
  MSB = SPI.transfer(READ); // Pulls in the MSB
  LSB = SPI.transfer(0); // Pulls in the LSB
  digitalWrite(slaveSelect, HIGH); // Turns ADC off

  ChannelA = word(MSB, LSB); // Combines the two bytes into a word
  voltage = ChannelA * (5.114/1023) * 3.245;
  Serial.print("Voltage ");
  Serial.print(voltage);
  Serial.print("    ADC Value   ");
  Serial.println(ChannelA);

  if (ChannelA > 500) {
    PORTD = PORTD ^ 0x80;
  } else {
    // Do nothing
  }
}
