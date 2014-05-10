#include <Wire.h>

// set the variables for the HMC5883L Compass
  #define CompassAddress (0x1E)  // Device 7 bit base address (Drop the LSB which holds the R/W bit)
  int Compass_X;  // Hold the last compass X axis reading
  int Compass_Y;  // Hold the last compass Y axis reading
  int Compass_Z;  // Hold the last compass Z axis reading

// set the variables for the ADXL345 Accelerometer
  #define AccelAddress (0x53) // Device ALT 7 bit base address (Drop the LSB which holds the R/W bit)
  // (ALT base address is used when pin 12 is GNDed (as in the SparkFun 9DOF board) 
  int Accel_X;          // Hold the last accelerometer X axis reading
  int Accel_Y;          // Hold the last accelerometer Y axis reading
  int Accel_Z;          // Hold the last accelerometer Z axis reading

// set the variables for the ITG3200 Gyroscope
  #define GyroAddress (0x68) // Device 7 bit base address (Drop the LSB which holds the R/W bit)
  // (ALT base address is used when AD0 pin is GNDed (as in the SparkFun 9DOF board) 
  int Gyro_T;           // Hold the last gyro Temperature reading
  int Gyro_X;           // Hold the last gyro X Axis reading
  int Gyro_Y;           // Hold the last gyro Y Axis  reading
  int Gyro_Z;           // Hold the last gyro Z Axis  reading

//---------------------------------------------------------------------------
void setup() 
{
// Setup Serial output
  Serial.begin(115200);
  Serial.println("Initiating Startup...");

// Setup I2C interface
  Wire.begin();
 
// Setup the HMC5883L Compass
  Serial.print("Setting up HMC5883L Compass...");

  Wire.beginTransmission(CompassAddress); 
  Wire.write(0x01);  // point to register address 1 (Configuration Register B)
  Wire.write(0x20);  // set - Gauss Scale to +/-1.3
  Wire.endTransmission();

  Wire.beginTransmission(CompassAddress); 
  Wire.write(0x02);  // point to register address 2 (Mode Register)
  Wire.write(0x00);  // set - Continuous-measurement mode
  Wire.endTransmission();
  Serial.println("Done");
  delay(100);
  
// Setup the ADXL345 Accelerometer
  Serial.print("Setting up ADXL345 Accelerometer...");

  //Put the sensor into measurement mode. The data sheet recommends that you first Reset, then Sleep mode then Measurement mode.
  Wire.beginTransmission(AccelAddress); 
  Wire.write(0x2D);  //Reset the POWER_CTL (0x2D) register.
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.beginTransmission(AccelAddress); 
  Wire.write(0x2D);  //Put the ADXL345 into StandBy mode by writing 0x10 to the POWER_CTL (0x2D) register.
  Wire.write(0x10);
  Wire.endTransmission();

  Wire.beginTransmission(AccelAddress); 
  Wire.write(0x2D);  //Put the ADXL345 into Measurement Mode by writing 0x08 to the POWER_CTL (0x2D) register.
  Wire.write(0x08);
  Wire.endTransmission();

  //Set the G force range the sensor will work in 
  Wire.beginTransmission(AccelAddress); 
  Wire.write(0x31);  //Put the ADXL345 into +/- 4G range by writing the value 0x01 to the DATA_FORMAT (0x31) register.
  Wire.write(0x01);  //(Default is +/-2G range
  Wire.endTransmission();
  Serial.println("Done");
  delay(100);
  
// Setup the ITG3200 Gyroscope
  Serial.print("Setting up ITG3200 Gyroscope...");

  //Set the sample rate to 100 hz
  Wire.beginTransmission(GyroAddress); 
  Wire.write(0x15);  // point to Sample Rate Divider register address
  //Wire.write(0x09);  // (internal sample rate of 8 kHz) / (*9* + 1) = 800 per second (800Hz)
  Wire.write(0x4F);  // (internal sample rate of 8 kHz) / (*79* + 1) = 100 per second (100Hz)
  Wire.endTransmission();

  //Set the gyroscope scale for +/-2000 degrees per second
  Wire.beginTransmission(GyroAddress); 
  Wire.write(0x16);  // point to DLPF and FS register address (0x16)
  Wire.write(0x18);  // set Full Scale range (+/-2000deg/sec) and DLPF (LPF=256Hz and Internal Sample Rate = 8kHz)
  Wire.endTransmission();

  Serial.println("Done");
  delay(100);

// End of Setup
  Serial.println("Startup complete.");
}

