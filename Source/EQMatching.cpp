/*
  ==============================================================================

    EQMatching.cpp
    Created: 29 Nov 2017 3:10:03pm
    Author:  USER

  ==============================================================================
*/

#include "EQMatching.h"

//==============================================================================

EQMatching::EQMatching(vector<double> _inputSignal, vector<double> _targetSignal, int _order): inputSignal(_inputSignal), 
targetSignal(_targetSignal), bSize(_order), aSize(_order), samplingRate(44100), frameSizeInSeconds(1)
{
	// Make the length of the input and target signals the same
	makeSameLength();

	// Sum each signal into one smaller frames. This will make computation
	// a whole lot more faster!
	inputSignal = sumFrames(inputSignal, samplingRate * frameSizeInSeconds);
	targetSignal = sumFrames(targetSignal, samplingRate * frameSizeInSeconds);

	// Append zeros according to the order of the filter
	inputSignal.insert(inputSignal.begin(), bSize, 0.0);
	targetSignal.insert(targetSignal.begin(), aSize, 0.0);

}

void EQMatching::setSamplingRate(int newRate)
{
	samplingRate = newRate;
}

void EQMatching::makeSameLength()
{
	// The length of the input and target signals must be the same
	// Therefore, the signal with shorter length is resized
	// to the other signal's length.
	// If size is the same, do nothing

	if (inputSignal.size() > targetSignal.size())
	{
		targetSignal.resize(inputSignal.size());
	}
	else if (inputSignal.size() < targetSignal.size())
	{
		inputSignal.resize(targetSignal.size());
	}
}

vector<double> EQMatching::sumFrames(vector<double> signal, int frameSize)
{
	// Get number of frames in the signal
	int totalFrames = (int)ceil((int)signal.size() / frameSize);

	// Adds zero if the last frame is shorter than the frameSize
	int numberOfZerosToAdd = totalFrames * frameSize - (int)signal.size();
	for (int i = 0; i < numberOfZerosToAdd; ++i)
		signal.push_back(0.0);

	// Initialise vector for the result
	vector<double> output(frameSize);

	// Add each frame into one
	for (int i = 0; i < totalFrames; ++i)
	{
		for (int sample = 0; sample < frameSize; ++sample)
		{
			output[sample] += signal[i * frameSize + sample];
		}
	}

	return output;
}

MatrixXd EQMatching::getOperationMatrix()
{
	/*

	Creates the operation matrix. The operation matrix
	contains shifted versions of input and target
	signals which resembles the filtering process

	*/

	int rowSize = (int)targetSignal.size() - aSize;
	int colSize = bSize + aSize + 1;

	MatrixXd mat(rowSize, colSize);

	for (int i = 0; i < rowSize; ++i)
	{
		for (int j = 0; j < bSize + 1; ++j)
		{
			mat(i, j) = inputSignal[bSize - j + i];
		}
	}

	for (int i = 0; i < rowSize; ++i)
	{
		for (int j = bSize + 1; j < colSize; ++j)
		{
			mat(i, j) = -targetSignal[aSize + bSize - j + i];
		}
	}

	return mat;
}

vector<double> EQMatching::getCoefficients()
{
	/*
	Solve in a least squares sense
	coeff = inv(Xt X) X' y'
	*/

	// Solve inv(Xt X)
	MatrixXd X = getOperationMatrix();
	MatrixXd Xt = X.transpose();
	MatrixXd XtX = Xt * X;
	MatrixXd invXtX = XtX.pinv();

	// Solve X' y'
	targetSignal.erase(targetSignal.begin(), targetSignal.begin() + aSize);
	MatrixXd y(targetSignal.size(), 1);

	for (int i = 0; i < (int)targetSignal.size(); ++i)
	{
		y(i,0) = targetSignal[i];
	}
	
	MatrixXd Xty = Xt * y;

	// Get coefficient matrix
	MatrixXd coeffMatrix = invXtX * Xty;

	// Convert for matrix to vector
	vector<double> coeff(bSize + aSize + 2);

	for (int i = 0; i < bSize + 1; ++i)
	{
		coeff[i] = coeffMatrix(i,0);
	}

	// The first coefficient of the denominator must be 1
	coeff[bSize + 1] = 1.0;

	for (int i = bSize + 2; i < (int)coeff.size(); ++i)
	{
		coeff[i] = coeffMatrix(i - 1, 0);
	}
	

	return coeff;

}

