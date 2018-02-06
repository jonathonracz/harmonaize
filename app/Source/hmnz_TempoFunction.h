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
        return t0 + (delta() * (beat - b0));
    }

    double beatPeriod (double beat) const noexcept override
    {
        return tempo (beat);
    }

    double time (double beat) const noexcept override
    {
        return (delta() / 2.0) * square (beat - b0) + (t0 * (beat - b0)) + timeOffset;
    }

    double beat (double time) const noexcept override
    {
        double k1 = square (t0) - (2.0 * delta() * (timeOffset - time));
        double delta2 = 2.0 * delta();
        double sol1 = (-t0 + k1) / delta2;

        if (sol1 > 0)
            return sol1 + b0;

        double sol2 = (-t0 - k1) / delta2;
        return sol2 + b0;
    }

private:
    double delta() const noexcept
    {
        return (b0 == b1) ? b1 : (t1 - t0) / (b1 - b0);
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
        return tempo (beat);
    }

    double time (double beat) const noexcept override
    {
        return (beat < b1) ? ((t0 * beat) + timeOffset) : ((t1 * beat) + timeOffset);
    }

    double beat (double time) const noexcept override
    {
        return (time < t1) ? ((time - timeOffset) / t0) : ((time - timeOffset) / t1);
    }
};

}
