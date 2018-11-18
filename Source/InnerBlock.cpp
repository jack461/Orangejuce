/*
  ==============================================================================

    InnerBlock.cpp
    Created: 19 Sep 2018 7:21:53pm
    Author:  Jean-Jacques Girardot

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "CircFracAudioBuffer.h"


void OrangejuceAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();

    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

        
    if (totalNumInputChannels <= 0 || totalNumOutputChannels <= 0 || numSamples <= 0)
        return;
        
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.


    const float * inChannels[64];
    for (auto k = 0; k < totalNumInputChannels; ++k)
    {
        inChannels[k] = buffer.getReadPointer (k); 
    }
    

    float * outChannels[64];
    for (auto k = 0; k < totalNumOutputChannels; ++k)
    {
        outChannels[k] = buffer.getWritePointer (k); 
    }

    
    
    for (auto k = 0; k < numSamples; ++k)
    {
        float smpl = 0;
        for (auto j = 0; j < totalNumInputChannels; j++)
        {
            const float * pts = inChannels[j];
            smpl += pts[k];
        }
        smpl = smpl/totalNumInputChannels;
        cfab.push(smpl);
        smpl = rca.get();
        for (auto j = 0; j < totalNumOutputChannels; j++)
        {
            float * pts = outChannels[j];
            pts[k] = smpl;
        }
        
    }
        

}

