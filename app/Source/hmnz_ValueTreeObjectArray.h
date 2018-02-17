/*
  ==============================================================================

    hmnz_ValueTreeObjectArray.h
    Created: 22 Jan 2018 2:19:54pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"

/**
    Generic list of ValueTree-backed objects. Originally based on code by David
    Rowland (drowaudio).
*/
template<class ObjectType, class CriticalSectionType = DummyCriticalSection>
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
        const ScopedLockType sl (arrayLock);
        while (objects.size() > 0)
            deleteObject (objects.removeAndReturn (objects.size() - 1));
    }

    bool isChildTree (ValueTree& v) const
    {
        return isSuitableType (v) && v.getParent() == parent;
    }

    int indexOf (const ValueTree& v) const noexcept
    {
        for (int i = 0; i < objects.size(); ++i)
        if (objects.getUnchecked (i)->getState() == v)
        return i;

        return -1;
    }

    ObjectType* objectWithState (const ValueTree& state) const
    {
        for (ObjectType* object : objects)
        {
            if (object->getState() == state)
                return object;
        }

        return nullptr;
    }

    int compareElements (ObjectType* first, ObjectType* second) const
    {
        int index1 = parent.indexOf (first->getState());
        int index2 = parent.indexOf (second->getState());
        return index1 - index2;
    }

    Array<ObjectType*> objects;

protected:
    ValueTree parent;
    UndoManager* undoManager;
    CriticalSectionType arrayLock;
    typedef typename CriticalSectionType::ScopedLockType ScopedLockType;

    // Call this in your derived constructor.
    void addObjects()
    {
        for (const auto& v : parent)
            if (isSuitableType (v))
                if (ObjectType* newObject = createNewObject (v, undoManager))
                    objects.add (newObject);
    }

    virtual ObjectType* createNewObject (const ValueTree& v, UndoManager* um) = 0;
    virtual void deleteObject (ObjectType* object) { delete object; }

    virtual void newObjectAdded (ObjectType*) {}
    virtual void objectRemoved (ObjectType*) {}
    virtual void objectOrderChanged() {}

private:
    bool isSuitableType (const ValueTree& v) const
    {
        return ObjectType::identifier == v.getType();
    }

    void sortArray()
    {
        objects.sort (*this);
    }

    void valueTreeChildAdded (ValueTree&, ValueTree& tree) override
    {
        if (isChildTree (tree))
        {
            const int index = parent.indexOf (tree);
            (void) index;
            jassert (index >= 0);

            if (ObjectType* newObject = createNewObject (tree, undoManager))
            {
                {
                    const ScopedLockType sl (arrayLock);

                    if (index == parent.getNumChildren() - 1)
                        objects.add (newObject);
                    else
                        objects.addSorted (*this, newObject);
                }

                newObjectAdded (newObject);
            }
            else
                jassertfalse;
        }
    }

    void valueTreeChildRemoved (ValueTree& exParent, ValueTree& tree, int) override
    {
        if (parent == exParent && isSuitableType (tree))
        {
            const int oldIndex = indexOf (tree);

            if (oldIndex >= 0)
            {
                ObjectType* o;

                {
                    const ScopedLockType sl (arrayLock);
                    o = objects.removeAndReturn (oldIndex);
                }

                objectRemoved (o);
                deleteObject (o);
            }
        }
    }

    void valueTreeChildOrderChanged (ValueTree& tree, int, int) override
    {
        if (tree == parent)
        {
            {
                const ScopedLockType sl (arrayLock);
                sortArray();
            }

            objectOrderChanged();
        }
    }

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override {}
    void valueTreeParentChanged (ValueTree&) override {}
    void valueTreeRedirected (ValueTree&) override { jassertfalse; } // may need to add handling if this is hit

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ValueTreeObjectArray)
};
