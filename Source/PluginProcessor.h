/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Filter.h"
#include <vector>
using namespace std;

//==============================================================================
/**
*/
class EqmatchingPlugInAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    EqmatchingPlugInAudioProcessor();
    ~EqmatchingPlugInAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

	// Sets the filter coefficients for LEFT and RIGHT channel
	void setLeftFilterCoefficients(vector<double> newB, vector<double> newA);
	void setRightFilterCoefficients(vector<double> newB, vector<double> newA);

	// Toggles when the capture buttons are clicked
	void toggleInputRecordingStatus();
	void toggleTargetRecordingStatus();

	// Return the captured signals
	vector<double> getRecordedInputLeftChannel();
	vector<double> getRecordedTargetLeftChannel();
	vector<double> getRecordedInputRightChannel();
	vector<double> getRecordedTargetRightChannel();

	int getRecordedInputSize();
	int getRecordedTargetSize();
	void clearRecordedInput();
	void clearRecordedTarget();
	void bypassFilter(bool _isBypassOn);

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
	Filter leftFilter, rightFilter;
	vector<double> bLeft, aLeft, bRight, aRight, leftInputBuffer, leftTargetBuffer, rightInputBuffer, rightTargetBuffer;
	bool isInputRecording, isTargetRecording, isBypassOn;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EqmatchingPlugInAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
