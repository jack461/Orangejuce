/*
  ==============================================================================

    Utilities.cpp
    Created: 15 Nov 2018 3:59:54pm
    Author:  Jean-Jacques Girardot

  ==============================================================================
*/

#include "Utilities.h"
#include <stdio.h>
#include <stdlib.h>



/*
static constexpr int dBScaleSize {59};
static constexpr float dBScale[dBScaleSize] {
    -1010.0, -1000.0, -800.0, -600.0, -450.0, -340.0,
    -260.0, -200.0, -150.0, -110.0, -80.0, -60.0, -48.0, -40.0,
    -38.0, -28.0, -22.0, -20.0, -18.0, -15.0, -12.0, -10.0,
    -8.0, -6.0, -5.0, -4.0, -3.0, -2.5, -2.0, -1.5, -1.0,
    -0.8, -0.6, -0.4, -0.3, -0.2, -0.15, -0.1, -0.05, 0.0,
    0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.6, 0.8, 1.0, 1.5, 2.0,
    2.5, 3.0, 4.0, 5.0, 6.0,  8.0, 10.0,  12.0
};
*/


/** This define a dB scale with some steps at 12dB, 6dB, 3dB, 1dB, 0.1dB, 0dB,
    -0.1dB, -1dB, -3dB, -6dB, -20dB, -40dB, -80dB, -1000dB and -oo
 */
static constexpr float minInfinity { -1010.0 };
static constexpr float minDBValue { -1000.0 };
static constexpr int dBScaleSize {92};
static constexpr float dBScale[dBScaleSize] {

    minInfinity, minInfinity, minDBValue, minDBValue, minDBValue, -800.0, -600.0, -450.0, -340.0,
    -260.0, -200.0, -150.0, -110.0, -80.0, -80.0, -80.0, -60.0, -48.0, -40.0, -40.0, -40.0,
    -34.0, -28.0, -22.0, -20.0, -20.0, -20.0, -18.0, -15.0, -12.0, -12.0, -12.0, -10.0,
    -8.0, -6.0, -6.0, -6.0, -5.0, -4.0, -3.0, -3.0, -3.0, -2.5, -2.0, -1.5, -1.0, -1.0, -1.0,
    -0.8, -0.6, -0.4, -0.3, -0.2, -0.15, -0.1, -0.1, -0.1, -0.05, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.05, 0.1, 0.1, 0.1, 0.15, 0.2, 0.3, 0.4, 0.6, 0.8, 1.0, 1.0, 1.0, 1.5, 2.0,
    2.5, 3.0, 3.0, 3.0, 4.0, 5.0, 6.0, 6.0, 6.0, 8.0, 10.0, 12.0, 12.0, 12.0
    
};

    /*
    12.0, 12.0, 12.0, 10.0, 8.0, 6.0, 6.0, 5.0, 4.0, 3.0, 3.0, 2.5,
    2.0, 1.5, 1.0, 1.0, 0.8, 0.6, 0.4, 0.3, 0.2, 0.15, 0.1, 0.05,
    0.0, 0.0, 0.0, 0.0, -0.05, -0.1, -0.15, -0.2, -0.3, -0.4, -0.6, -0.8,
    -1.0, -1.0, -1.5, -2.0, -2.5, -3.0, -3.0, -4.0, -5.0, -6.0, -6.0, -8.0, -10.0, -12.0, -12.0,
    -15.0, -18.0, -20.0, -20.0, -22.0, -28.0, -38.0, -40.0, -40.0, -48.0, -60.0, -80.0, -80.0, -110.0,
    -150.0, -200.0, -260.0, -340.0, -450.0, -600.0, -800.0, -1000.0, -1000.0, -1010.0, -1010.0 
     */

// Currently not used...
static float dBConverted[dBScaleSize];

/** A relative comparison of two values,
 with "eps" being a comparison tolerence
 */
static bool eqVal(float f1, float f2, float eps)
{
    return eps * (fabs(f1) + fabs(f2)) >= fabs(f1-f2);
}

/** Find an entry in an array of non decreasing values
 */
static int findLow(float val, const float * tab, int tabsize)
{
    int lb=0, hb = tabsize-1, mid;
    while ((hb - lb) > 1)
    {
        mid = (hb + lb)/2;
        if (val == tab[mid]) {
            // take care of a sequence of identical values
            hb = lb = mid;
            while (lb > 0 && val == tab[lb-1]) --lb;
            if (lb == 0) return lb;
            while (hb < tabsize-1 && val == tab[hb+1]) ++hb;
            if (hb == tabsize-1) return hb;
            return (hb+lb)/2;
        }
        if (val < tab[mid]) hb = mid;
        else lb = mid;
    }
    return lb;
}

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

