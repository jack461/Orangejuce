/*
  ==============================================================================

    VariousStructures.h
    Created: 19 Sep 2018 8:29:34pm
    Author:  Jean-Jacques Girardot

  ==============================================================================
*/

#pragma once

/** Fract. Delay Reader Control Bloc 
*/
struct CFABCtlBloc {
    float* beginPar;
    float* sizePar;
    float* speedPar;
    CFABCtlBloc() {
        beginPar = sizePar = speedPar = nullptr;
    };
};

