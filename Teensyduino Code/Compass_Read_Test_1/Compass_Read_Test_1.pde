#include <Wire.h>

// set the address for the HMC5883L Compass
int CompassAddress = 0x3C;

void setup() 
{
  CompassAddress = CompassAddress >> 1;   //Drop the LSB which holds the R/W bit

  Wire.begin();
  Serial.begin(9600);
  
  
    // set the address for the HMC5883L Compass
  Wire.beginTransmission(CompassAddress); 
  // Wire.send(0x00);  // point to register address 0 
  // Wire.send(0x70);  // Set - 8-average, 15 Hz default, normal measurement

  Wire.send(0x02);  // point to register address 2 (Configuration Register B)
  Wire.send(0x00);  // set - Continuous-measurement mode
    
  Wire.endTransmission();

  delay(100);
  Serial.println("Startup complete...");
}

void loop() 
{
  byte num;
   
  Serial.println("start read...");
 
  // Point to the first data register
  Wire.beginTransmission(CompassAddress); 
  Wire.send(0x03);  // point to the first data register
  Wire.endTransmission();
 
  // read 6 byte, from address 3 (Data Registers)
  Wire.requestFrom(CompassAddress, 5);
  while(Wire.available()) {
    num = Wire.receive();
    Serial.print("num = ");
    Serial.println(num, DEC);    //print the returned number as a decimal
  }
 
  delay(1000);
}