/** Edit a duration in s or ms
    dp is a value in seconds 
*/
String dbEdTime(double db, int decn, int mode)
{
    constexpr int bsize {64};
    constexpr int fsize {24};
    char buff[bsize];
    char fmtf[fsize];
    decn = decn<0 ? 0 : decn>16 ? 16 : decn;
    switch (mode) {
        default: // use "s"
            sprintf(fmtf, "%%.%df s", decn);
            snprintf(buff, bsize, fmtf, db);
            break;
        case 1: // use "ms"
            sprintf(fmtf, "%%.%df ms", decn);
            snprintf(buff, bsize, fmtf, 1000*db);
            break;
        case 2: // use "s & ms"
            {
                long s = long(db);
                db = (db - s)*1000;
                sprintf(fmtf, "%%d\"%%03.%df", decn);
                snprintf(buff, bsize, fmtf, s, db);
            }
            break;
        case 3: // use "m, s & ms"
            {
                long m = long(db/60.0);
                db = db - 60.0*m;
                long s = long(db);
                db = (db - s)*1000;
                sprintf(fmtf, "%%d'%%02d\"%%03.%df", decn);
                snprintf(buff, bsize, fmtf, m, s, db);
            }
            break;
    }
    return String(buff);
    
}

/** Edit a duration in s or ms
    dp is a value in seconds 
*/
String dbEdTimeV(double db, int prec)
{
    constexpr int bsize {64};
    char buff[bsize];
    if (db < 2.0)
    {
        // edit as ms.
        if (db < 0.02)
        {
            snprintf(buff, bsize, "%.3f ms", db*1000.0);
        }
        else
        {
            snprintf(buff, bsize, "%.2f ms", db*1000.0);
        }
    }
    else
    if (db < 20.0)
    {
        snprintf(buff, bsize, "%.3f s", db);
    }
    else
    {
        snprintf(buff, bsize, "%.2f s", db);
    }
    return String(buff);
}

/** Edit a frequency in Hz or kHz
    dp is a frequency in Hz 
*/
String dbEdFreq(double db, int prec)
{
    constexpr int bsize {64};
    char buff[bsize];
    if (db < 2000.0)
    {
        // edit as Hz.
        if (db < 20.0)
        {
            if (db < 2.0)
                snprintf(buff, bsize, "%.5f Hz", db);
            else
                snprintf(buff, bsize, "%.4f Hz", db);
        }
        else
        if (db < 200.0)
        {
            snprintf(buff, bsize, "%.3f Hz", db);
        }
        else
        {
            snprintf(buff, bsize, "%.2f Hz", db);
        }
    }
    else
    if (db < 8000.0)
    {
        snprintf(buff, bsize, "%.3f kHz", db*0.001);
    }
    else
    {
        snprintf(buff, bsize, "%.2f kHz", db*0.001);
    }
    return String(buff);
}

double strToDbl(String sstr)
{
    char * str = (char *)sstr.toRawUTF8();
    char * nxt = str;
    double dbl = strtod(str, &nxt);
    return dbl;
}

/** Parse a string as a "duration"
    Accept values expressed as int or float numbers with qualifiers such as
    "h" (hours), "m" or "'" (minutes), "s" or """ (seconds), and "ms" (milliseconds)
    Seconds is the default unit, but after an explicit "seconds" unit, default unit
    is the millisecond.
    Result is in seconds.
      3h25m   is 3 hours and 25 minutes
      3h25    is 3 hours and 25 seconds
      3.25h   is 3 hours and 15 minutes
      3h2'5   is 3 hours, 2 minutes and 5 seconds
      3'25"   is 3 minutes and 25 seconds
      3'25    is also 3 minutes and 25 seconds
      3"25    is 3 seconds and 25 ms
      3.25    is 3 seconds and 250 ms
 */
