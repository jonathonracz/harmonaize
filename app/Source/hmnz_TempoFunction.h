/*
  ==============================================================================

    hmnz_TempoFunction.h
    Created: 3 Feb 2018 12:26:37am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

/*
    Used for calculating tempo mappings based on an interval defined by a
    beginning beat, end beat, tempo at the beginning beat, tempo at the end
    beat, and time offset (in seconds) of the first beat.

    Calculations based on work presented by:

    Bruno Dias, H. Sofia Pinto, David M. Matos. BPMTimeline: JavaScript Tempo
    Functions and Time Mappings using an Analytical Solution. In Web Audio
    Conference WAC-2016, April 4–6, 2016, Atlanta, USA.
*/
namespace TempoFunctions
{

class TempoFunction
{
public:
    TempoFunction (double _b0, double _b1, double _t0, double _t1, double _timeOffset)
        : b0 (_b0), b1 (_b1), t0 (_t0), t1 (_t1), timeOffset (_timeOffset) {}
    virtual ~TempoFunction() {}

    virtual double tempo (double beat) const noexcept = 0;
    virtual double beatPeriod (double beat) const noexcept = 0;
    virtual double time (double beat) const noexcept = 0;
    virtual double beat (double time) const noexcept = 0;

    double b0, b1, t0, t1, timeOffset;

protected:
    // Defined here to keep this class free of external dependencies.
    inline static double square (double x) noexcept { return x * x; }
};

class Linear    : public TempoFunction
{
public:
    using TempoFunction::TempoFunction;

    double tempo (double beat) const noexcept override
    {
        double deltaCalc = delta(b0, b1, t0, t1);
        return t0 + (deltaCalc * (beat - b0));
    }

    double beatPeriod (double beat) const noexcept override
    {
        return 1.0 / tempo (beat);
    }

    double time (double beat) const noexcept override
    {
        double bP0 = beatPeriod (b0);
        double bP1 = beatPeriod (b1);
        double deltaCalc = delta(b0, b1, t0, t1);
        return (delta() / 2.0) * square (beat - b0) + (bP0 * (beat - b0)) + timeOffset;
    }

    double beat (double time) const noexcept override
    {
        double bP0 = beatPeriod (b0);
        double bP1 = beatPeriod (b1);
        double delta2Calc = 2.0 * delta(b0, b1, bP0, bP1);
        double k1 = square (bP0) - (delta2Calc * (timeOffset - time));

        double sol1 = (-bP0 + k1) / delta2Calc;
        if (sol1 > 0)
            return sol1 + b0;

        double sol2 = (-bP0 - k1) / delta2Calc;
        return sol2 + b0;
    }

private:
    double delta(double x0, double x1, double y0, double y1) const noexcept
    {
        return (x0 == x1) ? y1 : (y1 - y0) / (x1 - x0);
    }
};

class Step  : public TempoFunction
{
public:
    using TempoFunction::TempoFunction;

    double tempo (double beat) const noexcept override
    {
        return (beat < b1) ? t0 : t1;
    }

    double beatPeriod (double beat) const noexcept override
    {
        return 1.0 / tempo (beat);
    }

    double time (double beat) const noexcept override
    {
        double bP0 = beatPeriod (b0);
        double bP1 = beatPeriod (b1);
        return (beat < b1) ? ((bP0 * beat) + timeOffset) : ((bP1 * beat) + timeOffset);
    }

    double beat (double time) const noexcept override
    {
        double bP0 = beatPeriod (b0);
        double bP1 = beatPeriod (b1);
        return (time < bP1) ? ((time - timeOffset) / bP0) : ((time - timeOffset) / bP1);
    }
};

}
