byte variable[80];
byte index = 0;
byte incomingByte, LowerByte, UpperByte;
float oxygen1, oxygen2,flow1,flow2;

// unsigned int Reading[12], Flip[12];
bool stringComplete = false;  // whether the string is complete

void setup() {
  int error;
  Serial.begin(9600, SERIAL_8N1);
} // setup()

void loop() {
  while(!Serial.available()); // wait for a character
  incomingByte = Serial.read();
  if (incomingByte == 0x16) {
      while(!Serial.available()); // wait for a character
      incomingByte = Serial.read();
      while(!Serial.available()); // wait for a character
      incomingByte = Serial.read();

      // Get purity
      while(!Serial.available()); // wait for a character
      UpperByte = Serial.read();
      while(!Serial.available()); // wait for a character
      LowerByte = Serial.read();
      oxygen1 = (UpperByte * 256) ;
      oxygen2 = LowerByte;
      Serial.print("Oxygen purity=");      
      Serial.print((oxygen1+oxygen2)/10,1);      
      Serial.print(" %");
      Serial.print(" ,");    

      // Get flow rate
      while(!Serial.available()); // wait for a character
      UpperByte = Serial.read();
      while(!Serial.available()); // wait for a character
      LowerByte = Serial.read();
      flow1 = UpperByte * 256 ;
      flow2 = LowerByte;
      Serial.print("Flow rate=");      
      Serial.print((flow1+flow2)/10,1);      
      Serial.println(" lpm"); // "m" in lpm is not visible
      
      while(!Serial.available()); // wait for a character
      incomingByte = Serial.read();
      while(!Serial.available()); // wait for a character
      incomingByte = Serial.read();
      while(!Serial.available()); // wait for a character
      incomingByte = Serial.read();
      while(!Serial.available()); // wait for a character
      incomingByte = Serial.read();
      while(!Serial.available()); // wait for a character
      incomingByte = Serial.read();
    
    
    }
} // loop()
