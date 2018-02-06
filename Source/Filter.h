/*
  ==============================================================================

    Filter.h
    Created: 26 Nov 2017 5:12:33pm
    Author:  USER

  ==============================================================================
*/

#ifndef FILTER_H_INCLUDED
#define FILTER_H_INCLUDED

#include <vector>
using namespace std;

class Filter
{
public:
	
	Filter();
	~Filter();

	// Clears the delay data
	void reset();

	// Sets the filter with coefficients.
	// Notes: For now it works as an IIR filter.
	//		  FFT convolution works best for FIR filters.
	void setCoefficients(vector<double> _b, vector<double> _a);

	// Filter the signal using Direct Form II Transposed
	double filter(double currentSample);
	
	// Return the impulse response of the filter
	vector<double> getImpulseResponse();

	// Return the frequency response of the filter
	vector<double> getFrequencyResponse();

	// Return a vector that contains the frequency for each bin
	vector<double> getFrequency(double sampleRate);

	// Future improvements: creates low/high/bandpass coefficients

	// void makeLowPass(double cutOffFreq, double Q)
	// void makeHighPass(double cutOffFreq, double Q)
	// void makeBandPass(double cutOffFreq, double Q)

private:
	int filterSize, fftSize;
	const int impulseResponseSize;
	vector<double> delayData, b, a;
};

#endif  // FILTER_H_INCLUDED