//---------------------------------------------------------------------------
void loop() 
{
  Serial.println("start read cycle...");
 
  // Read Compass and print results 
  Read_Compass();
  Serial.print("Compass X=");
  Serial.print(Compass_X, DEC);
  Serial.print(" : Y=");
  Serial.print(Compass_Y, DEC);
  Serial.print(" : Z=");
  Serial.println(Compass_Z, DEC);

  //TODO: The "heading" should be calculated with "Scaled" readings using Gauss 1.3
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(Compass_Y * 0.92, Compass_X * 0.92);

  // Once you have your heading, you must then add your 'Declination Angle', which is
  // the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  // The angle must be in Radians (Radians = Degrees * (PI / 180))
  float declinationAngle = 0.20595;  // 11deg 48' = Cranbourne, Victoria, Australia
  heading += declinationAngle;
  
  if(heading < 0) heading += 2*PI;  // Correct for when signs are reversed.

  if(heading > 2*PI) heading -= 2*PI;  // Check for wrap due to addition of declination.
  
  heading *= 180/M_PI; // Convert radians to degrees for readability.
  
  Serial.print("Compas Heading=");
  Serial.println(heading);

  
  

  // Read Accel and print results 
  Read_Accel();
  Serial.print("Accel X=");
  Serial.print(Accel_X, DEC);
  Serial.print(" : Y=");
  Serial.print(Accel_Y, DEC);
  Serial.print(" : Z=");
  Serial.println(Accel_Z, DEC);

  // Read Gyro and print results 
  Read_Gyro();
  Serial.print("Gyro X=");
  Serial.print(Gyro_X, DEC);
  Serial.print(" : Y=");
  Serial.print(Gyro_Y, DEC);
  Serial.print(" : Z=");
  Serial.println(Gyro_Z, DEC);
  
  Serial.print("Gyro Temp=");
  Serial.println(Gyro_T, DEC);
  
  Serial.print("Gyro Adjusted Temp=");
  Serial.println( (((float)Gyro_T+13200)/280)+35 );  //35degrees offset = -13200
  
  
  Serial.println();
  // Introduce a delay into the loop
  delay(50);
}

//---------------------------------------------------------------------------
// Read the HMC5883L Compass
void Read_Compass()
{
  // Point to the first data register
  Wire.beginTransmission(CompassAddress); 
  Wire.write(0x03);  // point to the first data register
  Wire.endTransmission();
    
  // read 6 byte, from address 3 (Data Registers)
  Wire.beginTransmission(CompassAddress); 
  Wire.requestFrom(CompassAddress, 6);
  if (Wire.available() >= 6) {
    //Just confirming that the axis order is X Z Y (ie NOT X Y Z) 
    Compass_X = (Wire.receive() * 256) + Wire.receive();  // X axis MSB * 256 + X axis LSB
    Compass_Z = (Wire.receive() * 256) + Wire.receive();  // Z axis MSB * 256 + Z axis LSB
    Compass_Y = (Wire.receive() * 256) + Wire.receive();  // Y axis MSB * 256 + Y axis LSB
    }
  
  // Incorrent number of returned bytes
  else {
    Serial.println("Recieving incorrect amount of bytes from Compass");
    while(Wire.available()) {
      Serial.print("data byte = ");
      Serial.println(Wire.receive(), DEC);    //print the returned number as a decimal
    }
  }
  Wire.endTransmission();
}

//---------------------------------------------------------------------------
// Read the ADXL345 Accelerometer
void Read_Accel()
{
  // Point to the first data register  
  Wire.beginTransmission(AccelAddress);  // start transmission to device 
  Wire.write(0x32);                       // point to the first data register DATAX0
  Wire.endTransmission();                // end transmission

  // read 6 byte, from address 32 (Data Registers)
  Wire.beginTransmission(AccelAddress);  // start transmission to device 
  Wire.requestFrom(AccelAddress, 6);
  if (Wire.available() >= 6) {
    Accel_X = Wire.receive() + (Wire.receive() * 256);  // X axis LSB + X axis MSB * 256
    Accel_Y = Wire.receive() + (Wire.receive() * 256);  // Y axis LSB + Y axis MSB * 256
    Accel_Z = Wire.receive() + (Wire.receive() * 256);  // Z axis LSB + Z axis MSB * 256
  }

  // Incorrent number of returned bytes
  else {
    Serial.println("Recieving incorrect amount of bytes from Accelerometer");
    while(Wire.available()) {
      Serial.print("data byte = ");
      Serial.println(Wire.receive(), DEC);    //print the returned number as a decimal
    }
  }
  Wire.endTransmission();
}

//---------------------------------------------------------------------------
// Read the ITG3200 Gyroscope
void Read_Gyro()
{
  // Point to the first data register
  Wire.beginTransmission(GyroAddress); 
  Wire.write(0x1B);  // point to the first data register
  Wire.endTransmission();
    
  // read 8 byte, from address 4 (Data Registers)
  Wire.beginTransmission(GyroAddress); 
  Wire.requestFrom(GyroAddress, 8);
  if (Wire.available() >= 8) {
    //Just confirming that the data order is Temp, X, Y, Z
    Gyro_T = (Wire.receive() * 256) + Wire.receive();  // Temp MSB * 256 + Temp LSB
    Gyro_X = (Wire.receive() * 256) + Wire.receive();  // X axis MSB * 256 + X axis LSB
    Gyro_Y = (Wire.receive() * 256) + Wire.receive();  // Y axis MSB * 256 + Y axis LSB
    Gyro_Z = (Wire.receive() * 256) + Wire.receive();  // Z axis MSB * 256 + Z axis LSB
    }
  
  // Incorrent number of returned bytes
  else {
    Serial.println("Recieving incorrect amount of bytes from Gyroscope");
    while(Wire.available()) {
      Serial.print("data byte = ");
      Serial.println(Wire.receive(), DEC);    //print the returned number as a decimal
    }
  }
  Wire.endTransmission();
}


