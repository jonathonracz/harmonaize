/*
  ==============================================================================

    hmnz_EditWindow.cpp
    Created: 29 Jan 2018 12:36:26am
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_EditWindow.h"
#include "hmnz_Application.h"

EditWindow::EditWindow()
    : DocumentWindow (HarmonaizeApplication::getApp().getApplicationName(),
                      Desktop::getInstance().getDefaultLookAndFeel().findColour (ResizableWindow::backgroundColourId),
                      DocumentWindow::allButtons)
{
    setUsingNativeTitleBar (true);
    setResizeLimits (800, 600, std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    setContentOwned (new EditView(), false);
    centreWithSize (getWidth(), getHeight());
    setVisible (true);
    
    setApplicationCommandManagerToWatch (&HarmonaizeApplication::getCommandManager());
    #if JUCE_MAC
    setMacMainMenu (this);
    #endif
    HarmonaizeApplication::getCommandManager().setFirstCommandTarget (this);
    addKeyListener (HarmonaizeApplication::getCommandManager().getKeyMappings());

    ValueTree defaultEdit = Edit::createDefaultState();
    setEdit (defaultEdit);
}

EditWindow::~EditWindow()
{
    setEdit (ValueTree());
}

void EditWindow::setEdit (const ValueTree& edit)
{
    if (currentEdit.get())
    {
        static_cast<EditView*> (getContentComponent())->setEdit (nullptr);
        playbackEngine.setEdit (nullptr);
        ValueTree nullTree;
        editDebugger.setSource (nullTree);
        currentEdit.reset();
        undoManager.reset();
    }

    if (edit.getType() == IDs::Edit)
    {
        undoManager = std::unique_ptr<UndoManager> (new UndoManager);
        Edit* newEdit = new Edit (edit, undoManager.get());
        currentEdit = std::unique_ptr<Edit> (newEdit);
        editDebugger.setSource (currentEdit->getState());
        editDebugger.setVisible (false);
        playbackEngine.setEdit (currentEdit.get());
        static_cast<EditView*> (getContentComponent())->setEdit (currentEdit.get());
    }
}

StringArray EditWindow::getMenuBarNames()
{
    StringArray names;
    names.add ("File");
    names.add ("Edit");
    return names;
}

PopupMenu EditWindow::getMenuForIndex (int topLevelMenuIndex, const String& /*menuName*/)
{
    PopupMenu extraAppleMenuItems;
    extraAppleMenuItems.addCommandItem (&HarmonaizeApplication::getCommandManager(), CommandIDs::showPreferences);
//    extraAppleMenuItems.addCommandItem (&HarmonaizeApplication::getCommandManager(), CommandIDs::showDebugger);
    setMacMainMenu (this, &extraAppleMenuItems);
    
    PopupMenu menu;
    
    if (topLevelMenuIndex == 0)
    {
        // "File" menu
        menu.addCommandItem (&HarmonaizeApplication::getCommandManager(), CommandIDs::newProject);
        menu.addCommandItem (&HarmonaizeApplication::getCommandManager(), CommandIDs::openProject);
        
        menu.addCommandItem (&HarmonaizeApplication::getCommandManager(), CommandIDs::saveProject);
        menu.addCommandItem (&HarmonaizeApplication::getCommandManager(), CommandIDs::saveProjectAs);
    }
    else if (topLevelMenuIndex == 1)
    {
        // "Edit" menu
        PopupMenu editMenu;
        menu.addCommandItem (&HarmonaizeApplication::getCommandManager(), CommandIDs::undo);
        menu.addCommandItem (&HarmonaizeApplication::getCommandManager(), CommandIDs::redo);
    }
    
    return menu;
}

void EditWindow::menuItemSelected (int menuItemID, int /*topLevelMenuIndex*/)
{
    if (menuItemID == 250)
    {
    }
    else if (menuItemID >= 100 && menuItemID < 200)
    {
    }
    else
    {
    }
}

