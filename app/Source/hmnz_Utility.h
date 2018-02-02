/*
  ==============================================================================

    hmnz_Utility.h
    Created: 30 Jan 2018 12:11:42am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#define HMNZ_IS_ON_MESSAGE_THREAD \
    MessageManager::getInstance()->currentThreadHasLockedMessageManager()

#define HMNZ_ASSERT_IS_ON_MESSAGE_THREAD \
    jassert (HMNZ_IS_ON_MESSAGE_THREAD);

#define HMNZ_ASSERT_IS_NOT_ON_MESSAGE_THREAD \
    jassert (!HMNZ_IS_ON_MESSAGE_THREAD);

namespace Utility
{

template<typename Type>
void writeBackDefaultValueIfNotThere (CachedValue<Type>& value, UndoManager* um = nullptr)
{
    if (value.isUsingDefault())
        value.getValueTree().setProperty (value.getPropertyID(), value.getDefault(), um);
}

}
