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

    setEdit (new Edit (Edit::createDefaultState()));
}

EditWindow::~EditWindow()
{
    setEdit (nullptr);
}

void EditWindow::setEdit (Edit* edit) noexcept
{
    if (currentEdit.get())
    {
        static_cast<EditView*> (getContentComponent())->setEdit (nullptr);
        playbackEngine.setEdit (edit);
    }

    if (edit)
    {
        currentEdit = std::unique_ptr<Edit> (edit);
        editDebugger.setSource (currentEdit.get()->getState());
        playbackEngine.setEdit (currentEdit.get());
        static_cast<EditView*> (getContentComponent())->setEdit (currentEdit.get());
    }
}
