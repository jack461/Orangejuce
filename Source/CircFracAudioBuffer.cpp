/*
  ==============================================================================

    CircFracAudioBuffer.cpp
    Created: 17 Sep 2018 4:33:20pm
    Author:  Jean-Jacques Girardot

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "CircFracAudioBuffer.h"

//==============================================================================
CircFracAudioBuffer::CircFracAudioBuffer(long bS, long cGard)
{
    DBG("**** CircFracAudioBuffer-> ");
    rawBSize = 0;
    ready = false;
    bS = bS < MinBfSize ? MinBfSize : bS;
    bS = bS > MaxBfSize ? MaxBfSize : bS;
    cGard = cGard < 16 ? 16 : cGard;
    cGard = cGard > 2048 ? 2048 : cGard;
    bS = bS + cGard + cGard;
    rawBuffer = new float[bS];
    if (rawBuffer != nullptr) rawBSize = bS;
    // Compute various position for read/write limits
    guard = cGard;
    tmpInput = new float[guard];
    auxCopy = new float[guard];
    // The "values" will be written in [lowBuff  ...  highBuff]
    lowBuff = guard;
    highBuff = rawBSize - guard - 1;
    // we will duplicate values that are "near" the edges of the interval
    highLimit = highBuff - guard;
    lowLimit = lowBuff + guard;
    bSize = highBuff - lowBuff +1;
    wIndex = lowBuff;
    tmpIndex = 0;
    if (rawBuffer == nullptr || tmpInput == nullptr || auxCopy== nullptr)
    {
        if (rawBuffer != nullptr) { delete [] rawBuffer; rawBuffer = nullptr; }
        if (tmpInput != nullptr) { delete [] tmpInput; tmpInput = nullptr; }
        if (auxCopy != nullptr) { delete [] auxCopy; auxCopy = nullptr; }
        DBG("**** ERROR : CircFracAudioBuffer init fail ");
        jassertfalse;
        return;
    }
    allClear();
    ready = true;
    DBG("**** CircFracAudioBuffer<- ");
}

CircFracAudioBuffer::~CircFracAudioBuffer()
{
    if (rawBuffer != nullptr) { delete [] rawBuffer; rawBuffer = nullptr; }
    if (tmpInput != nullptr) { delete [] tmpInput; tmpInput = nullptr; }
    if (auxCopy != nullptr) { delete [] auxCopy; auxCopy = nullptr; }
    ready = false;
    DBG("**** ~CircFracAudioBuffer* ");
}

//  Implementation
void CircFracAudioBuffer::push (float ech)
{
    jassert(ready);
    if (! ready) return;
    jassert(wIndex >= lowBuff && wIndex < rawBSize);
    // Manage the "slow write" function
    if (tmpIndex >= guard) tmpIndex = 0;
    tmpInput[tmpIndex] = ech;
    auxCopy[tmpIndex] = rawBuffer[wIndex];
    // Manage the transition
    long j = tmpIndex;
    long k = wIndex;
    for (long i=0; i<guard; i++)
    {
        float alpha = ((1.0f+i)/float(guard));
        if (j < 0) j+= guard;
        float val = tmpInput[j]*alpha + auxCopy[j]*(1.0f-alpha);
        rawBuffer[k] = val;
        if (k < lowLimit) rawBuffer[k+bSize] = val;
        if (k > highLimit) rawBuffer[k-bSize] = val;
        j--; k--;
    }
    ++wIndex; if (wIndex >= rawBSize) wIndex = lowLimit;
    ++tmpIndex;
    /*
    // This merge the cases "start of buffer" and "end of buffer"
    if (wIndex > highLimit)
    {
        // DBG("write " << wIndex << " & " << wIndex-bSize << " : " << ech);
        rawBuffer[wIndex] = rawBuffer[wIndex-bSize] = ech;
        ++wIndex;
        if (wIndex >= rawBSize)
        {
            wIndex = lowLimit;
        }
        return;
    }
    
    // Most of the time, we are "in" the buffer
    // DBG("write " << wIndex << " : " << ech);
    rawBuffer[wIndex] = ech;
    ++wIndex;
    return;
    */
}


ReadCircFracAudioBuffer::ReadCircFracAudioBuffer(CircFracAudioBuffer& c, struct CFABCtlBloc& b) :
      caf(c), ctlb(b)
{
    DBG("**** ReadCircFracAudioBuffer-> ");
    ready = false;
    // Copy common variables
    rawBuffer = caf.rawBuffer;
    wguard = caf.guard;
    rguard = wguard-2;
    rawBSize =caf.rawBSize;
    beginValue = 0.0f;
    endValue = 100.0f;
    speedValue = 1.0f;
    modded = true; // we will have to recompute all...
    // By default use same limits as the writer
    lowLoop = lowBuff = caf.lowBuff;
    highLoop = highBuff = caf.highBuff;
    lowLLimit = lowLoop+rguard;
    highLLimit = highLoop-rguard;
    loopSize = highLoop - lowLoop +1;
    bSize = caf.bSize;
    rIndex = (float)lowBuff;
    rIncr = 1.0f;
    biGuard = 2.0f * rguard;
    DBG("** rawBSize   " << rawBSize);
    DBG("** lowLLimit  " << lowLLimit);
    DBG("** highLLimit " << highLLimit);
    DBG("** lowLoop    " << lowLoop);
    DBG("** highLoop   " << highLoop);
    DBG("**** ReadCircFracAudioBuffer<- ");
}

