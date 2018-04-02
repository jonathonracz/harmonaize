/*
  ==============================================================================

    hmnz_EditWindow.cpp
    Created: 29 Jan 2018 12:36:26am
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_EditWindow.h"

EditWindow::EditWindow()
    : DocumentWindow (HarmonaizeApplication::getApp().getApplicationName(),
                      Desktop::getInstance().getDefaultLookAndFeel().findColour (ResizableWindow::backgroundColourId),
                      DocumentWindow::TitleBarButtons::closeButton | DocumentWindow::TitleBarButtons::minimiseButton)
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
    editDebugger.addKeyListener (HarmonaizeApplication::getCommandManager().getKeyMappings());

    newProject();
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
    return {
        "File",
        "Edit",
        "View"
    };
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
        menu.addSeparator();
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
    const CommandID ids[] = {
        CommandIDs::newProject,
        CommandIDs::openProject,
        CommandIDs::saveProject,
        CommandIDs::saveProjectAs,
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
            result.setInfo ("New...", "Creates a new edit", category, 0);
            result.defaultKeypresses.add(KeyPress('n', ModifierKeys::commandModifier, 0));
            break;

        case CommandIDs::openProject:
            result.setInfo ("Open...", "Opens an existing edit", category, 0);
            result.defaultKeypresses.add (KeyPress ('o', ModifierKeys::commandModifier, 0));
            break;

        case CommandIDs::saveProject:
            result.setInfo ("Save", "Saves an existing edit", category, 0);
            result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::commandModifier, 0));
            break;

        case CommandIDs::saveProjectAs:
            result.setInfo ("Save As...", "Saves an existing edit with a new name", category, 0);
            result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::commandModifier | ModifierKeys::altModifier, 0));
            break;

        case CommandIDs::showPreferences:
            result.setInfo ("Preferences...", String(), category, 0);
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
        case CommandIDs::saveProject:
        {
            saveProject();
            break;
        }
        case CommandIDs::saveProjectAs:
        {
            saveProjectAs();
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
            if (currentEdit)
                undoManager->undo();

            break;
        }
        case CommandIDs::redo:
        {
            if (currentEdit)
                undoManager->redo();

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

void EditWindow::newProject()
{
    if (attemptToCloseProject())
    {
        setEdit (Edit::createDefaultState());
        projectDiskLocation = File();
        modifiedSinceLastSave = false;
        updateTitleBarText();
    }
}

void EditWindow::openProject()
{
    if (attemptToCloseProject())
    {
        FileChooser chooser(translate("Open Edit"), projectDiskLocation.getParentDirectory(), "*.hmnz");
        bool result = chooser.browseForFileToOpen();
        if (result)
        {
            std::unique_ptr<XmlElement> editElement(XmlDocument::parse(chooser.getResult()));
            if (editElement)
            {
                ValueTree editState = ValueTree::fromXml(*editElement);
                if (editState.isValid())
                {
                    setEdit (editState);
                    projectDiskLocation = chooser.getResult();
                    modifiedSinceLastSave = false;
                    updateTitleBarText();
                    return;
                }

                AlertWindow::showNativeDialogBox({}, translate("Could not open " + chooser.getResult().getFileName()), false);
            }

            AlertWindow::showNativeDialogBox({}, translate("Could not read " + chooser.getResult().getFileName()), false);
        }
    }
}

void EditWindow::saveProject()
{
    if (projectDiskLocation.getFullPathName().isEmpty())
    {
        saveProjectAs();
        return;
    }

    jassert (currentEdit);
    jassert (projectDiskLocation.getFullPathName().isNotEmpty());
    if (currentEdit && projectDiskLocation.getFullPathName().isNotEmpty())
    {
        std::unique_ptr<XmlElement> xml (currentEdit->getState().createXml());
        xml->writeToFile (projectDiskLocation, {});
        modifiedSinceLastSave = false;
        updateTitleBarText();
    }
}

void EditWindow::saveProjectAs()
{
    FileChooser chooser (translate ("Save Edit"), projectDiskLocation.getParentDirectory(), "*.hmnz");
    bool result = chooser.browseForFileToSave (true);
    if (result)
    {
        projectDiskLocation = chooser.getResult();
        saveProject();
    }
}

bool EditWindow::attemptToCloseProject()
{
    if (modifiedSinceLastSave)
    {
        int result = AlertWindow::showYesNoCancelBox (AlertWindow::AlertIconType::QuestionIcon, String(), translate ("The project has been modified. Save changes?"));
        switch (result)
        {
            case 0: return false;
            case 1: saveProject();
            default: break;
        }
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
        currentEdit->getState().removeListener (this);
        currentEdit.reset();
        undoManager.reset();
    }

    undoManager = std::unique_ptr<UndoManager> (new UndoManager);

    if (edit.getType() == IDs::Edit)
    {
        currentEdit = std::unique_ptr<Edit> (new Edit (edit, undoManager.get()));
    }
    else
    {
        AlertWindow::showNativeDialogBox ({}, translate ("Could not open edit."), false);
        ValueTree defaultEdit = Edit::createDefaultState();
        currentEdit = std::unique_ptr<Edit> (new Edit (defaultEdit, undoManager.get()));
    }

    currentEdit->getState().addListener (this);
    editDebugger.setSource (currentEdit->getState());
    editDebugger.setVisible (false);
    playbackEngine = std::unique_ptr<PlaybackEngine> (new PlaybackEngine (*currentEdit));
    setContentOwned (new EditView (*currentEdit), false);
}

void EditWindow::updateTitleBarText()
{
    String projectName = (projectDiskLocation.existsAsFile()) ? projectDiskLocation.getFileNameWithoutExtension() : translate ("Untitled");
    if (modifiedSinceLastSave)
        projectName += "*";

    setName (projectName);
}

void EditWindow::valueTreePropertyChanged (ValueTree& tree, const Identifier&)
{
    if (tree.getType() != IDs::Transport)
    {
        modifiedSinceLastSave = true;
        updateTitleBarText();
    }
}
