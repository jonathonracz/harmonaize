/*
  ==============================================================================

    hmnz_Utility.h
    Created: 30 Jan 2018 12:11:42am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#define HMNZ_ASSERT_IS_ON_MESSAGE_THREAD \
    jassert (MessageManager::getInstance()->currentThreadHasLockedMessageManager());
