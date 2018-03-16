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
#include "hmnz_CommandIDs.h"
#include "hmnz_Application.h"

class EditWindow    : public DocumentWindow,
                      public MenuBarModel,
                      public ApplicationCommandTarget
{
public:
    EditWindow();
    ~EditWindow();

    StringArray getMenuBarNames() override;
    PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& menuName) override;
    void menuItemSelected (int menuItemID, int topLevelMenuIndex) override;
    ApplicationCommandTarget* getNextCommandTarget() override;
    void getAllCommands (Array<CommandID>&) override;
    void getCommandInfo (CommandID, ApplicationCommandInfo&) override;
    bool perform (const InvocationInfo&) override;

    void setEdit (const ValueTree& edit);

    void saveState();
    void newProject();
    void openProject();
    void changeFile (File file);

private:
    PlaybackEngine playbackEngine;
    std::unique_ptr<EditView> editView;
    std::unique_ptr<Edit> currentEdit;
    std::unique_ptr<UndoManager> undoManager;
    jcf::ValueTreeDebugger editDebugger;

    File state;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditWindow)
};
