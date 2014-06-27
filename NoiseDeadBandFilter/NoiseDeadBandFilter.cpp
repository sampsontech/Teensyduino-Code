/*
* Noise Dead Band Filter Class
*
* Library for a simple Noise Dead Band filter
*
* Requirements: -
*
* TODO: - 
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

if data > RU then 
  // slide the dead band range up

if data < RL then 
  // slide the dead band range up

CV = (RU - RL) / 2 + RL;

      return CV;
    }

    double GetRU() { return RU; }
    double GetRL() { return RL; }

    void SetRange(double u, double l) {
      RU = u;
      RL = l;
    }


private:
    double RU; // Range Upper
    double RL; // Range Lower

    double CV; // Current Value
    double LV; // Last Value
};

#endif    // NOISE_DEAD_BAND_FILTER_CLASS