ReadCircFracAudioBuffer::~ReadCircFracAudioBuffer()
{
    DBG("**** ~ReadCircFracAudioBuffer* ");
}

float ReadCircFracAudioBuffer::get()
{
    if (!ready) return 0.0f;
    
    if (beginValue != * ctlb.beginPar)
    {
        beginValue = * ctlb.beginPar;
        DBG("****   BEGIN -> " << beginValue);
        modded = true;
    }
    if (sizeValue != * ctlb.sizePar)
    {
        sizeValue = * ctlb.sizePar;
        DBG("****   END -> " << endValue);
        modded = true;
    }
    if (speedValue != * ctlb.speedPar)
    {
        speedValue = * ctlb.speedPar;
        DBG("****   SPEED -> " << speedValue);
        rIncr = double(speedValue);
    }
    
    if (modded)
    {
        updateLoop ();
        DBG("****   LOOP -> [ " << lowLoop << " ... " << highLoop << " ]  at " << rIncr);
    }
    
    return smplGet(rIndex, rIncr);
}


float ReadCircFracAudioBuffer::smplGet(double& index, double& incr)
{
    enum class Algotype { simple, start, end };
    Algotype work = Algotype::simple;
    double rIndex = index;
        
    if (rIndex > highLLimit)
    {
        if (rIndex > highLLimit + rguard)
        {
            index -= loopSize;  // update ext. index
            rIndex = index;
            work = Algotype::start;
        }
        else
            work = Algotype::end;
    }
    if (rIndex < lowLLimit)
    {
        if (rIndex < lowLLimit - rguard)
        {
            index += loopSize; // update ext. index
            rIndex = index;
            work = Algotype::end;
        }
        else
            work = Algotype::start;
    }
    
    long iIndex = long(floor(rIndex));
    double delta = rIndex - double(iIndex);
    
    // Handle the "direct slightly greater" case
    if (delta <= 0.000001)
    {
        // DBG("read < " << iIndex);
        float value = rawBuffer[iIndex];
        index = double(iIndex) + incr; // update ext. index
        return value;
    }
    // Handle the "direct slightly lower" case
    if (delta >= 0.999999)
    {
        // DBG("read > " << ++iIndex);
        float value = rawBuffer[iIndex];
        index = double(iIndex) + incr; // update ext. index
        return value;
    }
    
    // We need to interpolate
    float xm1, x0, x1, x2;
    switch (work)
    {
            // Simple case (most of the time) : we use directly the values in the buffer
        case Algotype::simple :
            // simple algorithm
            xm1 = rawBuffer[iIndex - 1];
            x0 = rawBuffer[iIndex];
            x1 = rawBuffer[iIndex + 1];
            x2 = rawBuffer[iIndex + 2];
            // DBG("read " << index << " -> " << iIndex);
            break;
            
        // We are at the start of the buffer : merge with "end"
       case Algotype::start :
            {
            float qFactor = 0.5f + (index - float(lowLoop))/biGuard;
            // interpolate between start & end values
            xm1 = rawBuffer[iIndex - 1] * qFactor + rawBuffer[iIndex + loopSize - 1] * (1.0f - qFactor);
            x0 = rawBuffer[iIndex] * qFactor + rawBuffer[iIndex + loopSize] * (1.0f - qFactor);
            x1 = rawBuffer[iIndex + 1] * qFactor + rawBuffer[iIndex + loopSize + 1] * (1.0f - qFactor);
            x2 = rawBuffer[iIndex + 2] * qFactor + rawBuffer[iIndex + loopSize + 2] * (1.0f - qFactor);
            // DBG("read " << rIndex << " -> " << iIndex << " & " << iIndex + loopSize << " [" << iIndex - 1 << "  " << iIndex + loopSize + 2 << "] : " << qFactor );
            }
            break;
            
        // We are at the end of the buffer : merge with start
        case Algotype::end :
            {
            float qFactor = 0.5f + (float(highLoop) - rIndex)/biGuard;
            // interpolate between start & end values
            xm1 = rawBuffer[iIndex - 1] * qFactor + rawBuffer[iIndex - loopSize - 1] * (1.0f - qFactor);
            x0 = rawBuffer[iIndex] * qFactor + rawBuffer[iIndex - loopSize] * (1.0f - qFactor);
            x1 = rawBuffer[iIndex+1] * qFactor + rawBuffer[iIndex - loopSize + 1] * (1.0f - qFactor);
            x2 = rawBuffer[iIndex+2] * qFactor + rawBuffer[iIndex - loopSize + 2] * (1.0f - qFactor);
            // DBG("read " << rIndex << " -> " << iIndex << " & " << iIndex - loopSize << " [" << iIndex - loopSize - 1 << "  " << iIndex + 2 << "] : " << qFactor);
            }
            break;
            
    }
    
    index += incr;   // update ext. index
    
    // This is an "optimised" version of the Paul Breeuwsma version
    // of the Catmull-Rom interpolation
    float a = (3.0f * (x0 - x1) - xm1 + x2) * 0.5f;
    float b = 2.0f * x1 + xm1 - (5.0f*x0 + x2) * 0.5f;
    float c = (x1 - xm1) * 0.5f;
    
    return float((((a * delta) +b) * delta + c) * delta + x0);
}
