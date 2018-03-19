/*
  ==============================================================================

    hmnz_ArrangementViewHeaderControls.h
    Created: 19 Mar 2018 6:11:10pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class Edit;

class ArrangementViewHeaderControls : public Component,
                                      public Button::Listener
{
public:
    ArrangementViewHeaderControls (Edit& edit);

private:
    Edit& edit;
    TextButton addTrack;

    void resized() override;

    void buttonClicked (Button*) override;
};
