/*
  ==============================================================================

    DrawArray.cpp
    Created: 30 Nov 2017 1:50:13pm
    Author:  USER

  ==============================================================================
*/

#include "DrawArray.h"

DrawArray::DrawArray() : lineThickness(1.0f), lineColour(Colours::white), backgroundColour(Colours::black),
gridColour(Colours::white), labelColour(Colours::white),
maxViewY(50.0f), minViewY(-50.0f), isXAxisLog(false), isXLimitSet(false), isYLimitSet(false), fontName("Calibri"),
fontSize(12.0f), gridOpacity(0.4f)
{

}

DrawArray::~DrawArray()
{

}

void DrawArray::setLineColour(Colour newColour)
{
	lineColour = newColour;
}

void DrawArray::setBackGroundColour(Colour newColour)
{
	backgroundColour = newColour;
}

void DrawArray::setXAxisLog()
{
	isXAxisLog = true;
}

void DrawArray::plot(vector<double> newYAxisData)
{
	yAxisData = newYAxisData;
	xAxisData.resize(yAxisData.size());
	
	for (int i = 0; i < (int)xAxisData.size(); ++i)
	{		
		xAxisData[i] = (double)i;
	}

	plot(xAxisData, yAxisData);
}

void DrawArray::plot(vector<double> newXAxisData, vector<double> newYAxisData)
{
	yAxisData = newYAxisData;
	xAxisData = newXAxisData;

	if(!isXLimitSet)
		setXLimit((float)xAxisData.front(), (float)xAxisData.back());

	if (isXAxisLog)
	{
		for (int i = 0; i < (int)xAxisData.size(); ++i)
		{
			if (xAxisData[i] != 0.0)
				xAxisData[i] = log10((double)xAxisData[i]);
		}
	}

	repaint();
}

void DrawArray::setYLimit(float min, float max)
{
	isYLimitSet = true;
	minViewY = min;
	maxViewY = max;
	repaint();
}

void DrawArray::setXLimit(float min, float max)
{
	isXLimitSet = true;
	minViewX = min;
	maxViewX = max;
	if(isXAxisLog)
	{
		if (minViewX != 0.0f)
			minViewX = log10f(min);

		if (maxViewX != 0.0f)
			maxViewX = log10f(max);
	}

	repaint();
}

void DrawArray::setXGrid(vector<double> newXGrids)
{
	xGrid = newXGrids;

	if (isXAxisLog)
	{
		for (int i = 0; i < (int)xGrid.size(); ++i)
		{
			if (xGrid[i] != 0.0)
				xGrid[i] = log10(xGrid[i]);
		}
	}
	repaint();
}

void DrawArray::setXLabel(vector<double> newXLabels)
{
	xLabel = newXLabels;
	repaint();
}

void DrawArray::setYGrid(vector<double> newYGrids)
{
	yGrid = newYGrids;
	repaint();
}

void DrawArray::setYLabel(vector<double> newYLabels)
{
	yLabel = newYLabels;
	repaint();
}


void DrawArray::paint(Graphics& g)
{
	g.fillAll(backgroundColour);
	float compHeight = (float)getHeight();
	float compWidth = (float)getWidth();
	int arraySize = (int)xAxisData.size();

	if (arraySize > 0)
	{
		g.setColour(lineColour);
		for (int i = 0; i < arraySize - 1; ++i)
		{
			g.drawLine(jmap((float) xAxisData[i], minViewX, maxViewX, 0.0f, compWidth),
				jmap((float)yAxisData[i], minViewY, maxViewY, compHeight, 0.0f),
				jmap((float)xAxisData[i + 1], minViewX, maxViewX, 0.0f, compWidth),
				jmap((float)yAxisData[i + 1], minViewY, maxViewY, compHeight, 0.0f),
				lineThickness);
		}
	}

	// Draw Grids
	drawGrids(g);

	// Draw Labels
	drawLabels(g);

}

void DrawArray::drawGrids(Graphics &g)
{
	float compHeight = (float)getHeight();
	float compWidth = (float)getWidth();

	// Draw X Grid
	if (xGrid.size() > 0)
	{
		g.setColour(gridColour.withAlpha(0.5f));
		for (int i = 0; i < (int)xGrid.size(); ++i)
		{
			g.drawLine(jmap((float)xGrid[i], minViewX, maxViewX, 0.0f, compWidth),
				jmap((float)minViewY, minViewY, maxViewY, compHeight, 0.0f),
				jmap((float)xGrid[i], minViewX, maxViewX, 0.0f, compWidth),
				jmap((float)maxViewY, minViewY, maxViewY, compHeight, 0.0f),
				gridOpacity);
		}
	}

	// Draw Y Grid
	if (yGrid.size() > 0)
	{
		g.setColour(gridColour.withAlpha(0.5f));
		for (int i = 0; i < (int)yGrid.size(); ++i)
		{
			g.drawLine(jmap(minViewX, minViewX, maxViewX, 0.0f, compWidth),
				jmap((float)yGrid[i], minViewY, maxViewY, compHeight, 0.0f),
				jmap(maxViewX, minViewX, maxViewX, 0.0f, compWidth),
				jmap((float)yGrid[i], minViewY, maxViewY, compHeight, 0.0f),
				gridOpacity);
		}
	}
}

void DrawArray::drawLabels(Graphics& g)
{
	float compHeight = (float)getHeight();
	float compWidth = (float)getWidth();
	float opacity = 0.5f;

	// Draw X Label
	if (xLabel.size() > 0)
	{
		g.setColour(labelColour.withAlpha(opacity));
		g.setFont(Font(fontName, fontSize, Font::plain));
		String label;
		float yOffset = 5.0f;
		float xOffset = 3.0f;
		for (int i = 0; i < (int)xLabel.size(); ++i)
		{
			Rectangle<float> drawText;
			if (!isXAxisLog)
				drawText = { jmap((float)xLabel[i], minViewX, maxViewX, 0.0f, compWidth) + xOffset, yOffset, compWidth, compHeight };
			else
				drawText = { jmap(log10f((float)xLabel[i]), minViewX, maxViewX, 0.0f, compWidth) + xOffset, yOffset, compWidth, compHeight };

			label = String(xLabel[i]);

			if (xLabel[i] >= 1000.0)
			{
				label = String(xLabel[i] / 1000.0) + "k";
			}

			g.drawText(String(label), drawText, Justification::topLeft, false);

		}
	}

	// Draw Y Label
	if (yLabel.size() > 0)
	{
		g.setColour(labelColour.withAlpha(opacity));
		g.setFont(Font(fontName, fontSize, Font::plain));
		String label;
		float yOffset = 1.0f;
		float xOffset = 5.0f;
		for (int i = 0; i < (int)yLabel.size(); ++i)
		{
			Rectangle<float> drawText;			
			drawText = {xOffset, jmap((float)yLabel[i], minViewY, maxViewY, compHeight, 0.0f) + yOffset, compWidth, compHeight };			
			label = String(yLabel[i]);
			if (yLabel[i] > 0)
				label = "+" + label;

			g.drawText(String(label), drawText, Justification::topLeft, false);
		}
	}
}

void DrawArray::resized()
{

}

