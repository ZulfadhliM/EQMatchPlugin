/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
EqmatchingPlugInAudioProcessorEditor::EqmatchingPlugInAudioProcessorEditor (EqmatchingPlugInAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), isInputRecording(false), isTargetRecording(false), isMatching(false),
	minX(20.0f), maxX(20000.0f),
	minY(-30.0f), maxY(30.0f), fontName("Calibri")
{
	createCaptureButton(&captureInputButton, &captureInputLabel);
	createCaptureButton(&captureTargetButton, &captureTargetLabel);
	createClearButton(&clearInputButton, &clearInputLabel);
	createClearButton(&clearTargetButton, &clearTargetLabel);
	createHeaderLabel(&inputHeader, "Input sound");
	createHeaderLabel(&targetHeader, "Target sound");
	createHeaderLabel(&filterHeader, "Filter");

	addAndMakeVisible(eqMatchButton);
	eqMatchButton.addListener(this);
	eqMatchButton.setButtonText("Match");
	eqMatchButton.setColour(TextButton::buttonColourId, Colours::darkgrey);

	addAndMakeVisible(bypassButton);
	bypassButton.addListener(this);
	bypassButton.setImages(false, true, true,
		ImageCache::getFromMemory(BinaryData::power_png, BinaryData::power_pngSize), 1.000f, Colours::lightslategrey,
		Image(), 1.000f, Colours::lightslategrey.brighter(0.3f),
		Image(), 1.000f, Colours::darkgrey);
	bypassButton.setClickingTogglesState(true);
	bypassButton.setToggleState(true, dontSendNotification);
	bypassButton.setEnabled(false);
	addAndMakeVisible(bypassButtonLabel);
	bypassButtonChanged();
	bypassButtonLabel.setFont(Font(fontName, 12.0f, Font::plain));
	bypassButtonLabel.setJustificationType(Justification::centred);

	addAndMakeVisible(drawRightFreqResp);
	drawRightFreqResp.setBackGroundColour(Colour(0x00000000));
	drawRightFreqResp.setLineColour(Colours::red.withAlpha(0.9f));
	drawRightFreqResp.setXAxisLog();
	drawRightFreqResp.setYLimit(minY, maxY);
	drawRightFreqResp.setXLimit(minX, maxX);
	drawRightFreqResp.setAlwaysOnTop(true);

	addAndMakeVisible(drawLeftFreqResp);
	drawLeftFreqResp.setBackGroundColour(Colour(0xff0f0e11));
	drawLeftFreqResp.setLineColour(Colour(0xffbbbbff).withAlpha(0.9f));
	drawLeftFreqResp.setXAxisLog();
	drawLeftFreqResp.setYLimit(minY, maxY);
	drawLeftFreqResp.setXLimit(minX, maxX);
	drawLeftFreqResp.setXGrid({30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0, 200.0, 300.0, 400.0, 500.0, 600.0, 700.0, 800.0, 900.0, 1000.0, 2000.0, 3000.0, 4000.0, 5000.0, 6000.0, 7000.0, 8000.0, 9000.0, 10000.0});
	drawLeftFreqResp.setXLabel({50.0, 100.0, 200.0, 500.0, 1000.0, 2000.0, 5000.0, 10000.0, 15000.0});
	drawLeftFreqResp.setYGrid({ -24.0, -18.0, -12.0, -6.0, 0.0, 6.0, 12.0, 18.0, 24.0 });
	drawLeftFreqResp.setYLabel({ -24.0, -18.0, -12.0, -6.0, 0.0, 6.0, 12.0, 18.0, 24.0 });

	addAndMakeVisible(show);
	show.setBounds(0, 0, 100, 20);
	show.setColour(Label::textColourId, Colours::white);
	

    setSize (800, 400);
}

EqmatchingPlugInAudioProcessorEditor::~EqmatchingPlugInAudioProcessorEditor()
{
}

void EqmatchingPlugInAudioProcessorEditor::createCaptureButton(ImageButton* button, Label* label)
{
	addAndMakeVisible(button);
	addAndMakeVisible(label);
	button->addListener(this);
	defaultCaptureButton(button, label);
}

void EqmatchingPlugInAudioProcessorEditor::defaultCaptureButton(ImageButton* button, Label* label)
{
	button->setImages(false, true, true,
		ImageCache::getFromMemory(BinaryData::capture_png, BinaryData::capture_pngSize), 1.000f, Colours::lightslategrey,
		Image(), 1.000f, Colours::lightslategrey.brighter(0.3f),
		Image(), 1.000f, Colours::lightslategrey);
	label->setText("Capture", dontSendNotification);
	label->setJustificationType(Justification::centred);
	label->setColour(Label::textColourId, Colours::lightslategrey);
	label->setFont(Font(fontName, 12.0f, Font::plain));
}

