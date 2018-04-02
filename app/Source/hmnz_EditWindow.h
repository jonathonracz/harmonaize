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
                      public ApplicationCommandTarget,
                      public ValueTree::Listener
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

    void newProject();
    void openProject();
    void saveProject();
    void saveProjectAs();

private:
    File projectDiskLocation;
    bool modifiedSinceLastSave = false;
    std::unique_ptr<PlaybackEngine> playbackEngine;
    std::unique_ptr<Edit> currentEdit;
    std::unique_ptr<UndoManager> undoManager;
    jcf::ValueTreeDebugger editDebugger;

    bool attemptToCloseProject();
    void setEdit (const ValueTree& edit);
    void updateTitleBarText();

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override { jassertfalse; }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditWindow)
};
