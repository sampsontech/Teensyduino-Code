#include <iostream>

#include "PID Controller Class 4.cpp"

using namespace std;

void main () {

	PIDController PIDX (0.8, 0.5, 0.5, 500);
	PIDX.SetSetPoint(1000);

	int x;
	double current = 1;

	cout << "Start" << endl;

	for (x=0; x<100; x++) {
		current += 0.53 * PIDX.CalculateGain(current, 1);
		cout << "# " << x;
		cout << " - Current = " << current;
		cout << " - SetPoint = " << PIDX.GetSetPoint();
		cout << " - P = " << PIDX.GetProportional();
		cout << " - I = " << PIDX.GetIntegral();
		cout << " - D = " << PIDX.GetDifferential();
		cout << endl;
	}
}