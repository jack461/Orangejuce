/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "CircFracAudioBuffer.h"
#include "Utilities.h"

//==============================================================================
OrangejuceAudioProcessor::OrangejuceAudioProcessor()
     :
#ifndef JucePlugin_PreferredChannelConfigurations
     AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
     parameters (*this, nullptr), 
     cfab(48000 * 60), // a 60 seconds buffer at 48kHz
     rca (cfab, looper1)
{
    DBG("**** OrangejuceAudioProcessor 1.0.1 -> ");
    using Parameter = AudioProcessorValueTreeState::Parameter;
    parameters.createAndAddParameter (std::make_unique<Parameter> (
                                          "l1begin",       // parameterID
                                          "Begin",       // parameter name
                                          "",           // parameter label (suffix)
                                          NormalisableRange<float> (0.0f, 100.0f, 0.001f),    // range
                                          0.0f,         // default value
                                          [] (float p) { return dbEdit(double(p), 3) + String(" %"); },
                                          nullptr));

    parameters.createAndAddParameter (std::make_unique<Parameter> (
                                         "l1end",       // parameterID
                                          "End",       // parameter name
                                          "",           // parameter label (suffix)
                                          NormalisableRange<float> (0.0f, 100.0f, 0.001f),    // range
                                          100.0f,         // default value
                                          [] (float p) { return dbEdit(double(p), 3) + String(" %"); },
                                          nullptr));

    parameters.createAndAddParameter (std::make_unique<Parameter> (
                                         "l1speed",       // parameterID
                                          "Speed",       // parameter name
                                          "",           // parameter label (suffix)
                                          NormalisableRange<float> (-4.0f, 4.0f, 0.0001f),    // range
                                          1.0f,         // default value
                                          [] (float p) { return dbEdit(double(p), 4); },
                                          nullptr));

  
                                          
    parameters.state = ValueTree (Identifier ("OrangeV01"));

    looper1.beginPar = parameters.getRawParameterValue ("l1begin");
    looper1.endPar = parameters.getRawParameterValue ("l1end");
    looper1.speedPar = parameters.getRawParameterValue ("l1speed");
    
    
   
   
    presetLoaded = false;
    DBG("**** OrangejuceAudioProcessor<- ");
}

OrangejuceAudioProcessor::~OrangejuceAudioProcessor()
{
    DBG("**** ~OrangejuceAudioProcessor* ");
}


//==============================================================================
void OrangejuceAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    rca.checkReady();
}

void OrangejuceAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OrangejuceAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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





//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OrangejuceAudioProcessor();
}
