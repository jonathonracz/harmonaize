/*
  ==============================================================================

    hmnz_EditView.cpp
    Created: 30 Jan 2018 1:12:54pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_EditView.h"

EditView::EditView()
{
    addAndMakeVisible (transportView);
}

EditView::~EditView()
{
}

void EditView::setEdit (Edit* _edit)
{
    if (edit)
        edit->getState().removeListener (this);

    edit = _edit;

    if (edit)
    {
        transportView.setTransport (edit->transport.get());
        edit->getState().addListener (this);
    }
}

void EditView::resized()
{
    FlexBox mainBox;
    mainBox.flexDirection = FlexBox::Direction::row;

    mainBox.items.add (FlexItem (transportView).withFlex (1.0f));

    mainBox.performLayout (getLocalBounds());
}

void EditView::paint (Graphics& g)
{
    if (edit)
        g.fillAll (Colours::red);
    else
        g.fillAll (Colours::blue);
}
