/*
  ==============================================================================

    hmnz_CommandIDs.h
    Created: 12 Mar 2018 2:11:34am
    Author:  Landon

  ==============================================================================
*/

#pragma once

namespace CommandIDs
{
    enum
    {
        newProject              = 0x300000,
        openProject             = 0x300001,
        saveProject             = 0x300002,
        saveProjectAs           = 0x300003,

        showPreferences         = 0x300020,
        showDebugger            = 0x300021,

        undo                    = 0x300040,
        redo                    = 0x300041,

        scaleUp                 = 0x300050,
        scaleDown               = 0x300051,

        playPause               = 0x300060,
        goToBeginning           = 0x300061,
        toggleMetronome         = 0x300062,
        reset                   = 0x300063,
        record                  = 0x300064,
        generateAccompaniment   = 0x300065,

        lastCommandIDEntry
    };
}

namespace CommandCategories
{
    static const char* const general       = "General";
    static const char* const editing       = "Editing";
    static const char* const view          = "View";
}