double strToDuration(String sstr)
{
    char * str = (char *)sstr.toRawUTF8();
    char * nxt = str;
    double dbl {0.0}, tmp {0.0};
    const double factors[4] {0.001, 1.0, 60.0, 3600.0};
    int ftyp = 1;
    int ch;
    bool doit = true;
    while (doit && (ch=*str) != 0)
    {
        nxt = str+1;
        switch(ch)
        {
            case ' ': case '\t': case '\n': case '\r':
                break;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
            case '.':
                tmp = strtod(str, &nxt);
                if (str == nxt) nxt = str+1;
                break;
            case 'h': // hours
                dbl = dbl + tmp * factors[3]; tmp = 0.0; ftyp=1;
                break;
            case 'm':
                if (str[1]=='s')
                {
                    // a duration in ms
                    dbl = dbl + tmp * factors[0]; tmp = 0.0; ftyp=0;
                    nxt = str+2;
                    break;
                }
            case '\'': // minutes
                dbl = dbl + tmp * factors[2]; tmp = 0.0; ftyp=1;
                break;
            case 's': case '"': // seconds
                dbl = dbl + tmp * factors[1]; tmp = 0.0; ftyp=0;
                break;
            default: // unknown char : stop, no error...
                doit = false;
        };
        str = nxt;
    }
    dbl = dbl + tmp * factors[ftyp];
    return dbl;
}

/**  Edit a value in decibels
 */
String dBToStr(float db)
{
    constexpr int bsize {64};
    char buff[bsize];
    // DBG("  dB->str   " << db);
    if (db < -12.0)
    {
        if (db < -100.0)
        {
            if (db < minDBValue)
            {
                // strcpy(buff, "-\xe2\x88\x9e dB"); // -infinity dB
                return String(CharPointer_UTF8 ("-\xe2\x88\x9e dB"));
            }
            else
                snprintf(buff, bsize, "%.0f dB", db);
        }
        else
            snprintf(buff, bsize, "%.1f dB", db);
    }
    else
    {
        if (db == 0)
            strcpy(buff, "0.00 dB");
        else
            snprintf(buff, bsize, "%+.2f dB", db);
    }
    return String(buff);
}

/** Convert a 0-1 slider value to a value in decibels
 according to our conversion array, with linear interpolation
 */
static inline float innercv01todB(float val)
{
    val = val < 0.0 ? 0.0 : val > 1.0 ? 1.0 : val;
    float idx = val*(dBScaleSize-1);
    int j = (int)idx;
    float alph = idx - (float)j;
    float z = (dBScale[j] * (1.0 - alph) + dBScale[j+1] * alph);
    // DBG("      cv01todB:  " << val << "    i = " << idx << "  [" << j << ":" << dBScale[j] << "  " << val << "  " << (j+1) << ":" << dBScale[j+1]  <<"]*" << alph  << "  -->  " << z);
    return z;
}

static inline float innercvdBto01(float val)
{
    val = val < minDBValue ? minInfinity : val > 12.0 ? 12.0 : val;
    int p = findLow(val, dBScale, dBScaleSize);
    // DBG(" p=" << p);
    if (p == 0) return 0.0;
    if (p == dBScaleSize-1) return 1.0;
    float u=dBScale[p], v=dBScale[p+1];
    float w = (u != v) ? p + (val-u)/(v-u) : (float)p;
    float z = (w)/(dBScaleSize-1.0);
    //
    // DBG("      cvdBto01:  " << val << "    p = " << p << "  [" << u << "  " << val << "  " << v  <<"]  --> " << w << "  --> " << z);
    return z;
}

/*
 Check and trace the function
 */
float cv01todB(float val)
{
    // static long cnt=100000000;
    float z = innercv01todB(val);
    // float newval = innercvdBto01(z); cnt++;
    // DBG("[" << cnt << "]  01->dB   " << val << " --> "  << z << "  ~~   " << newval);
    // jassert(eqVal(val,newval,1e-6f));
    return z;
}


float cvdBto01(float val)
{
    // static long cnt=200000000;
    float z = innercvdBto01(val);
    // cnt++;
    // DBG("[" << cnt << "]  dB->01   " << val << " --> "  << z << "  ~~   " << innercv01todB(z));
    // jassert(val == innercv01todB(z));
    return z;
}

float dBstrToDbl(String sstr)
{
    char * str = (char *)sstr.toRawUTF8();
    char * nxt = str;
    double dbl = strtod(str, &nxt);
    if (str == nxt) dbl = minInfinity; // for -infinity
    // DBG("  Str->dB   " << sstr << " --> " << dbl);
    return dbl;
}

//===================================================
// Initialize Utilities
void initUtilities()
{
    for (int i=0; i<dBScaleSize; i++)
    {
        double db = dBScale[i];
        dBConverted[i] = db < minDBValue ? 0.0 : std::pow(10.0, db*0.05);
    }
}
