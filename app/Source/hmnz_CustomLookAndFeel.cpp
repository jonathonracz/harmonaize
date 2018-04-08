/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 8 Apr 2018 12:25:02pm
    Author:  Landon

  ==============================================================================
*/

#include "hmnz_CustomLookAndFeel.h"


CustomLookAndFeel::CustomLookAndFeel()
{
    setDefaultSansSerifTypefaceName("Lucida Sans Unicode");
    fontScale = 1;
}

CustomLookAndFeel::~CustomLookAndFeel()
{
    
}

Font CustomLookAndFeel::getComboBoxFont (ComboBox&)
{
    return Font (15 * fontScale);
}

Font CustomLookAndFeel::getTextButtonFont (TextButton& textButton, int buttonHeight)
{
    return Font (15 * fontScale);
}

Font CustomLookAndFeel::getLabelFont (Label& label)
{
    return Font (15 * fontScale);
}

Font CustomLookAndFeel::getSliderPopupFont (Slider& slider)
{
    return Font (15 * fontScale);
}

void CustomLookAndFeel::setFontScale(double scale)
{
    fontScale = scale;
}

double CustomLookAndFeel::getFontScale()
{
    return fontScale;
}
