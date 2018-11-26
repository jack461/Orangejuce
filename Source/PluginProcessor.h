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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OrangejuceAudioProcessor)
};
