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
    setResizable (true, false);
    centreWithSize (getWidth(), getHeight());
    setVisible (true);
    
    setApplicationCommandManagerToWatch (&HarmonaizeApplication::getCommandManager());

#if JUCE_MAC
    setMacMainMenu (this);
#else
    setMenuBar (this);
#endif

    HarmonaizeApplication::getCommandManager().setFirstCommandTarget (this);
    addKeyListener (HarmonaizeApplication::getCommandManager().getKeyMappings());
    editDebugger.addKeyListener(HarmonaizeApplication::getCommandManager().getKeyMappings());

    ValueTree defaultEdit = Edit::createDefaultState();
    setEdit (defaultEdit);
}

EditWindow::~EditWindow()
{
    setEdit (ValueTree());

#if JUCE_MAC
    setMacMainMenu (nullptr);
#else
    setMenuBar (nullptr);
#endif
}

StringArray EditWindow::getMenuBarNames()
{
    StringArray names;
    names.add ("File");
    names.add ("Edit");
    names.add ("View");
    return names;
}

PopupMenu EditWindow::getMenuForIndex (int topLevelMenuIndex, const String& /*menuName*/)
{
    PopupMenu extraAppleMenuItems;
    extraAppleMenuItems.addCommandItem (&HarmonaizeApplication::getCommandManager(), CommandIDs::showPreferences);
    setMacMainMenu (this, &extraAppleMenuItems);
    
    PopupMenu menu;
    
    if (topLevelMenuIndex == 0)
    {
        // "File" menu
        menu.addCommandItem (&HarmonaizeApplication::getCommandManager(), CommandIDs::newProject);
        menu.addCommandItem (&HarmonaizeApplication::getCommandManager(), CommandIDs::openProject);
    }
    else if (topLevelMenuIndex == 1)
    {
        // "Edit" menu
        PopupMenu editMenu;
        menu.addCommandItem (&HarmonaizeApplication::getCommandManager(), CommandIDs::undo);
        menu.addCommandItem (&HarmonaizeApplication::getCommandManager(), CommandIDs::redo);
    }

    else if (topLevelMenuIndex == 2)
    {
        PopupMenu viewMenu;
        menu.addCommandItem (&HarmonaizeApplication::getCommandManager(), CommandIDs::scaleUp);
        menu.addCommandItem (&HarmonaizeApplication::getCommandManager(), CommandIDs::scaleDown);
    }
    
    return menu;
}

void EditWindow::menuItemSelected (int menuItemID, int /*topLevelMenuIndex*/)
{
    
}

void EditWindow::getAllCommands (Array<CommandID>& commands)
{
    // this returns the set of all commands that this target can perform..
    const CommandID ids[] = { CommandIDs::newProject,
        CommandIDs::openProject,
        CommandIDs::showPreferences,
        CommandIDs::showDebugger,
        CommandIDs::undo,
        CommandIDs::redo,
        CommandIDs::scaleUp,
        CommandIDs::scaleDown
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

        case CommandIDs::scaleUp:
            result.setInfo ("Scale Up", String(), category, 0);
            result.addDefaultKeypress ('=', ModifierKeys::commandModifier);
            break;

        case CommandIDs::scaleDown:
            result.setInfo ("Scale Down", String(), category, 0);
            result.addDefaultKeypress ('-', ModifierKeys::commandModifier);
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
            newProject();
            break;
        }
        case CommandIDs::openProject:
        {
            openProject();
            break;
        }
        case CommandIDs::showPreferences:
        {
            if (HarmonaizeApplication::getApp().preferencesView->isVisible())
                HarmonaizeApplication::getApp().preferencesView->setVisible (false);
            else
            {
                HarmonaizeApplication::getApp().preferencesView->setVisible (true);
                HarmonaizeApplication::getApp().preferencesView->grabKeyboardFocus();
            }
            break;
        }
        case CommandIDs::showDebugger:
        {
        #if JUCE_DEBUG
            editDebugger.setVisible (!editDebugger.isVisible());
            break;
        #endif
        }
        case CommandIDs::undo:
        {
            break;
        }
        case CommandIDs::redo:
        {
            break;
        }
        case CommandIDs::scaleUp:
        {
            HarmonaizeApplication::getApp().preferencesView->scaleUp();
            break;
        }
        case CommandIDs::scaleDown:
        {
            HarmonaizeApplication::getApp().preferencesView->scaleDown();
            break;
        }
        default:
            return false;
    }
    
    return true;
}

void EditWindow::setEdit (const ValueTree& edit)
{
    if (currentEdit.get())
    {
        clearContentComponent();
        playbackEngine.reset();
        ValueTree nullTree;
        editDebugger.setSource (nullTree);
        currentEdit.reset();
        undoManager.reset();
    }

    if (edit.getType() == IDs::Edit)
    {
        undoManager = std::unique_ptr<UndoManager> (new UndoManager);
        currentEdit = std::unique_ptr<Edit> (new Edit (edit, undoManager.get()));
        editDebugger.setSource (currentEdit->getState());
        editDebugger.setVisible (false);
        playbackEngine = std::unique_ptr<PlaybackEngine> (new PlaybackEngine (*currentEdit));
        setContentOwned (new EditView (*currentEdit), false);
    }
}

void EditWindow::saveState()
{
    if (state.exists())
    {
        ValueTree currentState = currentEdit.get()->getState().createCopy();
        ValueTree transport = currentState.getChildWithName("Transport");
        transport.setProperty ("PlayState", 0, nullptr);
        transport.setProperty ("RecordEnabled", 0, nullptr);
        XmlElement* xml = currentState.createXml();
        xml->writeToFile (state, "");
        delete xml;
    }
}

void EditWindow::newProject()
{
    FileChooser fileChooser ("New Project");
    fileChooser.browseForFileToSave (true);
    state = fileChooser.getResult();
    state.create();
}

void EditWindow::openProject()
{
    FileChooser fileChooser ("Open Project");
    fileChooser.browseForFileToOpen();
    File file = fileChooser.getResult();
    XmlElement* e = XmlDocument::parse (file);
    ValueTree valueTree = ValueTree::fromXml (*e);
    delete e;
    setEdit(valueTree);
    state = file;
}

void EditWindow::changeFile (File file)
{
    state = file;
}

void EditWindow::valueTreePropertyChanged (ValueTree& tree, const Identifier&)
{
    if (tree != currentEdit->transport.getState())
        saveState();
}

void EditWindow::valueTreeChildAdded (ValueTree&, ValueTree&)
{
    saveState();
}

void EditWindow::valueTreeChildRemoved (ValueTree&, ValueTree&, int)
{
    saveState();
}

void EditWindow::valueTreeChildOrderChanged (ValueTree&, int, int)
{
    saveState();
}
