/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Teensy++ 2.0, Platform=avr, Package=teensy
*/

#define __AVR_AT90usb1286__
#define __AVR_AT90USB1286__
#define ARDUINO 101
#define ARDUINO_MAIN
#define __AVR__
#define TEENSYDUINO 118
#define __cplusplus
extern "C" void __cxa_pure_virtual() {;}

//
//
double DSF_Ave2(double I);
double DSF_Ave3(double I);
double DSF_LPF(double I, double F);

#include "C:\Users\david\Desktop\Teensy Software\arduino-1.0.5-r2\hardware\teensy\cores\teensy\arduino.h"
#include "S:\Software_GitHub_Sync\Teensyduino-Code\Accel_Distance_1\Accel_Distance_1.ino"
#include "S:\Software_GitHub_Sync\Teensyduino-Code\Accel_Distance_1\KalmanFilter_Class_2.cpp"
#include "S:\Software_GitHub_Sync\Teensyduino-Code\Accel_Distance_1\NoiseDeadBandFilter_Class.cpp"
#include "S:\Software_GitHub_Sync\Teensyduino-Code\Accel_Distance_1\SparkFun_9DOF_6_Class.cpp"
