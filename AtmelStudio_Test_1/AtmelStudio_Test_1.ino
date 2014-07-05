void setup()
{
// Setup Serial output
  Serial.begin(115200);
  Serial.println("Initiating Startup...");
  delay(100);
  Serial.println("Done...");
}

void loop()
{
Serial.print("x");
delay(100);
}
