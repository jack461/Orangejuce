/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Utilities.h"

//==============================================================================
OrangejuceAudioProcessorEditor::OrangejuceAudioProcessorEditor (OrangejuceAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
     DBG("**** OrangejuceAudioProcessorEditor-> ");
     // Make sure that before the constructor has finished, you've set the
     // editor's size to whatever you need it to be.
     allLabels[0].setText ("Loop start", dontSendNotification);
     addAndMakeVisible (allLabels[0]);

     allLabels[1].setText ("Loop end", dontSendNotification);
     addAndMakeVisible (allLabels[1]);

     allLabels[2].setText ("Loop speed", dontSendNotification);
     addAndMakeVisible (allLabels[2]);

     addAndMakeVisible (allSliders[0]);
     l1beginAttachment.reset (new SliderAttachment (valueTreeState, "l1begin", allSliders[0]));
     allSliders[0].setTextValueSuffix(" %");
     allSliders[0].setPopupDisplayEnabled (true, false, this);

     addAndMakeVisible (allSliders[1]);
     l1endAttachment.reset (new SliderAttachment (valueTreeState, "l1end", allSliders[1]));
     allSliders[1].setTextValueSuffix(" %");

     addAndMakeVisible (allSliders[2]);
     l1speedAttachment.reset (new SliderAttachment (valueTreeState, "l1speed", allSliders[2]));
     allSliders[2].setColour(Slider::thumbColourId, Colours::lightseagreen);
     
     // Test : this slider is not automatically managed
     newSlider = std::make_unique<Slider>();
     addAndMakeVisible (* newSlider);
     newSlider->setColour(Slider::thumbColourId, Colours::red);
     sliderName = Identifier("newSlider");
     
     setSize (paramSliderWidth + paramLabelWidth, jmax (150, paramControlHeight * 5));
     
     startTimer (1000);
     
     timerCounter = 0;
     // DBG("---> " << timerCounter);
     DBG("**** OrangejuceAudioProcessorEditor<- ");
}

OrangejuceAudioProcessorEditor::~OrangejuceAudioProcessorEditor()
{
    DBG("**** ~OrangejuceAudioProcessorEditor* ");
}

//==============================================================================
void OrangejuceAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
    g.setColour (Colours::orange);
    g.setFont (16.0f);
    g.drawFittedText ("Orange JUCE", getLocalBounds(), Justification::topLeft, 1);
    
}

void OrangejuceAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    auto r = getLocalBounds();

    auto xRect = r.removeFromTop (paramControlHeight); // room for plugin name
        
    xRect = r.removeFromTop (paramControlHeight);
    allLabels[0].setBounds (xRect.removeFromLeft (paramLabelWidth));
    allSliders[0].setBounds (xRect);

    xRect = r.removeFromTop (paramControlHeight);
    allLabels[1].setBounds (xRect.removeFromLeft (paramLabelWidth));
    allSliders[1].setBounds (xRect);

    xRect = r.removeFromTop (paramControlHeight);
    allLabels[2].setBounds (xRect.removeFromLeft (paramLabelWidth));
    allSliders[2].setBounds (xRect);

    xRect = r.removeFromTop (paramControlHeight);
    xRect.removeFromLeft (paramLabelWidth);
    newSlider->setBounds (xRect);
}


void OrangejuceAudioProcessorEditor::timerCallback()
{
    if (processor.presetLoaded)
    {
        timerCounter = long(100*double(valueTreeState.state.getProperty (sliderName, 0)));
        processor.presetLoaded = false;
    }
    else
    {
        valueTreeState.state.setProperty (sliderName, timerCounter/100.0, nullptr);
    }
    newSlider->setValue (timerCounter/100000.0, dontSendNotification);
    if ((++timerCounter & 0xff) == 0)
    {
       DBG("**** OrangejuceAudioProcessorEditor timerCallback");
       
    }
}
