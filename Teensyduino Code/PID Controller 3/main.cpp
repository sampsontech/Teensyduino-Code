#include <iostream>
#include <cmath>

#include "PID Controller Class 2.hpp"

using namespace std;

void main () {

	PIDController PIDX (1, 1, 1, 100);

	int x;

	cout << "Start\n";

	for (x=0; x<100; x++) {
		cout << x << "/n";
		cout << PIDX.CalculateGain(x, 1);
	}
}