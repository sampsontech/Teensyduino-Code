/*
*    Measure the distance moved by using the Accelerometer on the 
*    SparkFun 9DOF
*
*    Requirements:  - "Wire" I2C library
*                   - SparkFun 9DOF Class (version 6)
*
*    TODO:
*
*    Ref:
*
*/

#include <Wire.h>
#include "SparkFun_9DOF_6_Class.cpp"

// Define accelerometer object 
ADXL345_HAL Accel;

//---------------------------------------------------------------------------
void setup() {
  // Setup Serial output
  Serial.begin(115200);
  Serial.println("Initiating Startup...");

  // Setup I2C interface
  Wire.begin();

  // Initialize accelerometer object 
  Serial.print("Setting up ADXL345 Accelerometer...");
  Accel.Init_Dev(&Wire);    // Pass the "Wire" I2C object that it is to use 
  Serial.println("Done");

  // force a calibration event
  Accel.Accel_X_Center_Ave = 99;

  // End of Setup
  Serial.println("Startup complete.");
}

//---------------------------------------------------------------------------
void loop() {
  float Pos_X = 0;          // Holds the X position
  unsigned long TT;         // Time This reading
  unsigned long TL;         // Time Last reading
  
  Serial.println("Starting measurement...");
  
  // Just run the calibration routine once
  if (Accel.Accel_X_Center_Ave==99) {
    Serial.println("Performing a Accelometer calibration using the Averaging method");
    Accel.Accel_Calib_Ave();

    Serial.println("Performing a Accelometer calibration using the Low Pass Filter method");
    Accel.Accel_Calib_LPF();

    Serial.println("Done");
  }

  // Read Accel and print results 
  for(;;) {
    double D = 0;
    
    TT = micros();  
    Accel.Read_Accel();
    
    D = Accel.Accel_X * (double)9.8 / (TT - TL);
    Pos_X += D;
    TL = TT;
    
    Serial.print("Pos = ");
    Serial.print(Pos_X,6);
    Serial.print("\tDelta = ");
    Serial.print(D,6);
    Serial.println();
    
    // Introduce a delay into the loop
    delay(50);
  }
}


