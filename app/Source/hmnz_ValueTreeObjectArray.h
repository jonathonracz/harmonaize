/*
  ==============================================================================

    hmnz_ValueTreeObjectArray.h
    Created: 22 Jan 2018 2:19:54pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_ArrayIterator.h"

/**
    Generic list of ValueTree-backed objects based on ValueTreeObject.
    Originally based on code by David Rowland (drowaudio).
*/
template<class ObjectType, class CriticalSectionType>
class ValueTreeObjectArray   : public ValueTree::Listener
{
public:
    ValueTreeObjectArray (const ValueTree& parentTree, UndoManager* um)
        : parent (parentTree), undoManager (um)
    {
        parent.addListener (this);
    }

    virtual ~ValueTreeObjectArray()
    {
        parent.removeListener (this);
        const ScopedLockType sl (arrayLock);
        while (objects.size() > 0)
            deleteObject (objects.removeAndReturn (objects.size() - 1));
    }

    ValueTree& getParent() const
    {
        return parent;
    }

    UndoManager* getUndoManager() const
    {
        return undoManager;
    }

    bool isChildTree (const ValueTree& v) const
    {
        return isSuitableType (v) && v.getParent() == parent;
    }

    int indexOf (ObjectType* obj) const
    {
        const ScopedLockType sl (arrayLock);
        return objects.indexOf (obj);
    }

    int indexOfStateInObjects (const ValueTree& v) const
    {
        for (int i = 0; i < objects.size(); ++i)
            if (objects.getUnchecked (i)->getState() == v)
                return i;

        return -1;
    }

    int indexOfObjectInState (ObjectType* object) const
    {
        return (object != nullptr) ? parent.indexOf (object->getState()) : -1;
    }

    ObjectType* objectWithState (const ValueTree& state) const
    {
        const ScopedLockType sl (arrayLock);
        for (ObjectType* object : objects)
        {
            if (object->getState() == state)
                return object;
        }

        return nullptr;
    }

    void addState (const ValueTree& v)
    {
        insertState (v, -1);
    }

    void insertState (const ValueTree& v, int index)
    {
        parent.addChild (v, index, undoManager);
    }

    void insertStateAtObjectIndex (const ValueTree& v, int index)
    {
        const ScopedLockType sl (arrayLock);
        jassert (index <= objects.size());
        if (index < 0 || index >= objects.size())
        {
            insertState (v, -1);
            return;
        }

        int stateIndex = indexOfObjectInState (objects[index]);
        insertState (v, stateIndex);
    }

    void removeObject (ObjectType* object)
    {
        parent.removeChild (object->getState(), undoManager);
    }

    int size() const
    {
        const ScopedLockType sl (arrayLock);
        return objects.size();
    }

    ObjectType* operator[] (int index) const
    {
        const ScopedLockType sl (arrayLock);
        return objects[index];
    }

    ObjectType* getFirst() const
    {
        const ScopedLockType sl (arrayLock);
        return objects.getFirst();
    }

    ObjectType* getLast() const
    {
        const ScopedLockType sl (arrayLock);
        return objects.getLast();
    }

    jdr::ArrayForwardIteratorConst<ObjectType*> begin() const
    {
        return jdr::ArrayForwardIteratorConst<ObjectType*>::begin (objects);
    }

    jdr::ArrayForwardIteratorConst<ObjectType*> end() const
    {
        return jdr::ArrayForwardIteratorConst<ObjectType*>::end (objects);
    }

    const CriticalSectionType& getLock() const
    {
        return arrayLock;
    }

    using ScopedLockType = typename CriticalSectionType::ScopedLockType;

protected:
    Array<ObjectType*> objects;
    ValueTree parent;
    UndoManager* undoManager;
    CriticalSectionType arrayLock;

    // Call this in your derived constructor.
    void addObjects()
    {
        for (const auto& v : parent)
            if (isSuitableType (v))
                if (ObjectType* newObject = createNewObject (v, undoManager))
                    objects.add (newObject);
    }

    bool isSuitableType (const ValueTree& v) const
    {
        return ObjectType::identifier == v.getType();
    }

    virtual ObjectType* createNewObject (const ValueTree& v, UndoManager* um) = 0;

    virtual void deleteObject (ObjectType* object)
    {
        delete object;
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ValueTreeObjectArray)
};
