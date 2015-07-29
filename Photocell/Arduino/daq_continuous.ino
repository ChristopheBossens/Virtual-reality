// Arduino photocell sketch
// In the setup method, the value in the Serial.begin() function call
// represents the baud rate, this can be tweaked if higher transmission speeds
// are desired
short analogPin = 0;
int analogValue = 0;
int testValue = 1000;

byte packet[] = {0,0, 0, 0};
void setup()
{
  Serial.begin(250000);
}

void loop()
{
  analogValue = analogRead(analogPin);
  packet[0] = 0xff;
  packet[1] = 0xff;
  packet[2] = lowByte(analogValue);
  packet[3] = highByte(analogValue);
  Serial.write(packet,4);
}
