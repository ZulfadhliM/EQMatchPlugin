/*
  ==============================================================================

    Filter.cpp
    Created: 26 Nov 2017 5:12:33pm
    Author:  USER

  ==============================================================================
*/

#include "Filter.h"
#include "JuceHeader.h"
//==============================================================================

Filter::Filter(): impulseResponseSize(8192)
{
}

Filter::~Filter()
{
}

void Filter::reset()
{
	if (!delayData.empty())
	{
		delayData.clear();
	}

	delayData.resize(filterSize, 0.0);
}

void Filter::setCoefficients(vector<double> _b, vector<double> _a)
{
	b = _b;
	a = _a;
	if (b.size() > a.size())
	{
		filterSize = (int)b.size();
		a.resize(b.size(), 0.0);
	}
	else
	{
		filterSize = (int)a.size();
		b.resize(a.size(), 0.0);
	}
	filterSize -= 1;

	reset();
}

double Filter::filter(double currentSample)
{
	// Filter using Direct Form II Transposed
	// advantage is that it requires only one delay line

	double output = 0.0;
	output += delayData[0] + b[0] * currentSample;

	for (int i = 0; i < filterSize - 1; ++i)
	{
		delayData[i] = delayData[i + 1] + b[i + 1] * currentSample - a[i + 1] * output;
	}

	delayData[filterSize - 1] = b[filterSize] * currentSample - a[filterSize] * output;

	return output;
}

vector<double> Filter::getImpulseResponse()
{
	reset();

	vector<double> impulseResponse(impulseResponseSize);
	impulseResponse[0] = filter(1.0);

	for (int i = 1; i < impulseResponseSize; ++i)
	{
		impulseResponse[i] = filter(0.0);
	}

	return impulseResponse;
}

vector<double> Filter::getFrequencyResponse()
{
	vector<double> impulseResponse = getImpulseResponse();
	float* fftImpulseResponse = new float[impulseResponseSize];
	vector<double> freqResponse(impulseResponseSize / 4);

	for (int i = 0; i < impulseResponseSize; ++i)
	{
		fftImpulseResponse[i] = (float) impulseResponse[i];
	}

	fftSize = impulseResponseSize / 2;
	FFT fft((int)log2(fftSize), false);
	fft.performFrequencyOnlyForwardTransform(fftImpulseResponse);

	for (int i = 0; i < impulseResponseSize / 4; ++i)
	{
		freqResponse[i] = 10.0 * log((double) fftImpulseResponse[i]);
	}

	delete[] fftImpulseResponse;

	return freqResponse;
}

vector<double> Filter::getFrequency(double sampleRate)
{
	vector<double> freqs(fftSize / 2);
	for (int i = 0; i < (int)freqs.size(); ++i)
		freqs[i] = sampleRate / (double)fftSize * (double)i;
	return freqs;
}


