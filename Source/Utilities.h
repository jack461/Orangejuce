/*
  ==============================================================================

    Utilities.h
    Created: 15 Nov 2018 3:59:54pm
    Author:  Jean-Jacques Girardot

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

namespace cnrh {
    
extern void initUtilities();
extern String dbEdit(double db, int decn=2);
extern String dbEdTime(double dp, int prec=2, int mode=0);
extern String dbEdTimeV(double dp, int prec=2);
extern String dbEdFreq(double dp, int prec=2);
extern double strToDbl(String sstr);
extern double strToDuration(String sstr);
extern String dBToStr(float db);
extern float dBstrToDbl(String sstr);
extern bool eqVal(float f1, float f2, float eps);

inline float nullSnap(float start, float end, float val)
{
    // DBG("      nullSnap:  " << val);
    return val < start ? start : val > end ? end : val;
};
};

/**
 Associate a parameter value to a custom range defined by a vector of "equidistant" values
 The values should be in a non decreasing order
 Fuctions help mapping to [0 1] and reciprocal.
 Can provide an appropriate NormalisableRange.
 --
 Half bonus : you can change the displayed range...
 */
class CustomNormalisableRangeHelper
{
public:
    CustomNormalisableRangeHelper(std::vector<float> * cPoints, float lowV=0.0, float highV=0.0)
    : points(* cPoints)
    {
        DBG("**** CustomNormalisableRangeHelper");
        lowB=lowVal=lowV; highB=highVal=highV;
        cntCv01ToValue = cntCvValueTo01 = 0;
        checkValues();
        cntCv01ToValue = cntCvValueTo01 = 1000000;
    };
    ~CustomNormalisableRangeHelper()
    {
        DBG("**** ~CustomNormalisableRangeHelper ** Counters : " << cntCv01ToValue << "  " << cntCvValueTo01);
    };
    float lowBound() { return lowVal; };
    float highBound() { return highVal; };
    void setRange(float lowV, float highV);
    float cv01ToValue(float v);
    float cv01ToValue(float min, float max, float v) { return cv01ToValue(v); } ;
    float cvValueTo01(float v);
    float cvValueTo01(float min, float max, float v) { return cvValueTo01(v); } ;
    float snap(float v) { return v < lowB ? lowB : v > highB ? highB : v; };
    float snap(float min, float max, float v) { return v < min ? min : v > max ? max : v; };
    NormalisableRange<float> normalisableRange () { return NormalisableRange<float>(
        lowVal, highVal,
        [this](float start, float end, float gain) { return cv01ToValue(gain); },
        [this](float start, float end, float dB) { return cvValueTo01(dB); },
        [this](float start, float end, float val) { return snap(val); } );
    }
private:
    std::vector<float> points;
    float lowVal, highVal; // min and max declared values, never changed
    float lowB, highB; // current low & high values
    int lowIndex, highIndex; // current used limits
    int vSize; // size of point vector
    long cntCv01ToValue, cntCvValueTo01; // temporary counters for debugging
    void checkValues();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomNormalisableRangeHelper)
};

