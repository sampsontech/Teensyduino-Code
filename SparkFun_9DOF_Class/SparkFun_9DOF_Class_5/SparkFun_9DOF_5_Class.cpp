/*
* SparkFun 9DOF Senser Board Classes
*
* These classes provide basic access to the sensors on the SparkFun 9DOF board  
*
* Requirements:  - "Wire.h" needs to be included by the parent code
*                - A pointer to the Wire object needs to be passed to the constructor function
*
* TODO:  - When the wrong number of returned bytes from the Wire.Available function, can those
*          bytes be ignored ordo they need to read in and then discarded???
*
* REF:   https://www.sparkfun.com/products/10724
*
*/

// ---------------------------------------------------------------------------

// ADXL345 Accelerometer
#ifndef ADXL345_HAL_CLASS    // Load guard for the ADXL345_HAL sub class
#define ADXL345_HAL_CLASS

class ADXL345_HAL {
  private:
    TwoWire * I2C;            // Hold pointer to the "Wire" I2C object which was declared/created by the parent
    
  protected:
  public:
    // set the variables for the ADXL345 Accelerometer
    #define AccelAddress (0x53) // Device ALT 7 bit base address (Drop the LSB which holds the R/W bit)
    // (ALT base address is used when pin 12 is GNDed (as in the SparkFun 9DOF board) 
    int Accel_X;          // Hold the last accelerometer X axis reading
    int Accel_Y;          // Hold the last accelerometer Y axis reading
    int Accel_Z;          // Hold the last accelerometer Z axis reading
    int Accel_Reading_Count;
    float Accel_X_Center_Ave, Accel_Y_Center_Ave, Accel_Z_Center_Ave;
    long Accel_X_Center_Ave_Total, Accel_Y_Center_Ave_Total, Accel_Z_Center_Ave_Total;
    float Accel_X_Center_LPF, Accel_Y_Center_LPF, Accel_Z_Center_LPF;
        
    // Constructor
    ADXL345_HAL() { }

    // Destructor
    ~ADXL345_HAL() { }
 
    // Initialise the device
    void Init_Dev(TwoWire * I2C_Ptr) {

      I2C = I2C_Ptr;       // Pointer to the "Wire" I2C object 
    
      // Setup the ADXL345 Accelerometer
      Serial.print("Setting up ADXL345 Accelerometer...");

      //Put the sensor into measurement mode. The data sheet recommends that you first Reset, then Sleep mode then Measurement mode.
      I2C->beginTransmission(AccelAddress); 
      I2C->write(0x2D);  //Reset the POWER_CTL (0x2D) register.
      I2C->write(0b00000000);
      I2C->endTransmission();

      I2C->beginTransmission(AccelAddress); 
      I2C->write(0x2D);  //Put the ADXL345 into StandBy mode by writing 0x10 to the POWER_CTL (0x2D) register.
      I2C->write(0b00010000);
      I2C->endTransmission();

      I2C->beginTransmission(AccelAddress); 
      I2C->write(0x2D);  //Put the ADXL345 into Measurement Mode by writing 0x08 to the POWER_CTL (0x2D) register.
      I2C->write(0b00001000);
      I2C->endTransmission();

      // force a calibration event
      Accel_X_Center_Ave = 99;

      //Set the G force range the sensor will work in 
      I2C->beginTransmission(AccelAddress); 
      I2C->write(0x31);  //Put the ADXL345 into +/- 4G range by writing the value 0x01 to the DATA_FORMAT (0x31) register.
      I2C->write(0b00000001);  //(Default is +/-2G range
      I2C->endTransmission();

      Serial.println("Done");
      delay(100);
    }

