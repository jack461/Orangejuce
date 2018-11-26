/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class OrangejuceAudioProcessor;
//==============================================================================
/**
*/
class OrangejuceAudioProcessorEditor  : public AudioProcessorEditor,
                                        public Timer
{
public:
    enum
    {
        paramControlHeight = 24,
        paramLabelWidth    = 100,
        paramSliderWidth   = 340
    };

    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

    OrangejuceAudioProcessorEditor (OrangejuceAudioProcessor& p, AudioProcessorValueTreeState& vt);
    ~OrangejuceAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OrangejuceAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;

    Label  allLabels[4];
    Slider  allSliders [4];
    std::unique_ptr<SliderAttachment> l1beginAttachment;
    std::unique_ptr<SliderAttachment> l1sizeAttachment;
    std::unique_ptr<SliderAttachment> l1speedAttachment;
    std::unique_ptr<SliderAttachment> l1volAttachment;

    std::unique_ptr<Slider> newSlider;
    Identifier sliderName;
    long timerCounter;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OrangejuceAudioProcessorEditor)
};
