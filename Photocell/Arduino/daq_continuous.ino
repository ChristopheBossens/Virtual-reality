// Arduino photocell sketch
// In the setup method, the value in the Serial.begin() function call
// represents the baud rate, this can be tweaked if higher transmission speeds
// are desired (but might loose accuracy when rate is too high
short analogPin = 0;
int analogValue = 0;
int testValue = 1000;

long timeStamp;
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
  timeStamp = micros();
  analogValue = analogRead(analogPin); 
  
  if (Serial.available() > 0)         // Tag byte
    packet[2] = Serial.read();
  else
    packet[2] = 0x01;  
    
  packet[3] = (byte)timeStamp;        // T1 byte
  packet[4] = (byte)timeStamp >> 8;   // T2 byte
  packet[5] = lowByte(analogValue);   // A1 byte
  packet[6] = highByte(analogValue);  // A2 byte
  Serial.write(packet,7);
}
