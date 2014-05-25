#include <Wire.h>

// set the variables for the HMC5883L Compass
  #define CompassAddress (0x1E)  // Device 7 bit base address (Drop the LSB which holds the R/W bit)
  // Hold the Compass readings and values
  int Compass_Raw_X, Compass_Raw_Y, Compass_Raw_Z;          // Compass raw readings
  int Compass_Calib_X, Compass_Calib_Y, Compass_Calib_Z;    // Compass calibrated readings
  int Compass_Max_X, Compass_Max_Y, Compass_Max_Z;          // Compass raw max upper limit reading
  int Compass_Min_X, Compass_Min_Y, Compass_Min_Z;          // Compass raw min lower limit reading
  int Compass_Offset_X, Compass_Offset_Y, Compass_Offset_Z;  // Compass calibration offset 
  float Compass_Heading;                                    // Compass heading in degrees on the X Y plane
  float Compass_Heading_XZ;                                 // Compass heading in degrees on the X Z plane
  float Compass_Heading_YZ;                                 // Compass heading in degrees on the Y Z plane
  // Once you have your heading, you must then add your 'Declination Angle', which is
  // the 'Error' of the magnetic field in your location. Find yours here: http://www.magnetic-declination.com/
  // If you cannot find your Declination, set it to zero, your compass will be slightly off.
  // The angle must be in Radians (Radians = Degrees * (PI / 180))
  //float Compass_Declination_Angle = 0;                      // Compass Declination Angle error for my area
  float Compass_Declination_Angle = 0.20595;                // 11 deg 48' = Cranbourne, Victoria, Australia

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
  Wire.write(0x00);          // point to register address 1 (Configuration Register A)
  Wire.write(0b01110000);    // set to 8 samples per reading @ 15Hz
  Wire.endTransmission();

  Wire.beginTransmission(CompassAddress); 
  Wire.write(0x01);          // point to register address 1 (Configuration Register B)
  Wire.write(0b00100000);    // Set gain to 1 (default - sensor bandwidth +/-1.3 Guass & Scale 0.92)
  Wire.endTransmission();

  Wire.beginTransmission(CompassAddress); 
  Wire.write(0x02);          // point to register address 2 (Mode Register)
  Wire.write(0b00000000);    // Set continuous measurement mode
  Wire.endTransmission();

  // Help start the calibration process - especially through the initial cycles - with some rough initial data
  // These are a bit smaller than what is ultimatly expected by about 1/4. But will help until the ultimate
  // Min's and Max's are finally found through device rotation. 
  Compass_Max_X = 450;
  Compass_Min_X = -450;
  Compass_Max_Y = 450;
  Compass_Min_Y = -450;
  Compass_Max_Z = 450;
  Compass_Min_Z = -450;

  Serial.println("Done");
  delay(100);
  
// Setup the ADXL345 Accelerometer
  Serial.print("Setting up ADXL345 Accelerometer...");

  //Put the sensor into measurement mode. The data sheet recommends that you first Reset, then Sleep mode then Measurement mode.
  Wire.beginTransmission(AccelAddress); 
  Wire.write(0x2D);  //Reset the POWER_CTL (0x2D) register.
  Wire.write(0b00000000);
  Wire.endTransmission();

  Wire.beginTransmission(AccelAddress); 
  Wire.write(0x2D);  //Put the ADXL345 into StandBy mode by writing 0x10 to the POWER_CTL (0x2D) register.
  Wire.write(0b00010000);
  Wire.endTransmission();

  Wire.beginTransmission(AccelAddress); 
  Wire.write(0x2D);  //Put the ADXL345 into Measurement Mode by writing 0x08 to the POWER_CTL (0x2D) register.
  Wire.write(0b00001000);
  Wire.endTransmission();

  //Set the G force range the sensor will work in 
  Wire.beginTransmission(AccelAddress); 
  Wire.write(0x31);  //Put the ADXL345 into +/- 4G range by writing the value 0x01 to the DATA_FORMAT (0x31) register.
  Wire.write(0b00000001);  //(Default is +/-2G range
  Wire.endTransmission();

  Serial.println("Done");
  delay(100);
  
