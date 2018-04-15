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
    setDefaultSansSerifTypefaceName ("SF Pro Display");
}

CustomLookAndFeel::~CustomLookAndFeel()
{
}

Font CustomLookAndFeel::getComboBoxFont (ComboBox& comboBox)
{
    Font defaultFont = LookAndFeel_V4::getComboBoxFont (comboBox);
    return defaultFont.withHeight (defaultFont.getHeight() * fontScale);
}

Font CustomLookAndFeel::getTextButtonFont (TextButton& textButton, int buttonHeight)
{
    Font defaultFont = LookAndFeel_V4::getTextButtonFont (textButton, buttonHeight);
    return defaultFont.withHeight (defaultFont.getHeight() * fontScale);
}

Font CustomLookAndFeel::getLabelFont (Label& label)
{
    Font defaultFont = LookAndFeel_V4::getLabelFont (label);
    return defaultFont.withHeight (defaultFont.getHeight() * fontScale);
}

Font CustomLookAndFeel::getSliderPopupFont (Slider& slider)
{
    Font defaultFont = LookAndFeel_V4::getSliderPopupFont (slider);
    return defaultFont.withHeight (defaultFont.getHeight() * fontScale);
}

void CustomLookAndFeel::setFontScale (float scale)
{
    fontScale = scale;
}

float CustomLookAndFeel::getFontScale()
{
    return fontScale;
}

CustomLookAndFeel& CustomLookAndFeel::getHarmonaizeLookAndFeel()
{
    // Note that this will explode if the default look and feel hasn't been set
    // to an instance of this class!
    return static_cast<CustomLookAndFeel&> (getDefaultLookAndFeel());
}
