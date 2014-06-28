/*
* Noise Dead Band Filter Class
*
* Library for a simple Noise Dead Band filter
*
* Requirements: -
*
* TODO: - Create a routine that takes a ref to the Accelerometer and
*         the Axis to use and establish the dead band range
*
* REF: - 
*
* USAGE:
*   NoiseDeadBandFilter NDBF;            // create/declare with default parameters
*
*   NDBF.SetRange(double Upper, double Lower);  //Set the range of the Dead Band
*
*   while (true)
*   {
*       double value = getValueFromSensor();
*       double correctedValue = NDBF.Filter(double value);
*   }
*/

#ifndef NOISE_DEAD_BAND_FILTER_CLASS
#define NOISE_DEAD_BAND_FILTER_CLASS

class NoiseDeadBandFilter {
public:
    NoiseDeadBandFilter() {}
     
    double Filter(double data) {
      LV = CV;

      if(data > RU) { 
        // slide the dead band range up
        RL = (data - RU) + RL;
        RU = data;
      }

      if(data < RL) { 
        // slide the dead band range down
        RU = RU - (RL - data);
        RL = data; 
      }
      
      CV = (RU - RL) / 2 + RL;

      return CV;
    }

    void SetRange(double u, double l) {
      RU = u;
      RL = l;
    }

    double GetRange() { return (RU - RL); }

    double GetRU() { return RU; }

    double GetRL() { return RL; }

private:
    double RU; // Range Upper
    double RL; // Range Lower

    double CV; // Current Value
    double LV; // Last Value
};

#endif    // NOISE_DEAD_BAND_FILTER_CLASS
