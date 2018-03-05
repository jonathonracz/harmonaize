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
    Generic list of ValueTree-backed objects. Originally based on code by David
    Rowland (drowaudio).
*/
template<class ObjectType, class CriticalSectionType = DummyCriticalSection>
class ValueTreeObjectArray   : public ValueTree::Listener
{
public:
    struct Listener
    {
        virtual ~Listener() = default;
        virtual void objectAdded (ObjectType* object, ValueTreeObjectArray<ObjectType, CriticalSectionType>* array) {}
        virtual void objectRemoved (ObjectType* object, ValueTreeObjectArray<ObjectType, CriticalSectionType>* array) {}
        virtual void objectOrderChanged (ValueTreeObjectArray<ObjectType, CriticalSectionType>* array) {}
    };

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

    void addListener (Listener* listener) noexcept
    {
        listeners.add (listener);
    }

    void removeListener (Listener* listener) noexcept
    {
        listeners.remove (listener);
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

    int indexOf (ObjectType* obj) const noexcept
    {
        const ScopedLockType sl (arrayLock);
        return objects.indexOf (obj);
    }

    int indexOfStateInObjects (const ValueTree& v) const noexcept
    {
        for (int i = 0; i < objects.size(); ++i)
            if (objects.getUnchecked (i)->getState() == v)
                return i;

        return -1;
    }

    int indexOfObjectInState (ObjectType* object) const noexcept
    {
        return (object != nullptr) ? parent.indexOf (object->getState()) : -1;
    }

    ObjectType* objectWithState (const ValueTree& state) const noexcept
    {
        const ScopedLockType sl (arrayLock);
        for (ObjectType* object : objects)
        {
            if (object->getState() == state)
                return object;
        }

        return nullptr;
    }

    void addState (const ValueTree& v) noexcept
    {
        insertState (v, -1);
    }

    void insertState (const ValueTree& v, int index) noexcept
    {
        parent.addChild (v, index, undoManager);
    }

    void insertStateAtObjectIndex (const ValueTree& v, int index) noexcept
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

    void removeObject (ObjectType* object) noexcept
    {
        parent.removeChild (object->getState(), undoManager);
    }

    int size() const noexcept
    {
        const ScopedLockType sl (arrayLock);
        return objects.size();
    }

    ObjectType* operator[] (int index) const noexcept
    {
        const ScopedLockType sl (arrayLock);
        return objects[index];
    }

    ObjectType* getFirst() const noexcept
    {
        const ScopedLockType sl (arrayLock);
        return objects.getFirst();
    }

    ObjectType* getLast() const noexcept
    {
        const ScopedLockType sl (arrayLock);
        return objects.getLast();
    }

    jdr::ArrayForwardIteratorConst<ObjectType*> begin() const noexcept
    {
        return jdr::ArrayForwardIteratorConst<ObjectType*>::begin (objects);
    }

    jdr::ArrayForwardIteratorConst<ObjectType*> end() const noexcept
    {
        return jdr::ArrayForwardIteratorConst<ObjectType*>::end (objects);
    }

    int compareElements (ObjectType* first, ObjectType* second) const noexcept
    {
        int index1 = parent.indexOf (first->getState());
        int index2 = parent.indexOf (second->getState());
        return index1 - index2;
    }

    const CriticalSectionType& getLock() const noexcept
    {
        return arrayLock;
    }

    using ScopedLockType = typename CriticalSectionType::ScopedLockType;

protected:
    // Call this in your derived constructor.
    void addObjects() noexcept
    {
        for (const auto& v : parent)
            if (isSuitableType (v))
                if (ObjectType* newObject = createNewObject (v, undoManager))
                    objects.add (newObject);
    }

    virtual ObjectType* createNewObject (const ValueTree& v, UndoManager* um) = 0;
    virtual void deleteObject (ObjectType* object) { delete object; }

private:
    Array<ObjectType*> objects;
    ValueTree parent;
    UndoManager* undoManager;
    CriticalSectionType arrayLock;
    ListenerList<Listener> listeners;

    bool isSuitableType (const ValueTree& v) const noexcept
    {
        return ObjectType::identifier == v.getType();
    }

    void sortArray() noexcept
    {
        objects.sort (*this);
    }

    void valueTreeChildAdded (ValueTree& treeChildAddedTo, ValueTree& tree) override
    {
        if (treeChildAddedTo == parent)
        {
            ObjectType* addedObject = nullptr;
            int indexOfAlreadyExistingObject = indexOfStateInObjects (tree);
            jassert (isChildTree (tree));

            if (indexOfAlreadyExistingObject < 0)
            {
                const int index = parent.indexOf (tree);
                (void) index;
                jassert (index >= 0);

                addedObject = createNewObject (tree, undoManager);

                {
                    const ScopedLockType sl (arrayLock);

                    if (index == parent.getNumChildren() - 1)
                        objects.add (addedObject);
                    else
                        objects.addSorted (*this, addedObject);
                }
            }
            else
            {
                addedObject = objects[indexOfAlreadyExistingObject];
            }

            jassert (addedObject);
            listeners.call (&Listener::objectAdded, addedObject, this);
        }
    }

    void valueTreeChildRemoved (ValueTree& exParent, ValueTree& tree, int) override
    {
        if (parent == exParent && isSuitableType (tree))
        {
            const int oldIndex = indexOfStateInObjects (tree);

            if (oldIndex >= 0)
            {
                ObjectType* o;

                {
                    const ScopedLockType sl (arrayLock);
                    o = objects.removeAndReturn (oldIndex);
                }

                listeners.call (&Listener::objectRemoved, o, this);
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

            listeners.call (&Listener::objectOrderChanged, this);
        }
    }

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override {}
    void valueTreeParentChanged (ValueTree&) override {}
    void valueTreeRedirected (ValueTree&) override { jassertfalse; } // may need to add handling if this is hit

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ValueTreeObjectArray)
};
