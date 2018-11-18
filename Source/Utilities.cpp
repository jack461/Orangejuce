/*
  ==============================================================================

    Utilities.cpp
    Created: 15 Nov 2018 3:59:54pm
    Author:  Jean-Jacques Girardot

  ==============================================================================
*/

#include "Utilities.h"
#include <stdio.h>

String dbEdit(double db, int decn)
{
    constexpr int bsize {64};
    constexpr int fsize {8};
    char buff[bsize];
    char fmtf[fsize];
    decn = decn<0 ? 0 : decn>16 ? 16 : decn;
    sprintf(fmtf, "%%.%df", decn);
    snprintf(buff, bsize, fmtf, db);
    return String(buff);
}
