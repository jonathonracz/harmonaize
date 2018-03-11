/*
  ==============================================================================

    hmnz_EditWindow.h
    Created: 29 Jan 2018 12:36:26am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_Edit.h"
#include "hmnz_EditView.h"
#include "hmnz_PlaybackEngine.h"

class EditWindow    : public DocumentWindow
{
public:
    EditWindow();
    ~EditWindow();

    void setEdit (Edit* edit) noexcept;

private:
    PlaybackEngine playbackEngine;
    std::unique_ptr<Edit> currentEdit;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditWindow)
};