void EqmatchingPlugInAudioProcessorEditor::changeCaptureButtonToStop(ImageButton* button, Label* label)
{
	Colour stopColour = Colour(0xffe20606);
	button->setImages(false, true, true,
		ImageCache::getFromMemory(BinaryData::stop_png, BinaryData::capture_pngSize), 1.000f, stopColour,
		Image(), 1.000f, stopColour.brighter(0.3f),
		Image(), 1.000f, stopColour);
	label->setText("Stop", dontSendNotification);
	label->setColour(Label::textColourId, stopColour);
}

void EqmatchingPlugInAudioProcessorEditor::hasSignal(ImageButton* button, Label* label, int bufferSize)
{
	if (bufferSize > 0)
	{
		Colour capturedColour = Colours::seagreen.brighter(0.1f);
		button->setImages(false, true, true,
			ImageCache::getFromMemory(BinaryData::captured_png, BinaryData::captured_pngSize), 1.000f, capturedColour,
			Image(), 1.000f, capturedColour.brighter(0.3f),
			Image(), 1.000f, capturedColour);
		label->setText("Captured", dontSendNotification);
		label->setColour(Label::textColourId, capturedColour);
	}
	else
	{
		defaultCaptureButton(button, label);
	}
}

void EqmatchingPlugInAudioProcessorEditor::createClearButton(ImageButton* button, Label* label)
{
	Colour clearColour = Colours::lightslategrey;
	addAndMakeVisible(button);
	addAndMakeVisible(label);
	button->addListener(this);
	button->setImages(false, true, true,
		ImageCache::getFromMemory(BinaryData::clear_png, BinaryData::clear_pngSize), 1.000f, clearColour,
		Image(), 1.000f, clearColour.brighter(0.3f),
		Image(), 1.000f, clearColour.brighter(0.6f));
	label->setText("Clear", dontSendNotification);
	label->setJustificationType(Justification::centred);
	label->setColour(Label::textColourId, clearColour);
	label->setFont(Font(fontName, 12.0f, Font::plain));
}

void EqmatchingPlugInAudioProcessorEditor::createHeaderLabel(Label* label, String headerName)
{
	addAndMakeVisible(label);
	label->setColour(Label::textColourId, Colours::lightslategrey);
	label->setFont(Font(fontName, 14.0f, Font::plain));
	label->setText(headerName, dontSendNotification);
}

void EqmatchingPlugInAudioProcessorEditor::bypassButtonChanged()
{
	if (!bypassButton.getToggleState())
	{
		bypassButtonLabel.setText("On", dontSendNotification);
		bypassButtonLabel.setColour(Label::textColourId, Colours::lightslategrey);
		processor.bypassFilter(false);
	}
	else
	{
		bypassButtonLabel.setText("Bypass", dontSendNotification);
		bypassButtonLabel.setColour(Label::textColourId, Colours::darkgrey);
		processor.bypassFilter(true);
	}
}
//---------------------------------------------------------------------//

void EqmatchingPlugInAudioProcessorEditor::buttonClicked(Button* button)
{
	if (button == &captureInputButton)
	{
		isInputRecording = !isInputRecording;
		processor.toggleInputRecordingStatus();
		if (!isInputRecording)
		{
			hasSignal(&captureInputButton, &captureInputLabel, processor.getRecordedInputSize());
		}
		else
		{
			changeCaptureButtonToStop(&captureInputButton, &captureInputLabel);
		}
	
	}

	if (button == &captureTargetButton)
	{
		isTargetRecording = !isTargetRecording;
		processor.toggleTargetRecordingStatus();

		if (!isTargetRecording)
		{
			hasSignal(&captureTargetButton, &captureTargetLabel, processor.getRecordedTargetSize());
		}
		else
		{
			changeCaptureButtonToStop(&captureTargetButton, &captureTargetLabel);
		}
	}

	if (button == &clearInputButton)
	{
		processor.clearRecordedInput();
		hasSignal(&captureInputButton, &captureInputLabel ,processor.getRecordedInputSize());
	}	

	if (button == &clearTargetButton)
	{
		processor.clearRecordedTarget();
		hasSignal(&captureTargetButton, &captureTargetLabel, processor.getRecordedTargetSize());
	}

	if (button == &eqMatchButton)
	{
		matchEQ();
	}

	if (button == &bypassButton)
	{
		bypassButtonChanged();
	}
}