    // Read the ADXL345 Accelerometer
    void Read_Accel() {
      I2C->beginTransmission(AccelAddress);  // start transmission to device 
      I2C->write(0x32);                       // point to the first data register DATAX0
      I2C->endTransmission();                // end transmission

      // read 6 byte, from address 32 (Data Registers)
      I2C->beginTransmission(AccelAddress);  // start transmission to device 
      I2C->requestFrom(AccelAddress, 6);
      if (I2C->available() >= 6) {
        Accel_X = I2C->read() + (I2C->read() * 256);  // X axis LSB + X axis MSB * 256
        Accel_Y = I2C->read() + (I2C->read() * 256);  // Y axis LSB + Y axis MSB * 256
        Accel_Z = I2C->read() + (I2C->read() * 256);  // Z axis LSB + Z axis MSB * 256
      }

      // Incorrent number of returned bytes
      else {
        Serial.println("Recieving incorrect amount of bytes from Accelerometer");
        while(I2C->available()) {
          Serial.print("data byte = ");
          Serial.println(I2C->read(), DEC);    //print the returned number as a decimal
        }
      }
      I2C->endTransmission();
    }

    // Calibrate the Accelerometer - "Average" method
    void Accel_Calib_Ave() {
      Serial.println("Performing a Accelometer calibration using the Averaging method");
  
      Accel_X_Center_Ave = 0;
      Accel_X_Center_Ave_Total =0;
      Accel_Y_Center_Ave = 0;
      Accel_Y_Center_Ave_Total =0;
      Accel_Z_Center_Ave = 0;
      Accel_Z_Center_Ave_Total =0;
    
      for (Accel_Reading_Count=1; Accel_Reading_Count<=100; Accel_Reading_Count++) {
        Serial.print(".");
    
        Read_Accel();
      
        // Summ up the reads for the average
        Accel_X_Center_Ave_Total += Accel_X;
        Accel_Y_Center_Ave_Total += Accel_Y;
        Accel_Z_Center_Ave_Total += Accel_Z;
      }

      // Calc the average over the sample
      Accel_X_Center_Ave = (float)Accel_X_Center_Ave_Total / Accel_Reading_Count;
      Accel_Y_Center_Ave = (float)Accel_Y_Center_Ave_Total / Accel_Reading_Count;
      Accel_Z_Center_Ave = (float)Accel_Z_Center_Ave_Total / Accel_Reading_Count;

      Serial.println("Done");
}  

    // Calibrate the Accelerometer - "Low Pass Filter" method
    void Accel_Calib_LPF() {
      Serial.println("Performing a Accelometer calibration using the Low Pass Filter method");
  
      Accel_X_Center_LPF = 0;
      Accel_Y_Center_LPF = 0;
      Accel_Z_Center_LPF = 0;
    
      for (Accel_Reading_Count=1; Accel_Reading_Count<=100; Accel_Reading_Count++) {
        Serial.print(".");
    
        Read_Accel();
    
        // Take 90% of the previous cumulative value and 10% on the current reading 
        // (Bit like averaging
        Accel_X_Center_LPF = Accel_X_Center_LPF * 0.9f + Accel_X * 0.1f;
        Accel_Y_Center_LPF = Accel_Y_Center_LPF * 0.9f + Accel_Y * 0.1f;
        Accel_Z_Center_LPF = Accel_Z_Center_LPF * 0.9f + Accel_Z * 0.1f;
      }

      Serial.println("Done");
    }  

    // Print Accelerometer details to serial
    void Print_Accel_Data(int detail_level) {
      // Detail_Level of 1 = Print basic information 
      // Detail_Level of 9 = Print all available information 
  
      if (detail_level==1 || detail_level==9){
        Serial.print("Accel Center Ave");
        Serial.print("\t X=");
        Serial.print(Accel_X_Center_Ave, 6);
        Serial.print("\t Y=");
        Serial.print(Accel_Y_Center_Ave, 6);
        Serial.print("\t Z=");
        Serial.println(Accel_Z_Center_Ave, 6);

        Serial.print("Accel Center Ave Total");
        Serial.print("\t X=");
        Serial.print(Accel_X_Center_Ave_Total);
        Serial.print("\t Y=");
        Serial.print(Accel_Y_Center_Ave_Total);
        Serial.print("\t Z=");
        Serial.println(Accel_Z_Center_Ave_Total);

        Serial.print("Accel Center LPF");
        Serial.print("\t X=");
        Serial.print(Accel_X_Center_LPF, 6);
        Serial.print("\t Y=");
        Serial.print(Accel_Y_Center_LPF, 6);
        Serial.print("\t Z=");
        Serial.println(Accel_Z_Center_LPF, 6);
    
        float a_XZ, a_YZ;
        a_XZ = atan2(Accel_X_Center_LPF, Accel_Z_Center_LPF);
        a_YZ = atan2(Accel_Y_Center_LPF, Accel_Z_Center_LPF);
    
        Serial.print("Accel Center LPF - ATAN2 XZ = ");
        Serial.print(a_XZ, 6);
        Serial.print("\t ATAN2 YZ = ");
        Serial.println(a_YZ, 6);
        Serial.print("XZ + YZ = ");
        Serial.println((a_XZ+a_YZ), 6);
      }
  
      if (detail_level==9){
        Serial.print("Accel X=");
        Serial.print(Accel_X, DEC);
        Serial.print("\t : Y=");
        Serial.print(Accel_Y, DEC);
        Serial.print("\t : Z=");
        Serial.println(Accel_Z, DEC);
      }
    }

};
#endif  // ADXL345_HAL_CLASS
  
