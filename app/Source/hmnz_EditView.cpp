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
    addAndMakeVisible (arrangementView);
}

EditView::~EditView()
{
}

void EditView::setEdit (Edit* _edit)
{
    if (edit)
    {
        edit->getState().removeListener (this);
    }

    edit = _edit;

    if (edit)
    {
        transportView.setEdit (edit);
        arrangementView.setEdit (edit);
        edit->getState().addListener (this);
        keyboard = std::unique_ptr<MidiKeyboardComponent> (new MidiKeyboardComponent (
                edit->getMidiKeyboardState(), MidiKeyboardComponent::Orientation::horizontalKeyboard));
        addAndMakeVisible (*keyboard);
        keyboard->setKeyWidth(53);
        keyboard->setLowestVisibleKey(60);
        resized(); // Re-layout everything.
    }
}

void EditView::resized()
{
    FlexBox mainBox;
    mainBox.flexDirection = FlexBox::Direction::column;

    if (edit)
    {
        mainBox.items.add (FlexItem (transportView).withFlex (1.0f));
        mainBox.items.add (FlexItem (arrangementView).withFlex (1.0f));
        mainBox.items.add (FlexItem (*keyboard).withFlex (1.0f));
    }

    mainBox.performLayout (getLocalBounds());
}

void EditView::paint (Graphics& g)
{
}
