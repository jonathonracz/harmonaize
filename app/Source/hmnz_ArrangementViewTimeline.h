/*
  ==============================================================================

    hmnz_ArrangementViewTimeline.h
    Created: 9 Mar 2018 3:57:50pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ArrangementViewTimelineGrid.h"
#include "hmnz_ArrangementViewTrackLaneList.h"

class ArrangementViewTimeline   : public ArrangementViewComponent
{
public:
    ArrangementViewTimeline();

private:
    ArrangementViewTimelineGrid grid;
    ArrangementViewTrackLaneList trackLaneList;

    void editChanged (Edit* oldEdit) noexcept override;

    void resized() noexcept override;
};
