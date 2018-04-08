/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 8 Apr 2018 12:25:02pm
    Author:  Landon

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class CustomLookAndFeel : public LookAndFeel_V4
{
public:
    CustomLookAndFeel();
    virtual ~CustomLookAndFeel();
    void setFontScale(double scale);
    double getFontScale();
private:
    virtual Font getComboBoxFont (ComboBox&);
    virtual Font getTextButtonFont (TextButton&, int buttonHeight);
    virtual Font getLabelFont (Label&);
    virtual Font getSliderPopupFont (Slider&);

    double fontScale;
};
