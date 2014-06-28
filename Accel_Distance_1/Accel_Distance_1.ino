/*
*    Measure the distance moved by using the Accelerometer on the 
*    SparkFun 9DOF
*
*    Requirements:  - "Wire" I2C library
*                   - SparkFun 9DOF Class (version 6)
                    - Kalman Filter Class
*
*    TODO:
*
*    Ref:
*
*/

#include <Wire.h>
#include "SparkFun_9DOF_6_Class.cpp"
#include "KalmanFilter_Class_2.cpp"
#include "NoiseDeadBandFilter_Class.cpp"

// Define accelerometer object 
ADXL345_HAL Accel;
KalmanFilter KF;
NoiseDeadBandFilter NDBF;

//---------------------------------------------------------------------------
void setup() {
  // Setup Serial output
  Serial.begin(115200);
  Serial.println("Initiating Startup...");

  //---------------------------------------------------------------------------
  // Setup I2C interface
  Wire.begin();

  //---------------------------------------------------------------------------
  // Initialize accelerometer object 
  Serial.print("Setting up ADXL345 Accelerometer...");
  Accel.Init_Dev(&Wire);    // Pass the "Wire" I2C object that it is to use 
  Serial.println("Done");

  //---------------------------------------------------------------------------
  Serial.println("Performing Accelometer calibration to establish the Noise Dead Band");
  double ARU = -1000;      // Accel NDBF upper range value
  double ARL = 1000;       // Accel NDBF lower range value

  // read 1000 samples and get the MIN and MAX values to used for the Noise Dead Band Filter
  for (int x = 1; x <= 1000; x++) {
    Accel.Read_Accel();
    if (Accel.Accel_X > ARU) ARU = Accel.Accel_X;
    if (Accel.Accel_X < ARL) ARL = Accel.Accel_X;
  }
  NDBF.SetRange(ARU, ARL);                // update the NDBF with the initial range
  Serial.print("Accel NDB Upper = ");
  Serial.print(ARU,9);
  Serial.print("\tAccel NDB Lower = ");
  Serial.println(ARL,9);
  Serial.println("Done");

  //---------------------------------------------------------------------------
  Serial.println("Performing Accelometer calibration using the Averaging method");
  Accel.Accel_Calib_Ave();
  Serial.print("Accel Calib Ave = ");
  Serial.println(Accel.Accel_X_Center_Ave,9);

  //---------------------------------------------------------------------------
  Serial.println("Performing Accelometer calibration using the Low Pass Filter method");
  // Accel.Accel_Calib_LPF();
  
  double X_Temp = 0;

  // read 1000 samples and get the MIN and MAX values to used for the Noise Dead Band Filter
  for (int x = 1; x <= 1000; x++) {
    Accel.Read_Accel();
    X_Temp = NDBF.Filter(Accel.Accel_X);          // Filter out the noise dead band
   
    // Take 90% of the previous cumulative value and 10% on the current reading 
    // (Bit like averaging
    if (x == 1) {                                  // Prime the 1st reading
      Accel.Accel_X_Center_LPF = X_Temp;    
    }
    else {
      Accel.Accel_X_Center_LPF = Accel.Accel_X_Center_LPF * 0.9f + X_Temp * 0.1f;
    }
  }

  Serial.print("Accel Calib LPF = ");
  Serial.println(Accel.Accel_X_Center_LPF,9);

  //---------------------------------------------------------------------------
  // Set up a Kalman Filter for the input signal
  KF.setState(0);

  //---------------------------------------------------------------------------
  // End of Setup
  Serial.println("Startup complete.");
  
  delay(1000);
}

//---------------------------------------------------------------------------
void loop() {
  double Pos_X = 0;         // Holds the X position
  double Vf = 0;            // final Velocity
  double Vi = 0;            // initial Velocity
  double A = 0;             // Acceleration
  double D = 99999;             // Distance
  
  
  unsigned long TS;         // Time Started reading
  unsigned long TT;         // Time This reading
  unsigned long TL;         // Time Last reading
  double T;                 // Time Delta (in Seconds)
  
  Serial.println("Starting measurement...");

  // Initialise the timing data (need to do this as late as possible
  // so the first reading isn't skewed
  TS = micros();                                      // Capture the Start Time
  TL = TS;                                            // Initialise timer variables

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
    Accel.Read_Accel();                                // Read the Accelormeter

    A = NDBF.Filter(Accel.Accel_X);                    // Filter out the Noise Dead Band 
    D1 = A;
    
    A = A - Accel.Accel_X_Center_LPF;     // Adjust for center offset using LPF
    D2 = A;

    //A = DSF_Ave2(A);               // Smooth the raw sensor data
    //D1 = A;

    //A = DSF_Ave3(A);               // Smooth the raw sensor data
    //D2 = A;

    //A = DSF_LPF(A, 0.2);          // Smooth the raw sensor data
    //D3 = A;
    
    //A = KF.filter(A);             // Smooth the raw sensor data using a Kalman Filter
    //D4 = A;


    // TODO: Accel class: develop better calibration routines that calc/measure the scales of the sensor
    A = A / 134;                                      // Adjust for accelerometer scale (134 per G)
    A = A * 9.8;                                      // Convert G to Meters per Second
    
    // Dont calc velocity and position for the 1st 3 seconds of measurement
    // This is to allow all the filters to normalize 
    if (TT > (TS + 3000000)) {
    
      D = Vi * T + 0.5 * A * T * T;                     // Calc Distance travelled in this time delta

      Pos_X += D;                                       // Adjust the current position
    
      Vi += A * T;                                      // Hold the final velocity for the next cycle
    }
    
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
    Serial.print(" , ");
    Serial.print(D4,9);
    
    Serial.println();
    
    // Introduce a delay into the loop
    delay(25);
  }
}

// Digital Signal Filter - Average by 2 elements
// (current and last)
double DSF_Ave2(double I) {
  static double LI;                            // last input
  double O = 0;                                // output
  
  O = (I + LI) / 2;                            // ave together the current and last inputs
  LI = I;                                      // update the last input to the current value

  return O;                                    // return the output
}

// Digital Signal Filter - Average by 3 elements
// (current and last)
double DSF_Ave3(double I) {
  static double LI1;                           // last input
  static double LI2;                           // last input
  double O = 0;                                // output
  
  O = (I + LI1 + LI2) / 3;                     // ave together the current and last inputs
  
  LI2 = LI1;                                   // update the last input values
  LI1 = I;

  return O;                                    // return the output
}

// Digital Signal Filter - Low Pass Filter using x Factor
// (current and last)
double DSF_LPF(double I, double F) {
  static double LI;                           // last input
  double O = 0;                               // output
  
  O = I * F + LI * (1 - F);                  // Low Pass Filter calc using this an dlast inputs by provided scale factor
  
  LI = I;                                     // update the last input values

  return O;                                    // return the output
}