void EditWindow::getAllCommands (Array<CommandID>& commands)
{
    // this returns the set of all commands that this target can perform..
    const CommandID ids[] = { CommandIDs::newProject,
        CommandIDs::openProject,
        CommandIDs::saveProject,
        CommandIDs::saveProjectAs,
        CommandIDs::showPreferences,
        CommandIDs::showDebugger,
        CommandIDs::undo,
        CommandIDs::redo
    };
    
    commands.addArray (ids, numElementsInArray (ids));
}

ApplicationCommandTarget* EditWindow::getNextCommandTarget()
{
    return findFirstTargetParentComponent();
}

void EditWindow::getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result)
{
    const String category ("General");
    
    switch (commandID)
    {
        case CommandIDs::newProject:
            result.setInfo ("New", "Creates a new filter graph file", category, 0);
            result.defaultKeypresses.add(KeyPress('n', ModifierKeys::commandModifier, 0));
            break;
            
        case CommandIDs::openProject:
            result.setInfo ("Open...", "Opens a filter graph file", category, 0);
            result.defaultKeypresses.add (KeyPress ('o', ModifierKeys::commandModifier, 0));
            break;
            
        case CommandIDs::saveProject:
            result.setInfo ("Save", "Saves the current graph to a file", category, 0);
            result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::commandModifier, 0));
            break;
            
        case CommandIDs::saveProjectAs:
            result.setInfo ("Save As...",
                            "Saves a copy of the current graph to a file",
                            category, 0);
            result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::shiftModifier | ModifierKeys::commandModifier, 0));
            break;
            
        case CommandIDs::showPreferences:
            result.setInfo ("Preferences", String(), category, 0);
            result.addDefaultKeypress ('p', ModifierKeys::commandModifier);
            break;
            
        case CommandIDs::showDebugger:
            result.setInfo ("Debugger", String(), category, 0);
            result.addDefaultKeypress('d', ModifierKeys::commandModifier);
            break;
            
        case CommandIDs::undo:
            result.setInfo ("Undo", String(), category, 0);
            result.addDefaultKeypress ('z', ModifierKeys::commandModifier);
            break;
            
        case CommandIDs::redo:
            result.setInfo ("Redo", String(), category, 0);
            result.defaultKeypresses.add (KeyPress ('z', ModifierKeys::shiftModifier | ModifierKeys::commandModifier, 0));
            break;
            
        default:
            break;
    }
}

bool EditWindow::perform (const InvocationInfo& info)
{
    switch (info.commandID)
    {
        case CommandIDs::newProject:
        {
            //            if (graphHolder != nullptr && graphHolder->graph != nullptr && graphHolder->graph->saveIfNeededAndUserAgrees() == FileBasedDocument::savedOk)
            //                graphHolder->graph->newDocument();
            break;
        }
        case CommandIDs::openProject:
        {
            //            if (graphHolder != nullptr && graphHolder->graph != nullptr && graphHolder->graph->saveIfNeededAndUserAgrees() == FileBasedDocument::savedOk)
            //                graphHolder->graph->loadFromUserSpecifiedFile (true);
            break;
        }
        case CommandIDs::saveProject:
        {
            //            if (graphHolder != nullptr && graphHolder->graph != nullptr)
            //                graphHolder->graph->save (true, true);
            break;
        }
        case CommandIDs::saveProjectAs:
        {
            //            if (graphHolder != nullptr && graphHolder->graph != nullptr)
            //                graphHolder->graph->saveAs (File(), true, true, true);
            break;
        }
        case CommandIDs::showPreferences:
        {
            if (HarmonaizeApplication::getApp().preferencesView->isVisible())
                HarmonaizeApplication::getApp().preferencesView->setVisible (false);
            else
                HarmonaizeApplication::getApp().preferencesView->setVisible (true);
            break;
        }
        case CommandIDs::showDebugger:
        {
            if (editDebugger.isVisible())
                editDebugger.setVisible (false);
            else
                editDebugger.setVisible (true);
            break;
        }
        case CommandIDs::undo:
        {
            break;
        }
        case CommandIDs::redo:
        {
            break;
        }
        default:
            return false;
    }
    
    return true;
}
