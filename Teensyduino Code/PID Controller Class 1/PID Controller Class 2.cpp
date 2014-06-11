/*
* This is my first cut at a PID contoller class
*
* TODO: Look to remove the "Gain" variable
*       Rename the xxxGain variables to Kxxx
*       Incorporate some overflow definitions for Constructor and Calc routines
*
* REF:  http://en.wikipedia.org/wiki/PID_controller
*       http://www.drdobbs.com/embedded-c/184403292?pgno=3
*
*
*/

class PidController {
private:
    // Define Term variables
    double   Proportional,
             Integral,
             Differential;

    // Define Gain variables
    double   ProportionalGain,
             IntegralGain,
             DifferentialGain;
             
    double   IntegralLimit;

    double   SetPoint,
             Gain;
             
    unsigned long DifferentialCycle,
                  DifferentialCycleCount;
    
    double   ErrorSum,
             LastError;
protected:
    // Constructor and Destructor
    
    //TODO: Need to better define these to accept multiple overflow forms
    PidController( double P, double I, double D, double IL );
    
    ~PidController();

public:
    // Get values routines    
    double GetProportional() { return Proportional; }
    double GetIntegral() { return Integral; }
    double GetDifferential() { return Differential; }
    
    double GetProportionalGain() { return ProportionalGain; }
    double GetIntegralGain() { return IntegralGain; }
    double GetDifferentialGain() { return DifferentialGain; }

    double GetIntegralLimit() { return IntegralLimit; }
    double GetDifferentialCycle() { return DifferentialCycle; }
    double GetSetpoint() { return Setpoint; }
  
    // Set values routines
    void SetProportional( double PT ) { Proportional = PT; }
    void SetIntegral( double IT ) { Integral = IT; }
    void SetDifferential( double DT ) { Differential = DT; }

    void SetIntegralLimit( double IL ) { IntegralLimit = IL; }
    void SetDifferentialCycle( unsigned DC ) { DifferentialCycle = DC; }
    void SetSetpoint( double SP ) { Setpoint = SP; }
    
    // Calculation and update routines
    double CalculateGain( double Position, double TimeDelta );
    double CalculateGain_v1( double Position );
    double CalculateGain_Orig( double Position );
};

// Constructor
PidController:PidController (double kP, double kI, double kD, double IL) {
	ProportionalGain = kP;
	IntergralGain = kI;
	DifferentialGain = kD;
	IntergralLimit = IL;

			  
}

// Destructor 
PidController:~PidController () {};

// Calaculate Gain - My code - v2 - includes time period 
// TODO: Validate TimeDelta or allow for oveeflow of TimeDelta presence
double PidController::CalculateGain( double Position, double TimeDelta )
{
    double Error = Setpoint - Position;

    Proportional = Error;

    Integral = Integral + Error * TimeDelta;
    if( fabs( Integral ) > IntegralLimit ) {
       if( Integral => 0 )
           Integral = IntegralLimit;
       else
           Integral = -IntegralLimit;
       }

    Differential = ( Error - LastError ) / TimeDelta;

    LastError = Error;

    Gain = (Proportional * ProportionalGain) + (Intergral * IntegralGain) + (Differential * DifferentialGain);
    return Gain;
}


// Calaculate Gain - My code - v1
// TODO: Remove this routine when possible
double PidController::CalculateGain_v1( double position )
{
    double error = Setpoint - position;
    ErrorSum += error;

    Proportional = error;

    Integral = ErrorSum;
    if( fabs( Integral ) > IntegralLimit ) {
       if( Integral > 0 )
           Integral = IntegralLimit;
       else
           Integral = -IntegralLimit;
       ErrorSum -= error;
       }

    if( ++DifferentialCycleCount >= DifferentialCycle ) {
       Differential =  error - LastError;
       DifferentialCycleCount = 0;
       }
       
    LastError = error;

    Gain = (Proportional * ProportionalGain) + (Intergral * IntegralGain) + (Differential * DifferentialGain);
    return Gain;
}

// Calaculate Gain - Original code
// TODO: Remove this routine when possible
double PidController::CalculateGain_Orig( double position )
{
    double error = Setpoint - position;
    ErrorSum += error;
    ProportionalGain = Proportional * error;
    IntegralGain = Integral * ErrorSum;
    if( fabs( IntegralGain ) > IntegralLimit ) {
       if( IntegralGain > 0 )
           IntegralGain = IntegralLimit;
       else
           IntegralGain = -IntegralLimit;
       ErrorSum -= error;
       }
    if( ++DifferentialCycleCount >= DifferentialCycle ) {
       DifferentialGain =  Differential * ( error - LastError );
       DifferentialCycleCount = 0;
       }
    LastError = error;
    Gain = ProportionalGain + IntegralGain + DifferentialGain;
    return Gain;
}

void main () {
	PIDController PIDX;
	int x,y,z;

	PIDX(1, 1, 1, 100);

	for(x=0; x<100; x++) {
		COUT << PIDX.CalaculateGain(x,1);

	}

}