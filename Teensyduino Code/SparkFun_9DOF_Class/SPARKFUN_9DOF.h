#ifndef SPARKFUN_9DOF_H
#define SPARKFUN_9DOF_H
 
#include <WProgram.h>       // Std IO class
#include <Wire.h>           // I2C interface class 
 
class SPARKFUN_9DOF {

private:

    struct{
        int x;
        int y;
        int z;
    } accel;



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
  int Accel_Reading_Count;
  float Accel_X_Center_Ave, Accel_Y_Center_Ave, Accel_Z_Center_Ave;
  long Accel_X_Center_Ave_Total, Accel_Y_Center_Ave_Total, Accel_Z_Center_Ave_Total;
  float Accel_X_Center_LPF, Accel_Y_Center_LPF, Accel_Z_Center_LPF;

  // set the variables for the ITG3200 Gyroscope
  #define GyroAddress (0x68) // Device 7 bit base address (Drop the LSB which holds the R/W bit)
  // (ALT base address is used when AD0 pin is GNDed (as in the SparkFun 9DOF board) 
  int Gyro_T;           // Hold the last gyro Temperature reading
  int Gyro_X;           // Hold the last gyro X Axis reading
  int Gyro_Y;           // Hold the last gyro Y Axis  reading
  int Gyro_Z;           // Hold the last gyro Z Axis  reading


protected:

public:
        
        
        
    // Class Constructor
    SPARKFUN_9DOF() {
        
    };			
 
    //Class Destructor
    ~SPARKFUN_9DOF() {
        
    };

    // Initialise the device
    void init_dev() {
                
    };				

    // Read all the sensors
    void read_dev() {
                
    };
        
        
    // Read the Accelerometer
    void Read_Accel() {
        
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
        
};
 
#endif
