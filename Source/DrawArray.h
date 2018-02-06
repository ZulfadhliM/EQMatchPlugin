/*
  ==============================================================================

    DrawArray.h
    Created: 30 Nov 2017 1:50:13pm
    Author:  USER

  ==============================================================================
*/

#ifndef DRAWARRAY_H_INCLUDED
#define DRAWARRAY_H_INCLUDED

#include "JuceHeader.h"
#include <vector>
using namespace std;

class DrawArray : public Component
{
public:
	DrawArray();
	~DrawArray();
	
	void setLineColour(Colour newColour);
	void setBackGroundColour(Colour newColour);
	void setXAxisLog();
	void setYLimit(float min, float max);
	void setXLimit(float min, float max);
	
	void plot(vector<double> newYAxisData);
	void plot(vector<double> newXAxisData, vector<double> newYAxisData);
	void setXGrid(vector<double> newXGrids);
	void setXLabel(vector<double> newXLabels);
	void setYGrid(vector<double> newYGrids);
	void setYLabel(vector<double> newYLabels);
	void paint(Graphics &g) override;
	void resized() override;

private:
	Colour lineColour, gridColour, labelColour, backgroundColour;
	String fontName;
	vector<double> yAxisData, xAxisData, xGrid, xLabel, yGrid, yLabel;
	bool isXAxisLog, isXLimitSet, isYLimitSet;
	float lineThickness, gridOpacity, fontSize, maxViewY, minViewY, maxViewX, minViewX;

	void drawGrids(Graphics &g);
	void drawLabels(Graphics &g);
};

#endif  // DRAWARRAY_H_INCLUDED