// ---------------------------------------------------------------------------

// HMC5883L Magnetometer
#ifndef HMC5883L_HAL_CLASS    // Load guard for the HMC5883L_HAL sub class
#define HMC5883L_HAL_CLASS

class HMC5883L_HAL {
  private:
    TwoWire * I2C;            // Hold pointer to the "Wire" I2C object which was declared/created by the parent
    
  protected:
  public:
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
    //float Compass_Declination_Angle = 0.20595;                // 11 deg 48' = Cranbourne, Victoria, Australia
    float Compass_Declination_Angle;                // Set the value in the Init_Dev routine
        
    // Constructor
    HMC5883L_HAL() { }

    // Destructor
    ~HMC5883L_HAL() { }
 
    // Initialise the device
    void Init_Dev(TwoWire * I2C_Ptr) {

      I2C = I2C_Ptr;       // Pointer to the "Wire" I2C object 
    
      // Setup the HMC5883L Magnetometer
      Serial.print("Setting up HMC5883L Magnetometer...");

      I2C->beginTransmission(CompassAddress); 
      I2C->write(0x00);          // point to register address 1 (Configuration Register A)
      I2C->write(0b01110000);    // set to 8 samples per reading @ 15Hz
      I2C->endTransmission();

      I2C->beginTransmission(CompassAddress); 
      I2C->write(0x01);          // point to register address 1 (Configuration Register B)
      I2C->write(0b00100000);    // Set gain to 1 (default - sensor bandwidth +/-1.3 Guass & Scale 0.92)
      I2C->endTransmission();

      I2C->beginTransmission(CompassAddress); 
      I2C->write(0x02);          // point to register address 2 (Mode Register)
      I2C->write(0b00000000);    // Set continuous measurement mode
      I2C->endTransmission();

      // Help start the calibration process - especially through the initial cycles - with some rough initial data
      // These are a bit smaller than what is ultimatly expected by about 1/4. But will help until the ultimate
      // Min's and Max's are finally found through device rotation. 
      Compass_Max_X = 450;
      Compass_Min_X = -450;
      Compass_Max_Y = 450;
      Compass_Min_Y = -450;
      Compass_Max_Z = 450;
      Compass_Min_Z = -450;
  
      // Set declination angle for this location
      Compass_Declination_Angle = 0.20595;    // 11 deg 48' = Cranbourne, Victoria, Australia

      Serial.println("Done");
      delay(100);
    }

