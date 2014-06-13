/*
* PID Contoller Class
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

#ifndef PIDCONTROLLER_CLASS
#define PIDCONTROLLER_CLASS

#include <cmath>

class PIDController {
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
public:
    
		// Constructor
    // TODO: Need to better define these to accept multiple overflow forms
		PIDController (double kP, double kI, double kD, double IL) {
			ProportionalGain = kP;
			IntegralGain = kI;
			DifferentialGain = kD;
			IntegralLimit = IL;  
		}

		// Destructor 
		~PIDController () {}

/*
    // Calculation and update routines
    double CalculateGain( double Position, double TimeDelta );
    double CalculateGain_v1( double Position );
    double CalculateGain_Orig( double Position );
*/
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

    // Get values routines    
    double GetProportional() { return Proportional; }
    double GetIntegral() { return Integral; }
    double GetDifferential() { return Differential; }
    
    double GetProportionalGain() { return ProportionalGain; }
    double GetIntegralGain() { return IntegralGain; }
    double GetDifferentialGain() { return DifferentialGain; }

    double GetIntegralLimit() { return IntegralLimit; }
    double GetDifferentialCycle() { return DifferentialCycle; }
    double GetSetPoint() { return SetPoint; }
  
    // Set values routines
    void SetProportional( double PT ) { Proportional = PT; }
    void SetIntegral( double IT ) { Integral = IT; }
    void SetDifferential( double DT ) { Differential = DT; }

    void SetIntegralLimit( double IL ) { IntegralLimit = IL; }
    void SetDifferentialCycle( unsigned DC ) { DifferentialCycle = DC; }
    void SetSetPoint( double SP ) { SetPoint = SP; } 
};

#endif	//PIDCONTROLLER_CLASS
