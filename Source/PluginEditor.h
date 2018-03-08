/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "EQMatching.h"
#include "DrawArray.h"
#include <vector>
using namespace std;

//==============================================================================
/**
*/
class EqmatchingPlugInAudioProcessorEditor  : public AudioProcessorEditor, public Button::Listener
{
public:
    EqmatchingPlugInAudioProcessorEditor (EqmatchingPlugInAudioProcessor&);
    ~EqmatchingPlugInAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void buttonClicked(Button* button) override;

private:
	void createCaptureButton(ImageButton* button, Label* label);
	void createClearButton(ImageButton* button, Label* label);
	void changeCaptureButtonToStop(ImageButton* button, Label* label);
	void defaultCaptureButton(ImageButton* button, Label* label);
	void createHeaderLabel(Label* label, String headerName);
	void hasSignal(ImageButton* button, Label* label, int bufferSize);
	void bypassButtonChanged();
	void matchEQ(); // Does the EQ Matching technique when the button is clicked

private:
    EqmatchingPlugInAudioProcessor& processor;
	TextButton  eqMatchButton;
	Label show, captureInputLabel, captureTargetLabel, clearInputLabel, clearTargetLabel, bypassButtonLabel, inputHeader, targetHeader, filterHeader;
	ImageButton captureInputButton, captureTargetButton, clearInputButton, clearTargetButton, bypassButton;
	String fontName;
	DrawArray drawLeftFreqResp, drawRightFreqResp;
	Filter filter;
	bool isInputRecording, isTargetRecording, isMatching;
	int sampleRate, boxX, boxY, boxWidth, boxHeight;
	float minY, maxY, minX, maxX;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EqmatchingPlugInAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
