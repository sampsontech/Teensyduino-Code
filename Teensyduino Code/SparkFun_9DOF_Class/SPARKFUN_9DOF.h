#ifndef SPARKFUN_9DOF_H
#define SPARKFUN_9DOF_H
 
#include <WProgram.h>
 
class SPARKFUN_9DOF {
	
public:
        SPARKFUN_9DOF();				// Class Constructor
        ~SPARKFUN_9DOF();				// Class Destructor
        void init_dev();				// Initialise the device
        void read_dev();				// Read all the sensors
};
 
#endif