/*
  ==============================================================================

    hmnz_Utility.h
    Created: 30 Jan 2018 12:11:42am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "hmnz_VariantConverters.h"

#define HMNZ_IS_ON_MESSAGE_THREAD \
    MessageManager::getInstance()->currentThreadHasLockedMessageManager()

#define HMNZ_ASSERT_IS_ON_MESSAGE_THREAD \
    jassert (HMNZ_IS_ON_MESSAGE_THREAD);

#define HMNZ_ASSERT_IS_NOT_ON_MESSAGE_THREAD \
    jassert (!HMNZ_IS_ON_MESSAGE_THREAD);

namespace Utility
{

template<typename Type>
void writeBackDefault (CachedValue<Type>& value)
{
    if (value.isUsingDefault())
        value = value.getDefault();
}

}
