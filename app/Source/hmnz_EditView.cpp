/*
  ==============================================================================

    hmnz_EditView.cpp
    Created: 30 Jan 2018 1:12:54pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_EditView.h"
#include "hmnz_HintStrings.h"

EditView::EditView (Edit& _edit)
    : Component (HintStrings::Name::Edit),
      edit (_edit), transportView (_edit), arrangementView (_edit),
      keyboard (new MidiKeyboardComponentWithHint (_edit.getMidiKeyboardState(), MidiKeyboardComponent::Orientation::horizontalKeyboard))
{
    setTooltip (HintStrings::Description::Edit);

    addAndMakeVisible (hintView);
    addAndMakeVisible (transportView);
    addAndMakeVisible (arrangementView);
    addAndMakeVisible (keyboard.get());

    keyboard->setName (HintStrings::Name::VirtualKeyboard);
    keyboard->setTooltip (HintStrings::Description::VirtualKeyboard);

    hintView.setComponentToWatch (this);
    keyboard->setKeyWidth (53);
    keyboard->setLowestVisibleKey (60);
}

EditView::~EditView()
{
}

void EditView::resized()
{
    FlexBox mainBox;
    mainBox.flexDirection = FlexBox::Direction::column;

    FlexBox bottomBox;
    bottomBox.items.add (FlexItem (hintView).withWidth (120));
    bottomBox.items.add (FlexItem (*keyboard).withFlex (1.0f));

    mainBox.items.add (FlexItem (transportView).withFlex (1.0f));
    mainBox.items.add (FlexItem (arrangementView).withFlex (1.0f));
    mainBox.items.add (FlexItem (bottomBox).withFlex (1.0f));

    mainBox.performLayout (getLocalBounds());
}

void EditView::paint (Graphics& g)
{
}
