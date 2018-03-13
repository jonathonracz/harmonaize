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
        saveProject             = 0x300003,
        saveProjectAs           = 0x300004,
        
        showPreferences         = 0x300020,
        
        undo                    = 0x300040,
        redo                    = 0x300041,
        
        lastCommandIDEntry
    };
}

namespace CommandCategories
{
    static const char* const general       = "General";
    static const char* const editing       = "Editing";
    static const char* const view          = "View";
}