    // Read the HMC5883L Compass
    void Read_Compass()  {
      // Point to the first data register
      I2C->beginTransmission(CompassAddress); 
      I2C->write(0x03);  // point to the first data register
      I2C->endTransmission();
    
      // read 6 byte, from address 3 (Data Registers)
      I2C->beginTransmission(CompassAddress); 
      I2C->requestFrom(CompassAddress, 6);
      if (I2C->available() >= 6) {
        //Just confirming that the data register order is X Z Y (ie NOT X Y Z) 
        Compass_Raw_X = (I2C->read() * 256) + I2C->read();  // X axis MSB * 256 + X axis LSB
        Compass_Raw_Z = (I2C->read() * 256) + I2C->read();  // Z axis MSB * 256 + Z axis LSB
        Compass_Raw_Y = (I2C->read() * 256) + I2C->read();  // Y axis MSB * 256 + Y axis LSB
      }
  
      // Incorrent number of returned bytes
      else {
        Serial.println("Recieving incorrect amount of bytes from Compass");
        while(I2C->available()) {
          Serial.print("data byte = ");
          Serial.println(I2C->read(), DEC);    //print the returned number as a decimal
        }
      }
      I2C->endTransmission();
  
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

    // Print Compass details to serial
    void Print_Compass_Data(int detail_level) {
      // Detail_Level of 1 = Print basic information 
      // Detail_Level of 9 = Print all available information 
  
      if (detail_level==1 || detail_level==9){
        Serial.print("Compass Calib Heading");
        Serial.print("\t XY=");
        Serial.print(Compass_Heading);
        Serial.print("\t XZ=");
        Serial.print(Compass_Heading_XZ);
        Serial.print("\t YZ=");
        Serial.println(Compass_Heading_YZ);
      }

      if (detail_level==9){
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
      }
    }

};
#endif  // HMC5883L_HAL_CLASS

// ---------------------------------------------------------------------------

// ITG3200 Gyroscope
#ifndef ITG3200_HAL_CLASS    // Load guard for the ITG3200_HAL sub class
#define ITG3200_HAL_CLASS

class ITG3200_HAL {
  private:
    TwoWire * I2C;            // Hold pointer to the "Wire" I2C object which was declared/created by the parent
    
  protected:
  public:
    // set the variables for the ITG3200 Gyroscope
    #define GyroAddress (0x68) // Device 7 bit base address (Drop the LSB which holds the R/W bit)
    // (ALT base address is used when AD0 pin is GNDed (as in the SparkFun 9DOF board) 
    int Gyro_T;           // Hold the last gyro Temperature reading
    int Gyro_X;           // Hold the last gyro X Axis reading
    int Gyro_Y;           // Hold the last gyro Y Axis  reading
    int Gyro_Z;           // Hold the last gyro Z Axis  reading
        
    // Constructor
    ITG3200_HAL() { }

    // Destructor
    ~ITG3200_HAL() { }
 
    // Initialise the device
    void Init_Dev(TwoWire * I2C_Ptr) {

      I2C = I2C_Ptr;       // Pointer to the "Wire" I2C object 
    
      // Setup the ITG3200 Gyroscope
      Serial.print("Setting up ITG3200 Gyroscope...");

      //Set the sample rate to 100 hz
      I2C->beginTransmission(GyroAddress); 
      I2C->write(0x15);  // point to Sample Rate Divider register address
      // I2C->write(0x09);  // (internal sample rate of 8 kHz) / (*9* + 1) = 800 per second (800Hz)
      I2C->write(0b01001111);  // (internal sample rate of 8 kHz) / (*79* + 1) = 100 per second (100Hz)
      I2C->endTransmission();

      //Set the gyroscope scale for +/-2000 degrees per second
      I2C->beginTransmission(GyroAddress); 
      I2C->write(0x16);  // point to DLPF and FS register address (0x16)
      I2C->write(0b00011000);  // set Full Scale range (+/-2000deg/sec) and DLPF (LPF=256Hz and Internal Sample Rate = 8kHz)
      I2C->endTransmission();

      Serial.println("Done");
      delay(100);
    }

