/*
  ==============================================================================

    Utilities.h
    Created: 15 Nov 2018 3:59:54pm
    Author:  Jean-Jacques Girardot

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

extern void initUtilities();
extern String dbEdit(double db, int decn=2);
extern String dbEdTime(double dp, int prec=2, int mode=0);
extern String dbEdTimeV(double dp, int prec=2);
extern String dbEdFreq(double dp, int prec=2);
extern double strToDbl(String sstr);
extern double strToDuration(String sstr);
extern float cv01todB(float val);
extern float cvdBto01(float val);
extern String dBToStr(float db);
extern float dBstrToDbl(String sstr);
inline float nullSnap(float start, float end, float val)
{
    // DBG("      nullSnap:  " << val);
    return val < start ? start : val > end ? end : val;
};