void EqmatchingPlugInAudioProcessorEditor::matchEQ()
{
	vector<double> leftInputBuffer = processor.getRecordedInputLeftChannel();
	vector<double> leftTargetBuffer = processor.getRecordedTargetLeftChannel();
	vector<double> rightInputBuffer = processor.getRecordedInputRightChannel();
	vector<double> rightTargetBuffer = processor.getRecordedTargetRightChannel();

	int order = 220;
	int bSize = order + 1;

	EQMatching matchLeft(leftInputBuffer, leftTargetBuffer, order);
	vector<double> coeffLeft = matchLeft.getCoefficients();
	vector<double> bEstLeft(coeffLeft.begin(), coeffLeft.begin() + bSize);
	vector<double> aEstLeft(coeffLeft.begin() + bSize, coeffLeft.end());

	EQMatching matchRight(rightInputBuffer, rightTargetBuffer, order);
	vector<double> coeffRight = matchRight.getCoefficients();
	vector<double> bEstRight(coeffRight.begin(), coeffRight.begin() + bSize);
	vector<double> aEstRight(coeffRight.begin() + bSize, coeffRight.end());

	processor.setLeftFilterCoefficients(bEstLeft, aEstLeft);
	processor.setRightFilterCoefficients(bEstRight, aEstRight);

	filter.setCoefficients(bEstLeft, aEstLeft);
	vector<double> freqRespLeft = filter.getFrequencyResponse();
	vector<double> freqOfFreqResp = filter.getFrequency(processor.getSampleRate());
	drawLeftFreqResp.plot(freqOfFreqResp, freqRespLeft);

	filter.setCoefficients(bEstRight, aEstRight);
	vector<double> freqRespRight = filter.getFrequencyResponse();
	drawRightFreqResp.plot(freqOfFreqResp, freqRespRight);

	bypassButton.setEnabled(true);
	bypassButton.setToggleState(true, dontSendNotification);
	bypassButtonChanged();
}

//==============================================================================
void EqmatchingPlugInAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colour(0xff0f0e11).brighter(0.1f));
	g.setColour(Colour(0xff0f0e11));
	g.fillRect(boxX, boxY, boxWidth, boxHeight);
	g.setColour(Colours::lightslategrey);
	int margin = 20;
	g.drawLine(boxX + margin, boxY, boxX + boxWidth - margin, boxY, 2.0f);
}


void EqmatchingPlugInAudioProcessorEditor::resized()
{
	Rectangle<int> area = getLocalBounds().reduced(30);

	int headerHeight = 30;
	int buttonHeight = 25;
	int buttonLabelHeight = 30;
	int buttonWidth = 60;

	Rectangle<int> topArea = area.removeFromTop(area.getHeight() * 0.7);
	{
		drawLeftFreqResp.setBounds(topArea);
		drawRightFreqResp.setBounds(topArea);
	}

	int leftMargin = 20;
	int topMargin = 10;
	Rectangle<int> bottomArea = area.removeFromTop(area.getWidth()).withTrimmedTop(topMargin).withTrimmedLeft(leftMargin);
	boxX = bottomArea.getX() - leftMargin;
	boxY = bottomArea.getY() - topMargin;
	boxWidth = bottomArea.getWidth() + leftMargin;
	boxHeight = bottomArea.getHeight() + topMargin;
	{
		Rectangle<int> referenceArea = bottomArea.removeFromLeft(buttonWidth * 2 + 20);
		{
			inputHeader.setBounds(referenceArea.removeFromTop(headerHeight));

			Rectangle<int> captureInputArea = referenceArea.removeFromTop(buttonHeight);
			{
				captureInputButton.setBounds(captureInputArea.removeFromLeft(buttonWidth));
				clearInputButton.setBounds(captureInputArea.removeFromLeft(buttonWidth));
			}

			Rectangle<int> captureInputLabelArea = referenceArea.removeFromTop(buttonLabelHeight);
			{
				captureInputLabel.setBounds(captureInputLabelArea.removeFromLeft(buttonWidth));
				clearInputLabel.setBounds(captureInputLabelArea.removeFromLeft(buttonWidth));
			}
		}

		Rectangle<int> targetArea = bottomArea.removeFromLeft(buttonWidth * 2 + 20);
		{
			targetHeader.setBounds(targetArea.removeFromTop(headerHeight));

			Rectangle<int> captureTargetArea = targetArea.removeFromTop(buttonHeight);
			{
				captureTargetButton.setBounds(captureTargetArea.removeFromLeft(buttonWidth));
				clearTargetButton.setBounds(captureTargetArea.removeFromLeft(buttonWidth));
			}

			Rectangle<int> captureTargetLabelArea = targetArea.removeFromTop(buttonLabelHeight);
			{
				captureTargetLabel.setBounds(captureTargetLabelArea.removeFromLeft(buttonWidth));
				clearTargetLabel.setBounds(captureTargetLabelArea.removeFromLeft(buttonWidth));
			}
		}

		Rectangle<int> filterArea = bottomArea.removeFromLeft(buttonWidth * 2 + 20);
		{
			filterHeader.setBounds(filterArea.removeFromTop(headerHeight));

			Rectangle<int> filterButtonsArea = filterArea.removeFromTop(buttonHeight);
			{
				eqMatchButton.setBounds(filterButtonsArea.removeFromLeft(buttonWidth));
				bypassButton.setBounds(filterButtonsArea.removeFromLeft(buttonWidth + 10));
			}

			Rectangle<int> filterButtonLabelArea = filterArea.removeFromTop(buttonLabelHeight);
			{
				bypassButtonLabel.setBounds(filterButtonLabelArea.removeFromLeft(buttonWidth * 2).withTrimmedLeft(buttonWidth + 10));
			}		
		}
	}
}
