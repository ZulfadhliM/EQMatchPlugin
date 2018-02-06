/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
EqmatchingPlugInAudioProcessor::EqmatchingPlugInAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ), isInputRecording(false), isTargetRecording(false), isBypassOn(true)
#endif
{
	leftFilter.reset();
	rightFilter.reset();
}

EqmatchingPlugInAudioProcessor::~EqmatchingPlugInAudioProcessor()
{
}

//==============================================================================
const String EqmatchingPlugInAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EqmatchingPlugInAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool EqmatchingPlugInAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double EqmatchingPlugInAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EqmatchingPlugInAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int EqmatchingPlugInAudioProcessor::getCurrentProgram()
{
    return 0;
}

void EqmatchingPlugInAudioProcessor::setCurrentProgram (int index)
{
}

const String EqmatchingPlugInAudioProcessor::getProgramName (int index)
{
    return String();
}

void EqmatchingPlugInAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void EqmatchingPlugInAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
}

void EqmatchingPlugInAudioProcessor::setLeftFilterCoefficients(vector<double> newB, vector<double> newA)
{
	bLeft = newB;
	aLeft = newA;
	leftFilter.setCoefficients(bLeft, aLeft);
}

void EqmatchingPlugInAudioProcessor::setRightFilterCoefficients(vector<double> newB, vector<double> newA)
{
	bRight = newB;
	aRight = newA;
	rightFilter.setCoefficients(bRight, aRight);
}

void EqmatchingPlugInAudioProcessor::toggleInputRecordingStatus()
{
	isInputRecording = !isInputRecording;
}

void EqmatchingPlugInAudioProcessor::toggleTargetRecordingStatus()
{
	isTargetRecording = !isTargetRecording;
}

vector<double> EqmatchingPlugInAudioProcessor::getRecordedInputLeftChannel()
{
	return leftInputBuffer;
}


vector<double> EqmatchingPlugInAudioProcessor::getRecordedTargetLeftChannel()
{
	return leftTargetBuffer;
}

vector<double> EqmatchingPlugInAudioProcessor::getRecordedInputRightChannel()
{
	return rightInputBuffer;
}

vector<double> EqmatchingPlugInAudioProcessor::getRecordedTargetRightChannel()
{
	return rightTargetBuffer;
}

void EqmatchingPlugInAudioProcessor::clearRecordedInput()
{
	leftInputBuffer.clear();
	rightInputBuffer.clear();
}

void EqmatchingPlugInAudioProcessor::clearRecordedTarget()
{
	leftTargetBuffer.clear();
	rightTargetBuffer.clear();
}

int EqmatchingPlugInAudioProcessor::getRecordedInputSize()
{
	return (int)leftInputBuffer.size();
}

int EqmatchingPlugInAudioProcessor::getRecordedTargetSize()
{
	return (int)leftTargetBuffer.size();
}

void EqmatchingPlugInAudioProcessor::bypassFilter(bool _isBypassOn)
{
	isBypassOn = _isBypassOn;
}

void EqmatchingPlugInAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EqmatchingPlugInAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void EqmatchingPlugInAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{

    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	// Get current position of the play head
	AudioPlayHead* playHead = getPlayHead();
	AudioPlayHead::CurrentPositionInfo pos;
	playHead->getCurrentPosition(pos);

    // Do audio processing here
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);
		
		if (channel == 0)
		{
			// Process LEFT channel
			for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
			{
				// Filter signal if Bypass button is ON
				if (!isBypassOn)
					channelData[sample] = (float)leftFilter.filter(channelData[sample]);

				// Captures the input signal if button is ON and the play head is moving
				if (isInputRecording && pos.isPlaying)
					leftInputBuffer.push_back((double)channelData[sample]);

				// Captures the target signal if button is ON and the play head is moving
				if (isTargetRecording && pos.isPlaying)
					leftTargetBuffer.push_back((double)channelData[sample]);
			}
		}
		else
		{
			// Process RIGHT channel
			for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
			{
				if (!isBypassOn)
					channelData[sample] = (float)rightFilter.filter(channelData[sample]);

				if (isInputRecording && pos.isPlaying)
					rightInputBuffer.push_back((double)channelData[sample]);

				if (isTargetRecording && pos.isPlaying)
					rightTargetBuffer.push_back((double)channelData[sample]);
			}
		}
    }
}

//==============================================================================
bool EqmatchingPlugInAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* EqmatchingPlugInAudioProcessor::createEditor()
{
    return new EqmatchingPlugInAudioProcessorEditor (*this);
}

//==============================================================================
void EqmatchingPlugInAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void EqmatchingPlugInAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EqmatchingPlugInAudioProcessor();
}
