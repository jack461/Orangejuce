/*
  ==============================================================================

    CircFracAudioBuffer.h
    Created: 17 Sep 2018 4:33:20pm
    Author:  Jean-Jacques Girardot

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "VariousStructures.h"

//==============================================================================
/*
*/
static const long MinBfSize {4096};
static const long MaxBfSize {0x10000000};
static const long StdGuard {128};
static const long StdSize {960000};

class ReadCircFracAudioBuffer;

class CircFracAudioBuffer 
{
public:
    CircFracAudioBuffer(long bS=StdSize, long cGard=StdGuard);
    ~CircFracAudioBuffer();


    void push(float ech);
    bool isReady() { return ready; };
    long bfSize() { return bSize; };
    long bfStart() { return lowBuff; };
    
private:
    inline void allClear() {
        for (long i=0; i<rawBSize; i++) rawBuffer[i]=0.0f;
        for (long i=0; i<guard; i++) tmpInput[i]=auxCopy[i]=0.0f;
    };
    float * rawBuffer = nullptr;
    float * tmpInput = nullptr;
    float * auxCopy = nullptr;
    long guard;
    long rawBSize, bSize;
    // Working variables
    long lowBuff, highBuff;
    long lowLimit, highLimit;
    // write index
    long wIndex;
    long tmpIndex;
    bool ready = false;
    friend class ReadCircFracAudioBuffer;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CircFracAudioBuffer)
};

//==============================================================================
/** Manage read from a Circular Fractional Monophonic AudioBuffer
    There can be more than one reader from an audio buffer
*/
class ReadCircFracAudioBuffer
{
public :
    ReadCircFracAudioBuffer(CircFracAudioBuffer&, struct CFABCtlBloc&);
    ~ReadCircFracAudioBuffer();
    
    /**
     Interpolate a value at position rIndex in the buffer.
     This implies that the position is in the loop limits.
     Get uses an "optimised" version of the Paul Breeuwsma version
     of the Catmull-Rom interpolation.
     */
    float get();
    
    bool checkReady()
    {
        ready = rawBuffer != nullptr && ctlb.beginPar != nullptr &&
        ctlb.sizePar != nullptr && ctlb.speedPar != nullptr;
        if (ready)
        {
            beginValue = * ctlb.beginPar;
            sizeValue = * ctlb.sizePar;
            speedValue = * ctlb.speedPar;
            endValue = beginValue + sizeValue;
            rIncr = double(speedValue);
            updateLoop ();
        }
        return ready;
    };
    void setIncr(double newIncr) { rIncr = newIncr; };
    double getPos() { return rIndex; };
    double getIncr() { return rIncr; };
    long lpBegin() { return lowLoop-lowBuff; };
    long lpSize() { return loopSize; };
    void setPos(double nPos) { rIndex = (nPos < lowLoop) ? lowLoop : (nPos > highLoop) ? highLoop : nPos ; };
protected :
    void updateLoop ()
    {
        beginValue = beginValue < 0.0f ? 0.0f : beginValue > 100.0f ? 100.0f : beginValue;
        endValue = beginValue + sizeValue;
        endValue = endValue < 0.0f ? 0.0f : endValue > 100.0f ? 100.0f : endValue;
        lowLoop = lowBuff + long(beginValue/100.0*(highBuff-lowBuff));
        highLoop = lowBuff + long(endValue/100.0*(highBuff-lowBuff));
        if (lowLoop > highLoop) { long w = lowLoop; lowLoop = highLoop; highLoop = w;}
        if (lowLoop >= highLoop - rguard)
        {
            if (lowLoop <= lowBuff + rguard) { highLoop = lowLoop + rguard; }
            else { lowLoop = highLoop - rguard; }
        }
        lowLLimit = lowLoop+rguard;
        highLLimit = highLoop-rguard;
        loopSize = highLoop - lowLoop +1;
        if ((rIndex > highLoop) || (rIndex < lowLoop)) rIndex = double(lowLoop);
        modded = false;
    }
    float smplGet(double&, double&);
private :
    CircFracAudioBuffer& caf;
    struct CFABCtlBloc& ctlb;
    float * rawBuffer;
    long wguard, rguard; // write guard & readguard
    long rawBSize, bSize;
    // Working variables
    long lowBuff, highBuff; // limits of readable buffer rawBuffer
    long lowLoop, highLoop, loopSize; // limits & size of current loop
    long lowLLimit, highLLimit;  // limits of special read algo for 
    // read index
    double rIndex;
    double rIncr;  // current read increment
    double biGuard;  // 2*rguard
    
    float beginValue, endValue, sizeValue, speedValue;
    bool modded;
    bool ready;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReadCircFracAudioBuffer)
};