    // Read the ITG3200 Gyroscope
    void Read_Gyro() {
      // Point to the first data register
      I2C->beginTransmission(GyroAddress); 
      I2C->write(0x1B);  // point to the first data register
      I2C->endTransmission();
    
      // read 8 byte, from address 4 (Data Registers)
      I2C->beginTransmission(GyroAddress); 
      I2C->requestFrom(GyroAddress, 8);
      if (I2C->available() >= 8) {
        //Just confirming that the data order is Temp, X, Y, Z
        Gyro_T = (I2C->read() * 256) + I2C->read();  // Temp MSB * 256 + Temp LSB
        Gyro_X = (I2C->read() * 256) + I2C->read();  // X axis MSB * 256 + X axis LSB
        Gyro_Y = (I2C->read() * 256) + I2C->read();  // Y axis MSB * 256 + Y axis LSB
        Gyro_Z = (I2C->read() * 256) + I2C->read();  // Z axis MSB * 256 + Z axis LSB
        }
  
      // Incorrent number of returned bytes
      else {
        Serial.println("Recieving incorrect amount of bytes from Gyroscope");
        while(I2C->available()) {
          Serial.print("data byte = ");
          Serial.println(I2C->read(), DEC);    //print the returned number as a decimal
        }
      }
      I2C->endTransmission();
    }

    // Print Gyro details to serial
    void Print_Gyro_Data() {
      Serial.print("Gyro X=");
      Serial.print(Gyro_X, DEC);
      Serial.print("\t : Y=");
      Serial.print(Gyro_Y, DEC);
      Serial.print("\t : Z=");
      Serial.print(Gyro_Z, DEC);
      Serial.print("\t Gyro Temp=");
      Serial.println(Gyro_T, DEC);
  
      Serial.print("Gyro Adjusted Temp=");
      Serial.println( (((float)Gyro_T+13200)/280)+35 );  //35degrees offset = -13200
    }

};
#endif  // ITG3200_HAL_CLASS

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

// SparkFun 9 DOF Class Support Class
#ifndef SF9DOF_SUPPORT_CLASS    // Load guard for the ADXL345_HAL sub class
#define SF9DOF_SUPPORT_CLASS

class SF9DOF_Support {
  private:
//    TwoWire * I2C;            // Hold pointer to the "Wire" I2C object which was declared/created by the parent
    
  protected:
  public:
        
    // Constructor
    SF9DOF_Support() { }

    // Destructor
    ~SF9DOF_Support() { }
 
    // Write a byte to the I2C bus
    void I2C_WB(TwoWire * I2C_Ptr, unsigned char Addr, unsigned char Reg, unsigned char Data) {
      I2C->beginTransmission(Addr);   // Start the transmission
      I2C->write(Reg);                // Point to the data register to read
      I2C->write(Data);               // Write the data to the register
      I2C->endTransmission();         // End transmission
    }

    // Read a byte from the I2C bus
    unsigned char I2C_RB(TwoWire * I2C_Ptr, unsigned char Addr, unsigned char Reg) {
      unsigned char RB = 0;           // Hold returned data

      // Point to the data register to read from
      I2C->beginTransmission(Addr);   // Start transmission to device 
      I2C->write(Reg);                // Point to the data register to read
      I2C->endTransmission();         // End transmission

      // read the byte from data register
      I2C->beginTransmission(Addr);   // Start transmission to device 
      I2C->requestFrom(Addr, 1);      // Ask for 1 byte of data
      if (I2C->available() > 0) {     // Check if there is any data to read
        RB = I2C->read();             // Read the byte
      }
      I2C->endTransmission();         // End transmission

      return RB;                      // Return the read byte
    }

    // Read multiple bytes from the I2C bus
    unsigned char[] I2C_RB(TwoWire * I2C_Ptr, unsigned char Addr, unsigned char Reg, int Num) {
      unsigned char RB[Num];          // Hold returned data

      // Point to the first data register to read from
      I2C->beginTransmission(Addr);   // Start transmission to device 
      I2C->write(Reg);                // Point to the data register to read
      I2C->endTransmission();         // End transmission

      // read the data registers
      I2C->beginTransmission(Addr);   // Start transmission to device 
      I2C->requestFrom(Addr, Num);    // Ask for 1 byte of data
      if (I2C->available() >= Num) {  // Check if there is enough data to read
        for(int x = 0; x < Num; x++) { RB[x] = I2C->read() }   // Read the data
      }
      else { RB[0] = 0; }             // Insuffcient bytes of data returned

      I2C->endTransmission();         // End transmission
      
      return RB[];                    // Return the read bytes
    }

};
#endif  // SPARKFUN_9DOF_CLASS
