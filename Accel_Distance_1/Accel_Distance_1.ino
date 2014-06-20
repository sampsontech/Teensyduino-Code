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
  double Pos_X = 0;         // Holds the X position
  double Vf = 0;            // final Velocity
  double Vi = 0;            // initial Velocity
  double A = 0;             // Acceleration
  double D = 0;             // Distance
  
  
  unsigned long TT;         // Time This reading
  unsigned long TL;         // Time Last reading
  double T;          // Time Delta
  
  
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
    double D1 = 0;
    double D2 = 0;
    double D3 = 0;
    double D4 = 0;
    
    // Calc the Time Delta for this cycle
    TT = micros();                                    // Capture the timer value
    T = (TT - TL) / (double)1000000;                  // Calc the Time Delta for this reading (Seconds)
    TL = TT;                                          // Record the time marker for the next cycle

    // Calc the acceleration in Meters per Second ^2
    Accel.Read_Accel();                               // Read the Accelormeter and calc Velocity (final)
    //Vf = Accel.Accel_X - Accel.Accel_X_Center_Ave;     // Adjust for Center using Average 
    A = (double)Accel.Accel_X - (double)Accel.Accel_X_Center_LPF;     // Adjust for Center using LPF
    A = A / (double)134;                              // Adjust for Scale (134 per G)
    A = A * (double)9.8;                              // Convert G to Meters per Second
    
    D = Vi * T + 0.5 * A * T * T;                     // Calc Distance travelled in this time delta

    Pos_X += D;                                       // Adjust the current position
    
    Vi += A * T;                                      // Hold the final velocity for the next cycle
    
    Serial.print("Pos = ");
    Serial.print(Pos_X,6);
    Serial.print("\tDist = ");
    Serial.print(D,6);
    Serial.print("\tX Raw = ");
    Serial.print(Accel.Accel_X);
    Serial.print("\tA = ");
    Serial.print(A,6);
    Serial.print("\tVi = ");
    Serial.print(Vi,6);
    Serial.print("\tT = ");
    Serial.print(T,6);

    Serial.print("\tDn = ");
    Serial.print(D1,9);
    Serial.print(" , ");
    Serial.print(D2,9);
    Serial.print(" , ");
    Serial.print(D3,9);
    
    Serial.println();
    
    // Introduce a delay into the loop
    delay(50);
  }
}


