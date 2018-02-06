/*
  ==============================================================================

    EQMatching.h
    Created: 29 Nov 2017 3:10:03pm
    Author:  USER

  ==============================================================================
*/

#ifndef EQMATCHING_H_INCLUDED
#define EQMATCHING_H_INCLUDED

#include "JuceHeader.h"
#include <Eigen\Eigen>
using namespace Eigen;
using namespace std;

class EQMatching
{
public:
	/* Initialise with input and target signals and the IIR filter order
	Future improvements: (1) Input and target signals can be in the form of
							 JUCE's AudioSampleBuffer
						 (2) Option to choose FIR filter. For now, it is an
						     IIR filter.
						 (3) Can handle floats as well
	*/
	EQMatching(vector<double> _inputSignal, vector<double> _targetSignal, int _order);

	// Return the optimal filter coefficients. The main processing is done here
	vector<double> getCoefficients();

	// Set sampling rate. By default, it is 44.1 kHz
	void setSamplingRate(int newRate);

private:
	// Build operation matrix
	MatrixXd getOperationMatrix();

	// Sums each frame in the signal into one
	vector<double> sumFrames(vector<double> signal, int frameSize);

	// Make input and target signals the same length
	void makeSameLength();

private:
	int bSize, aSize, order, samplingRate, frameSizeInSeconds;
	vector<double> bEstimates, aEstimates, inputSignal, targetSignal;
};



#endif  // EQMATCHING_H_INCLUDED
