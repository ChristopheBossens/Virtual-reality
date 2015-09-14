// Arduino photocell sketch
// In the setup method, the value in the Serial.begin() function call
// represents the baud rate, this can be tweaked if higher transmission speeds
// are desired (but might loose accuracy when rate is too high
short analogPin = 0;

union
{
  int asInt;
  byte asByte[2];
} analogValue;

union
{
  long asLong;
  byte asByte[4];
} timeStamp;

byte packet[] = {0, 0, 0, 0, 0, 0, 0};

void setup()
{
  // Set fixed bytes
  packet[0] = 0xff;                   // Sync 1
  packet[1] = 0xff;                   // Sync 2
  
  // Begin serial
  Serial.begin(250000);
}

void loop()
{
  timeStamp.asLong = micros();
  analogValue.asInt = analogRead(analogPin); 
  
  if (Serial.available() > 0)         // Tag byte
    packet[2] = Serial.read();
  else
    packet[2] = 0x01;  
    
  packet[3] = timeStamp.asByte[0];        // T1 byte
  packet[4] = timeStamp.asByte[1];   // T2 byte
  packet[5] = analogValue.asByte[0];   // A1 byte
  packet[6] = analogValue.asByte[1];  // A2 byte
  Serial.write(packet,7);
}
