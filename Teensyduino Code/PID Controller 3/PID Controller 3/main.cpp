#include <ostream>
#include <cmath>

#include "PID Controller Class 2.cpp"

void main () {
	PIDController PIDX (1, 1, 1, 100);

	int x,y,z;

	cout << "Start\n";

	for (x=0; x<100; x++) {
		cout << x << "/n";
		cout << PIDX.CalculateGain(x, 1);
	}
}