#include <iostream>

#include "PID Controller Class 4.cpp"

using namespace std;

void main () {

	PIDController PIDX (1, 1, 1, 999999);
	PIDX.SetSetPoint(1000);

	int x;
	double current = 0;

	cout << "Start" << endl;

	for (x=0; x<100; x++) {
		current += PIDX.CalculateGain(current, 1);
		cout << "# " << x;
		cout << " - Current = " << current;
		cout << " - SetPoint = " << PIDX.GetSetPoint();
		cout << " - P = " << PIDX.GetProportional();
		cout << " - I = " << PIDX.GetIntegral();
		cout << " - D = " << PIDX.GetDifferential();
		cout << endl;
	}
}