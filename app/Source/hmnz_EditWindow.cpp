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
    setResizeLimits(800, 600, std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    setContentOwned (new EditView(), false);
    centreWithSize (getWidth(), getHeight());
    setVisible (true);

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
        playbackEngine.setEdit (currentEdit.get());
        static_cast<EditView*> (getContentComponent())->setEdit (currentEdit.get());
    }
}
