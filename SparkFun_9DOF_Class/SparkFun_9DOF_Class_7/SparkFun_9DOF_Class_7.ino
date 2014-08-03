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
#include "SparkFun_9DOF_7_Class.cpp"

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
  Serial.print("Setting up ADXL345 Accelerometer...");
  Accel.Init_Dev(&Wire);    // Pass the "Wire" I2C object that it is to use 
  Serial.println("Done");

  // force a calibration event
  Accel.Accel_X_Center_Ave = 99;

  // Initialize magnetometer object 
  Serial.print("Setting up HMC5883L Magnetometer...");
  Magno.Init_Dev(&Wire);    // Pass the "Wire" I2C object that it is to use 
  Serial.println("Done");
  
  // Initialize gyroscope object 
  Serial.print("Setting up ITG3200 Gyroscope...");
  Gyro.Init_Dev(&Wire);    // Pass the "Wire" I2C object that it is to use 
  Serial.println("Done");
    
  // End of Setup
  Serial.println("Startup complete.");
}

//---------------------------------------------------------------------------
void loop() {
  Serial.println("start read cycle...");
 
  // Read Compass and print results 
  //Magno.Read_Compass();
  //Print_Compass_Data(1);

  // Read Accel and print results 
  // if (Accel.Accel_X_Center_Ave==99) Accel.Accel_Calib_Ave();      // Just run the calibration routine once
  
  //Serial.println("Performing a Accelometer calibration using the Averaging method");
  //Accel.Accel_Calib_Ave();
  //Serial.println("Done");

  //Serial.println("Performing a Accelometer calibration using the Low Pass Filter method");
  //Accel.Accel_Calib_LPF();
  //Serial.println("Done");

  Print_Accel_Data(9);
  
  // Read Gyro and print results 
  //Gyro.Read_Gyro();
  //Print_Gyro_Data();
 
  Serial.println();
  // Introduce a delay into the loop
  delay(70);
}


// Print Accelerometer details to serial
void Print_Accel_Data(int detail_level) {
  // Detail_Level of 1 = Print basic information 
  // Detail_Level of 9 = Print all available information 
  if (detail_level==1 || detail_level==9){
    Serial.print("Accel Center Ave");
    Serial.print("\t X=");
    Serial.print(Accel.Accel_X_Center_Ave, 6);
    Serial.print("\t Y=");
    Serial.print(Accel.Accel_Y_Center_Ave, 6);
    Serial.print("\t Z=");
    Serial.println(Accel.Accel_Z_Center_Ave, 6);

    Serial.print("Accel Center Ave Total");
    Serial.print("\t X=");
    Serial.print(Accel.Accel_X_Center_Ave_Total);
    Serial.print("\t Y=");
    Serial.print(Accel.Accel_Y_Center_Ave_Total);
    Serial.print("\t Z=");
    Serial.println(Accel.Accel_Z_Center_Ave_Total);

    Serial.print("Accel Center LPF");
    Serial.print("\t X=");
    Serial.print(Accel.Accel_X_Center_LPF, 6);
    Serial.print("\t Y=");
    Serial.print(Accel.Accel_Y_Center_LPF, 6);
    Serial.print("\t Z=");
    Serial.println(Accel.Accel_Z_Center_LPF, 6);
    
    float a_XZ, a_YZ;
    a_XZ = atan2(Accel.Accel_X_Center_LPF, Accel.Accel_Z_Center_LPF);
    a_YZ = atan2(Accel.Accel_Y_Center_LPF, Accel.Accel_Z_Center_LPF);
    
    Serial.print("Accel Center LPF - ATAN2 XZ = ");
    Serial.print(a_XZ, 6);
    Serial.print("\t ATAN2 YZ = ");
    Serial.println(a_YZ, 6);
    Serial.print("XZ + YZ = ");
    Serial.println((a_XZ+a_YZ), 6);
  }
  
  if (detail_level==9){
    Serial.print("Accel X=");
    Serial.print(Accel.Accel_X, DEC);
    Serial.print("\t : Y=");
    Serial.print(Accel.Accel_Y, DEC);
    Serial.print("\t : Z=");
    Serial.println(Accel.Accel_Z, DEC);
  }
}

    // Print Compass details to serial
void Print_Compass_Data(int detail_level) {
  // Detail_Level of 1 = Print basic information 
  // Detail_Level of 9 = Print all available information 
  
  if (detail_level==1 || detail_level==9){
    Serial.print("Compass Calib Heading");
    Serial.print("\t XY=");
    Serial.print(Magno.Compass_Heading);
    Serial.print("\t XZ=");
    Serial.print(Magno.Compass_Heading_XZ);
    Serial.print("\t YZ=");
    Serial.println(Magno.Compass_Heading_YZ);
  }

  if (detail_level==9){
    Serial.print("Compass Calib");
    Serial.print("\t X=");
    Serial.print(Magno.Compass_Calib_X, DEC);
    Serial.print("\t : Y=");
    Serial.print(Magno.Compass_Calib_Y, DEC);
    Serial.print("\t : Z=");
    Serial.println(Magno.Compass_Calib_Z, DEC);

    Serial.print("Compass Raw");
    Serial.print("\t X=");
    Serial.print(Magno.Compass_Raw_X, DEC);
    Serial.print("\t : Y=");
    Serial.print(Magno.Compass_Raw_Y, DEC);
    Serial.print("\t : Z=");
    Serial.println(Magno.Compass_Raw_Z, DEC);

    Serial.print("Compass Min");
    Serial.print("\t X=");
    Serial.print(Magno.Compass_Min_X, DEC);
    Serial.print("\t : Y=");
    Serial.print(Magno.Compass_Min_Y, DEC);
    Serial.print("\t : Z=");
    Serial.println(Magno.Compass_Min_Z, DEC);

    Serial.print("Compass Max");
    Serial.print("\t X=");
    Serial.print(Magno.Compass_Max_X, DEC);
    Serial.print("\t : Y=");
    Serial.print(Magno.Compass_Max_Y, DEC);
    Serial.print("\t : Z=");
    Serial.println(Magno.Compass_Max_Z, DEC);

    Serial.print("Compass Offset");
    Serial.print("\t X=");
    Serial.print(Magno.Compass_Offset_X, DEC);
    Serial.print("\t : Y=");
    Serial.print(Magno.Compass_Offset_Y, DEC);
    Serial.print("\t : Z=");
    Serial.println(Magno.Compass_Offset_Z, DEC);
  }
}

// Print Gyro details to serial
void Print_Gyro_Data() {
  Serial.print("Gyro X=");
  Serial.print(Gyro.Gyro_X, DEC);
  Serial.print("\t : Y=");
  Serial.print(Gyro.Gyro_Y, DEC);
  Serial.print("\t : Z=");
  Serial.print(Gyro.Gyro_Z, DEC);
  Serial.print("\t Gyro Temp=");
  Serial.println(Gyro.Gyro_T, DEC);
  
  Serial.print("Gyro Adjusted Temp=");
  Serial.println( (((float)Gyro.Gyro_T+13200)/280)+35 );  //35degrees offset = -13200
 }

