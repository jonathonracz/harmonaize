/*
  ==============================================================================

    hmnz_ArrangementViewTrackHeader.h
    Created: 7 Mar 2018 4:47:06pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class Track;

class ArrangementViewTrackHeader  : public Component,
                                    public ValueTree::Listener,
                                    public Label::Listener,
                                    public Button::Listener
{
public:
    ArrangementViewTrackHeader (Track& track);
    ~ArrangementViewTrackHeader();

private:
    Track& track;
    Label name;
    ProgressBar loadProgress;
    TextButton instrumentSelector;

    class ProgressBarBackground;
    std::unique_ptr<ProgressBarBackground> progressBarBackground;

    void resized() override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    void labelTextChanged (Label*) override {}
    void editorHidden (Label *labelThatHasChanged, TextEditor& textEditor) override;

    void buttonClicked (Button *) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewTrackHeader)
};
