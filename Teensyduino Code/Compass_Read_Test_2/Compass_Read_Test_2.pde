#include <Wire.h>

// set the variables for the HMC5883L Compass
  #define CompassAddress (0x1E)  // Device base address of 0x3C with LSB removed ( 0x3C >> 1 )
  //Drop the LSB which holds the R/W bit

  // Hold the last compass reading
  int Compass_X;
  int Compass_Y;
  int Compass_Z;

// set the variables for the ADXL345 Accelerometer
  #define AccelAddress (0x53) // Device ALT base address of 0x53 (SD0-pin12 GNDed) with 
  // (alt address - pin 12->GND) as specified in data sheet 

  byte _buff[6];
  char POWER_CTL = 0x2D;	//Power Control Register
  char DATA_FORMAT = 0x31;
  char DATAX0 = 0x32;	//X-Axis Data 0
  char DATAX1 = 0x33;	//X-Axis Data 1
  char DATAY0 = 0x34;	//Y-Axis Data 0
  char DATAY1 = 0x35;	//Y-Axis Data 1
  char DATAZ0 = 0x36;	//Z-Axis Data 0
  char DATAZ1 = 0x37;	//Z-Axis Data 1
  int Accel_X;          // Hold the last accelerometer reading
  int Accel_Y;          // Hold the last accelerometer reading
  int Accel_Z;          // Hold the last accelerometer reading


void setup() 
{
  // Setup Serial output
  //Serial.begin(9600);
  Serial.begin(115200);
  Serial.println("Initiating Startup...");

  // Setup I2C interface and connected devices
  Wire.begin();
 
  // Setup the HMC5883L Compass
  Serial.print("Setting up HMC5883L Compass...");
  Wire.beginTransmission(CompassAddress); 
  Wire.send(0x02);  // point to register address 2 (Configuration Register B)
  Wire.send(0x00);  // set - Continuous-measurement mode
  Wire.endTransmission();
  Serial.println("Done...");
  delay(100);
  
  // Setup the ADXL345 Accelerometer
  Serial.print("Setting up ADXL345 Accelerometer...");
  Wire.beginTransmission(AccelAddress); 
  Wire.send(DATA_FORMAT, 0x01);  //Put the ADXL345 into +/- 4G range by writing the value 0x01 to the DATA_FORMAT register.
  Wire.send(POWER_CTL, 0x08);    //Put the ADXL345 into Measurement Mode by writing 0x08 to the POWER_CTL register.
  Wire.endTransmission();
  Serial.println("Done...");
  delay(100);
  

  Serial.println("Startup complete.");
}

void loop() 
{
  Serial.println("start read...");
 
  Read_Compass();
  Serial.print("X = ");
  Serial.print(Compass_X, DEC);
  Serial.print(" : Y = ");
  Serial.print(Compass_Y, DEC);
  Serial.print(" : Z = ");
  Serial.println(Compass_Z, DEC);

 delay(50);
}


void Read_Compass()
{
  byte temp;
  
  // Point to the first data register
  Wire.beginTransmission(CompassAddress); 
  Wire.send(0x03);  // point to the first data register
  Wire.endTransmission();
    
  // read 6 byte, from address 3 (Data Registers)
  Wire.requestFrom(CompassAddress, 6);
  if (Wire.available() == 6) {
    temp = Wire.receive();    // X axis MSB
    Compass_X = (temp * 256) + Wire.receive();  // plus X axis LSB
    temp = Wire.receive();    // Z axis MSB
    Compass_Z = (temp * 256) + Wire.receive();  // plus Z axis LSB
    temp = Wire.receive();    // Y axis MSB
    Compass_Y = (temp * 256) + Wire.receive();  // plus Y axis LSB
    //Just confirming that the axis order is X Z Y (ie NOT X Y Z) 
  }
  
  // Incorrent number of returned bytes
  else
  {
    Serial.println("Recieving incorrect amount of bytes");
    while(Wire.available()) {
    temp = Wire.receive();
    Serial.print("data byte = ");
    Serial.println(temp, DEC);    //print the returned number as a decimal
    }
  }
}

void readAccel() {
  uint8_t howManyBytesToRead = 6;
  readFrom( DATAX0, howManyBytesToRead, _buff); //read the acceleration data from the ADXL345

  // each axis reading comes in 10 bit resolution, ie 2 bytes.  Least Significat Byte first!!
  // thus we are converting both bytes in to one int
  int x = (((int)_buff[1]) << 8) | _buff[0];   
  int y = (((int)_buff[3]) << 8) | _buff[2];
  int z = (((int)_buff[5]) << 8) | _buff[4];
  Serial.print("x: ");
  Serial.print( x );
  Serial.print(" y: ");
  Serial.print( y );
  Serial.print(" z: ");
  Serial.println( z );
}


