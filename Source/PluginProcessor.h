/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "VariousStructures.h"
#include "CircFracAudioBuffer.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Utilities.h"


static constexpr float minInfinity { -1010.0 };
static constexpr float minDBValue { -1000.0 };

//==============================================================================
/**
*/
class OrangejuceAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    OrangejuceAudioProcessor();
    ~OrangejuceAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override
    {
        return new OrangejuceAudioProcessorEditor (*this, parameters);
    }
    bool hasEditor() const override                        { return true; }

    //==============================================================================
    const String getName() const override                  { return JucePlugin_Name; }
    bool acceptsMidi() const override                      { return false; }
    bool producesMidi() const override                     { return false; }
    bool isMidiEffect() const override                     { return false; }
    double getTailLengthSeconds() const override           { return 0; }
    //==============================================================================
 
     //==============================================================================
    int getNumPrograms() override                          { return 1; }
    int getCurrentProgram() override                       { return 0; }
    void setCurrentProgram (int) override                  {}
    const String getProgramName (int) override             { return {}; }
    void changeProgramName (int, const String&) override   {}


    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override
    {        
        MemoryOutputStream stream(destData, false);
        parameters.state.writeToStream (stream);
    }

    void setStateInformation (const void* data, int sizeInBytes) override
    {
       ValueTree tree = ValueTree::readFromData (data, size_t (sizeInBytes));
       if (tree.isValid()) {
           parameters.state = tree;
       }
    }

    bool presetLoaded;
private:
    //==============================================================================
    AudioProcessorValueTreeState parameters;
    
    CircFracAudioBuffer cfab;
    ReadCircFracAudioBuffer rca;
    
    float* l1beginParameter  = nullptr;
    float* l1endParameter  = nullptr;
    float* l1speedParameter  = nullptr;
    double smplRate;
    struct CFABCtlBloc looper1 ;
    std::function<String(float)> fptedt[2];
    std::function<float(String)> fptalx[1];

    // Declare a custom volume control vith various steps...
    CustomNormalisableRangeHelper volCtl {new std::vector<float>{
        minInfinity, minInfinity, minDBValue, minDBValue, minDBValue, -800.0, -600.0, -450.0, -340.0,
        -260.0, -200.0, -150.0, -110.0, -80.0, -80.0, -80.0, -60.0, -48.0, -40.0, -40.0, -40.0,
        -34.0, -28.0, -22.0, -20.0, -20.0, -20.0, -18.0, -15.0, -12.0, -12.0, -12.0, -10.0,
        -8.0, -6.0, -6.0, -6.0, -5.0, -4.5, -4.0, -3.0, -3.0, -3.0, -2.5, -2.0, -1.5, -1.0, -1.0, -1.0,
        -0.8, -0.6, -0.4, -0.3, -0.2, -0.15, -0.1, -0.1, -0.1, -0.05, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.05, 0.1, 0.1, 0.1, 0.15, 0.2, 0.3, 0.4, 0.6, 0.8, 1.0, 1.0, 1.0, 1.5, 2.0,
        2.5, 3.0, 3.0, 3.0, 4.0, 4.5, 5.0, 6.0, 6.0, 6.0, 8.0, 10.0, 12.0, 12.0, 12.0
    } };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OrangejuceAudioProcessor)
};