// Setup the ITG3200 Gyroscope
  Serial.print("Setting up ITG3200 Gyroscope...");

  //Set the sample rate to 100 hz
  Wire.beginTransmission(GyroAddress); 
  Wire.write(0x15);  // point to Sample Rate Divider register address
  //Wire.write(0x09);  // (internal sample rate of 8 kHz) / (*9* + 1) = 800 per second (800Hz)
  Wire.write(0b01001111);  // (internal sample rate of 8 kHz) / (*79* + 1) = 100 per second (100Hz)
  Wire.endTransmission();

  //Set the gyroscope scale for +/-2000 degrees per second
  Wire.beginTransmission(GyroAddress); 
  Wire.write(0x16);  // point to DLPF and FS register address (0x16)
  Wire.write(0b00011000);  // set Full Scale range (+/-2000deg/sec) and DLPF (LPF=256Hz and Internal Sample Rate = 8kHz)
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

  Serial.print("Compass Calib Heading\t =");
  Serial.print("\t XY=");
  Serial.print(Compass_Heading);
  Serial.print("\t XZ=");
  Serial.print(Compass_Heading_XZ);
  Serial.print("\t YZ=");
  Serial.println(Compass_Heading_YZ);


  Serial.print("Compass Calib");
  Serial.print("\t X=");
  Serial.print(Compass_Calib_X, DEC);
  Serial.print("\t : Y=");
  Serial.print(Compass_Calib_Y, DEC);
  Serial.print("\t : Z=");
  Serial.println(Compass_Calib_Z, DEC);

  Serial.print("Compass Raw");
  Serial.print("\t X=");
  Serial.print(Compass_Raw_X, DEC);
  Serial.print("\t : Y=");
  Serial.print(Compass_Raw_Y, DEC);
  Serial.print("\t : Z=");
  Serial.println(Compass_Raw_Z, DEC);

  Serial.print("Compass Min");
  Serial.print("\t X=");
  Serial.print(Compass_Min_X, DEC);
  Serial.print("\t : Y=");
  Serial.print(Compass_Min_Y, DEC);
  Serial.print("\t : Z=");
  Serial.println(Compass_Min_Z, DEC);

  Serial.print("Compass Max");
  Serial.print("\t X=");
  Serial.print(Compass_Max_X, DEC);
  Serial.print("\t : Y=");
  Serial.print(Compass_Max_Y, DEC);
  Serial.print("\t : Z=");
  Serial.println(Compass_Max_Z, DEC);

  Serial.print("Compass Offset");
  Serial.print("\t X=");
  Serial.print(Compass_Offset_X, DEC);
  Serial.print("\t : Y=");
  Serial.print(Compass_Offset_Y, DEC);
  Serial.print("\t : Z=");
  Serial.println(Compass_Offset_Z, DEC);

  Serial.println("");

  // Read Accel and print results 
  Read_Accel();
  Serial.print("Accel X=");
  Serial.print(Accel_X, DEC);
  Serial.print("\t : Y=");
  Serial.print(Accel_Y, DEC);
  Serial.print("\t : Z=");
  Serial.println(Accel_Z, DEC);

  // Read Gyro and print results 
  Read_Gyro();
  Serial.print("Gyro X=");
  Serial.print(Gyro_X, DEC);
  Serial.print("\t : Y=");
  Serial.print(Gyro_Y, DEC);
  Serial.print("\t : Z=");
  Serial.print(Gyro_Z, DEC);
  Serial.print("\t Gyro Temp=");
  Serial.println(Gyro_T, DEC);
  
  Serial.print("Gyro Adjusted Temp=");
  // Negative reading offset at +35C degrees = -13200
  // Sensitivity = 280 LSB per degree
  // Range = -30C to +85C degrees
  // Therefore TempC = (reading + offset) / sensitivity + range lower limit
  // Therefore TempC = (Gyro_T + 13200) / 280 + 30
  // But the test unit seems to be a little under (per unit variance?).
  // Therefore I'm adding 1 additional degree which seems to be within spec limits
  Serial.println( (((float)Gyro_T+13200)/280)+31 );  

  Serial.println();
  // Introduce a delay into the loop
  delay(70);
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
    //Just confirming that the data register order is X Z Y (ie NOT X Y Z) 
    Compass_Raw_X = (Wire.read() * 256) + Wire.read();  // X axis MSB * 256 + X axis LSB
    Compass_Raw_Z = (Wire.read() * 256) + Wire.read();  // Z axis MSB * 256 + Z axis LSB
    Compass_Raw_Y = (Wire.read() * 256) + Wire.read();  // Y axis MSB * 256 + Y axis LSB
    }
  
  // Incorrent number of returned bytes
  else {
    Serial.println("Recieving incorrect amount of bytes from Compass");
    while(Wire.available()) {
      Serial.print("data byte = ");
      Serial.println(Wire.read(), DEC);    //print the returned number as a decimal
    }
  }
  Wire.endTransmission();
  
  // Scale the Raw readings based on the sensor scale (Gauss = 1.3 & Scale = 0.92)
  Compass_Raw_X *= 0.92;
  Compass_Raw_Y *= 0.92;
  Compass_Raw_Z *= 0.92;
    
  // Update the Max Min limit readings
  if (Compass_Raw_X > Compass_Max_X) Compass_Max_X = Compass_Raw_X;
  if (Compass_Raw_X < Compass_Min_X) Compass_Min_X = Compass_Raw_X;
  if (Compass_Raw_Y > Compass_Max_Y) Compass_Max_Y = Compass_Raw_Y;
  if (Compass_Raw_Y < Compass_Min_Y) Compass_Min_Y = Compass_Raw_Y;
  if (Compass_Raw_Z > Compass_Max_Z) Compass_Max_Z = Compass_Raw_Z;
  if (Compass_Raw_Z < Compass_Min_Z) Compass_Min_Z = Compass_Raw_Z;
  
  // Update the offset
  Compass_Offset_X = (Compass_Max_X + Compass_Min_X) / 2;
  Compass_Offset_Y = (Compass_Max_Y + Compass_Min_Y) / 2;
  Compass_Offset_Z = (Compass_Max_Z + Compass_Min_Z) / 2;
  
  // Calculate calibrated readings
  Compass_Calib_X = Compass_Raw_X - Compass_Offset_X;
  Compass_Calib_Y = Compass_Raw_Y - Compass_Offset_Y;
  Compass_Calib_Z = Compass_Raw_Z - Compass_Offset_Z;
  
  //--- Calculate the X Y plane heading ---
  // Calculate heading (radians) using the X, Y plane - Assuming magnetometer is level
  Compass_Heading = atan2(Compass_Calib_Y, Compass_Calib_X);
  // Adjust with Declination error (magnetic north vs true north)
  Compass_Heading += Compass_Declination_Angle;
  // Correct for when signs are reversed.
  if(Compass_Heading < 0) Compass_Heading += 2*PI;
  // Check for wrap due to addition of declination.
  if(Compass_Heading > 2*PI) Compass_Heading -= 2*PI;  
  // Convert radians to degrees for readability.
  Compass_Heading *= 180/M_PI;
  
  //--- Calculate the X Z plane heading ---
  // Calculate heading (radians) using the X, Z plane
  Compass_Heading_XZ = atan2(Compass_Calib_Z, Compass_Calib_X);
  // Adjust with Declination error (magnetic north vs true north)
  Compass_Heading_XZ += Compass_Declination_Angle;
  // Correct for when signs are reversed.
  if(Compass_Heading_XZ < 0) Compass_Heading_XZ += 2*PI;
  // Check for wrap due to addition of declination.
  if(Compass_Heading_XZ > 2*PI) Compass_Heading_XZ -= 2*PI;  
  // Convert radians to degrees for readability.
  Compass_Heading_XZ *= 180/M_PI;
  
  //--- Calculate the Y Z plane heading ---
  // Calculate heading (radians) using the Y, Z plane
  Compass_Heading_YZ = atan2(Compass_Calib_Z, Compass_Calib_Y);
  // Adjust with Declination error (magnetic north vs true north)
  Compass_Heading_YZ += Compass_Declination_Angle;
  // Correct for when signs are reversed.
  if(Compass_Heading_YZ < 0) Compass_Heading_YZ += 2*PI;
  // Check for wrap due to addition of declination.
  if(Compass_Heading_YZ > 2*PI) Compass_Heading_YZ -= 2*PI;  
  // Convert radians to degrees for readability.
  Compass_Heading_YZ *= 180/M_PI;
  
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
    Accel_X = Wire.read() + (Wire.read() * 256);  // X axis LSB + X axis MSB * 256
    Accel_Y = Wire.read() + (Wire.read() * 256);  // Y axis LSB + Y axis MSB * 256
    Accel_Z = Wire.read() + (Wire.read() * 256);  // Z axis LSB + Z axis MSB * 256
  }

  // Incorrent number of returned bytes
  else {
    Serial.println("Recieving incorrect amount of bytes from Accelerometer");
    while(Wire.available()) {
      Serial.print("data byte = ");
      Serial.println(Wire.read(), DEC);    //print the returned number as a decimal
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
    Gyro_T = (Wire.read() * 256) + Wire.read();  // Temp MSB * 256 + Temp LSB
    Gyro_X = (Wire.read() * 256) + Wire.read();  // X axis MSB * 256 + X axis LSB
    Gyro_Y = (Wire.read() * 256) + Wire.read();  // Y axis MSB * 256 + Y axis LSB
    Gyro_Z = (Wire.read() * 256) + Wire.read();  // Z axis MSB * 256 + Z axis LSB
    }
  
  // Incorrent number of returned bytes
  else {
    Serial.println("Recieving incorrect amount of bytes from Gyroscope");
    while(Wire.available()) {
      Serial.print("data byte = ");
      Serial.println(Wire.read(), DEC);    //print the returned number as a decimal
    }
  }
  Wire.endTransmission();
}


