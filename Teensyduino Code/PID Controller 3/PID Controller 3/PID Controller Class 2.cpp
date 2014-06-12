/*
* PID Contoller Class Routines
* 
* This is my first cut at a PID contoller class
*
* TODO: Look to remove the "Gain" variable
*       Rename the xxxGain variables to Kxxx
*       Incorporate some overflow definitions for Constructor and Calc routines
*
* REF:  http://en.wikipedia.org/wiki/PID_controller
*       http://www.drdobbs.com/embedded-c/184403292?pgno=3
*
*/

// Constructor
PIDController::PIDController (double kP, double kI, double kD, double IL) {
	ProportionalGain = kP;
	IntegralGain = kI;
	DifferentialGain = kD;
	IntegralLimit = IL;  
}

// Destructor 
PIDController::~PIDController () {};

// Calaculate Gain - My code - v2 - includes time period 
// TODO: Validate TimeDelta or allow for oveeflow of TimeDelta presence
double PIDController::CalculateGain( double Position, double TimeDelta )
{
    double Error = SetPoint - Position;

    Proportional = Error;

    Integral = Integral + Error * TimeDelta;

    if( fabs( Integral ) > IntegralLimit ) {
       if( Integral >= 0 )
           Integral = IntegralLimit;
       else
           Integral = -IntegralLimit;
       }

    Differential = ( Error - LastError ) / TimeDelta;

    LastError = Error;

    Gain = (Proportional * ProportionalGain) + (Integral * IntegralGain) + (Differential * DifferentialGain);
    return Gain;
}
