/*
  ==============================================================================

    hmnz_EditWindow.h
    Created: 29 Jan 2018 12:36:26am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_Edit.h"

class EditWindow    : public DocumentWindow
{
public:
    EditWindow();

private:
    std::unique_ptr<Edit> currentEdit;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditWindow)
};
