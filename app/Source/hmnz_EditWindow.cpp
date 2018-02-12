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
    : DocumentWindow (HarmonaizeApplication::getInstance()->getApplicationName(),
                      Desktop::getInstance().getDefaultLookAndFeel().findColour (ResizableWindow::backgroundColourId),
                      DocumentWindow::allButtons)
{
    setUsingNativeTitleBar (true);
    setResizeLimits(800, 600, std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    currentEdit = std::unique_ptr<Edit> (new Edit (Edit::createDefaultState()));
    setContentOwned (new EditView(), false);
    static_cast<EditView*> (getContentComponent())->setEdit (currentEdit.get());
    centreWithSize (getWidth(), getHeight());
    setVisible (true);
}
