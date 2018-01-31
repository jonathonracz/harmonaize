/*
  ==============================================================================

    hmnz_Edit.h
    Created: 22 Jan 2018 12:56:24am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_TrackArray.h"
#include "hmnz_MasterTrack.h"
#include "hmnz_Transport.h"
#include "hmnz_Track.h"
#include "hmnz_CacheValueWrappers.h"

/**
    Represents an active edit (also known as a project).
*/
class Edit  : public ValueTreeObject<IDs::Edit>,
              public AudioSource
{
public:
    Edit (const ValueTree& v);
    ~Edit();

    static ValueTree createSkeletonEdit();

    // AudioSource methods
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;

    std::unique_ptr<MasterTrack> masterTrack;
    std::unique_ptr<Transport> transport;

private:
    UndoManager undoManager;
    TrackArray tracks;
    jcf::ValueTreeDebugger stateDebugger;

    // ValueTree methods
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;

    JUCE_DECLARE_WEAK_REFERENCEABLE (Edit)
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Edit)
};
