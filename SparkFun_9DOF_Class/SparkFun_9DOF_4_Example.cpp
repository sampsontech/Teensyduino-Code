/*
*    SparkFun 9DOF eample code that uses the SparkFun 9DOF class to
*    read the sensors, perform some basic calculations and display
*    the results
*
*    Requirements:  - "Wire" I2C library
*
*    TODO:
*
*    Ref:
*
*/

#include <Wire.h>
#include "SparkFun_9DOF_4_Class.cpp"

// Define accelerometer object 
ADXL345_HAL Accel;

// Define magnetometer object
HMC5883L_HAL Magno;

// Define gyroscope object
ITG3200_HAL Gyro;

//---------------------------------------------------------------------------
void setup() {
  // Setup Serial output
  Serial.begin(115200);
  Serial.println("Initiating Startup...");

  // Setup I2C interface
  Wire.begin();

  // Initialize accelerometer object 
  Accel.Init_Dev(&Wire);    // Pass the "Wire" I2C object that it is to use 

  // Initialize magnetometer object 
  Magno.Init_Dev(&Wire);    // Pass the "Wire" I2C object that it is to use 
    
  // Initialize gyroscope object 
  Gyro.Init_Dev(&Wire);    // Pass the "Wire" I2C object that it is to use 
    
  // End of Setup
  Serial.println("Startup complete.");
}

//---------------------------------------------------------------------------
void loop() {
  Serial.println("start read cycle...");
 
  // Read Compass and print results 
  Magno.Read_Compass();
  Magno.Print_Compass_Data(1);

  // Read Accel and print results 
  // if (Accel.Accel_X_Center_Ave==99) Accel.Accel_Calib_Ave();      // Just run the calibration routine once
  Accel.Accel_Calib_Ave();
  Accel.Accel_Calib_LPF();
  // Read_Accel();
  Accel.Print_Accel_Data(9);
  
  // Read Gyro and print results 
  Gyro.Read_Gyro();
  Gyro.Print_Gyro_Data();
 
  Serial.println();
  // Introduce a delay into the loop
  delay(70);
}
