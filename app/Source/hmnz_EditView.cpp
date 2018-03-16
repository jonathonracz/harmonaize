/*
  ==============================================================================

    hmnz_EditView.cpp
    Created: 30 Jan 2018 1:12:54pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_EditView.h"

EditView::EditView (Edit& _edit)
    : edit (_edit), transportView (_edit), arrangementView (_edit),
      keyboard (new MidiKeyboardComponent (_edit.getMidiKeyboardState(), MidiKeyboardComponent::Orientation::horizontalKeyboard))
{
    addAndMakeVisible (transportView);
    addAndMakeVisible (arrangementView);
    addAndMakeVisible (keyboard.get());

    keyboard->setKeyWidth(53);
    keyboard->setLowestVisibleKey(60);
}

EditView::~EditView()
{
}

void EditView::resized()
{
    FlexBox mainBox;
    mainBox.flexDirection = FlexBox::Direction::column;

    mainBox.items.add (FlexItem (transportView).withFlex (1.0f));
    mainBox.items.add (FlexItem (arrangementView).withFlex (1.0f));
    mainBox.items.add (FlexItem (*keyboard).withFlex (1.0f));

    mainBox.performLayout (getLocalBounds());
}

void EditView::paint (Graphics& g)
{
}
