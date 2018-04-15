/*
  ==============================================================================

    hmnz_HintView.cpp
    Created: 12 Apr 2018 12:58:52pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_HintView.h"
#include "hmnz_HintStrings.h"

HintView::HintView()
    : Component (HintStrings::Name::HintViewer)
{
    setTooltip (HintStrings::Description::HintViewer);

    addAndMakeVisible (name);
    addAndMakeVisible (description);
}

void HintView::setComponentToWatch (Component* _componentToWatch)
{
    if (componentToWatch)
        componentToWatch->removeMouseListener (this);

    componentToWatch = _componentToWatch;
    if (componentToWatch)
        componentToWatch->addMouseListener (this, true);
}

void HintView::mouseEnter (const MouseEvent& event)
{
    setContentForComponent (event.eventComponent);
}

void HintView::mouseExit (const MouseEvent& event)
{
    setContentForComponent (nullptr);
}

void HintView::resized()
{
    int nameHeight = static_cast<int> (std::ceil (name.getFont().getHeight())) + name.getBorderSize().getTopAndBottom();
    name.setBounds (0, 0, getWidth(), nameHeight * 2);
    description.setBounds (0, name.getHeight(), getWidth(), getHeight() - name.getHeight());
}

void HintView::setContentForComponent (Component* component)
{
    while (component && component->getName().isEmpty())
        component = component->getParentComponent();

    if (!component)
    {
        name.setText ({}, NotificationType::dontSendNotification);
        description.setText ({}, NotificationType::dontSendNotification);
        return;
    }

    name.setText (component->getName(), NotificationType::dontSendNotification);
    if (TooltipClient* toolTipClient = dynamic_cast<TooltipClient*> (component))
        description.setText (toolTipClient->getTooltip(), NotificationType::dontSendNotification);
}
