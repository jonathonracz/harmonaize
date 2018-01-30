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
}

EditView::~EditView()
{
}

void EditView::setEdit (Edit* edit)
{
    currentEdit = edit;
}

void EditView::resized()
{

}

void EditView::paint (Graphics& g)
{
    if (currentEdit)
        g.fillAll (Colours::red);
    else
        g.fillAll (Colours::blue);
}
