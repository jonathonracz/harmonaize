/*
  ==============================================================================

    hmnz_ArrangementView.cpp
    Created: 4 Mar 2018 11:27:08pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementView.h"
#include "hmnz_ArrangementViewModel.h"
#include "hmnz_Edit.h"

void ArrangementView::setEdit (Edit* _edit)
{
    if (edit)
        edit->getState().removeListener (this);

    edit = _edit;

    if (edit)
        edit->getState().addListener (this);
}

void ArrangementView::resized() noexcept
{

}

void ArrangementView::paint (Graphics& g) noexcept
{
    if (!edit)
        return;

    ArrangementViewModel& model = edit->arrangementViewModel;
    int numLinesToDraw = static_cast<int> (model.timeEnd - model.timeStart);
    int linePixelDelta = getWidth() / numLinesToDraw;
    int lineStartOffset = linePixelDelta * (model.timeStart - std::floor (model.timeStart));

    g.setColour (Colours::white);
    for (int i = 0; i < numLinesToDraw; ++i)
    {
        int xPos = lineStartOffset + (i * linePixelDelta);
        g.fillRect (Rectangle<int> (2, getHeight()).withCentre (Point<int> (xPos, getHeight() / 2)));
    }
}

void ArrangementView::valueTreePropertyChanged (ValueTree& tree, const Identifier& property) noexcept
{
    if (tree == edit->arrangementViewModel.getState())
    {
        repaint();
    }
}
