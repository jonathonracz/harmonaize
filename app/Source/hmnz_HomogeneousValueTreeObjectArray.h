/*
  ==============================================================================

    hmnz_HomogeneousValueTreeObjectArray.h
    Created: 8 March 2018 12:07:48pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "hmnz_ArrayIterator.h"

/**
    Generic homogeneous list of ValueTreeObject-backed objects. Originally based
    on code by David Rowland (drowaudio).
*/
template<typename ObjectType, typename CriticalSectionType = DummyCriticalSection>
class HomogeneousValueTreeObjectArray   : public ValueTree::Listener
{
public:
    struct Listener
    {
        virtual ~Listener() = default;
        virtual void objectAdded (ObjectType*, int) {}
        virtual void objectRemoved (ObjectType*, int) {}
        virtual void objectOrderChanged (ObjectType*, int, int) {}
    };

    HomogeneousValueTreeObjectArray (const ValueTree& parentTree, UndoManager* um)
        : parent (parentTree), undoManager (um)
    {
        parent.addListener (this);
    }

    virtual ~HomogeneousValueTreeObjectArray()
    {
        parent.removeListener (this);
        deleteAllObjects();
        jassert (objects.size() == 0); // must call freeObjects() in the subclass destructor!
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
        return v.getParent() == parent;
    }

    int indexOf (ObjectType* obj) const noexcept
    {
        const ScopedLockType sl (arrayLock);
        return objects.indexOf (obj);
    }

    int indexOf (const ValueTree& state) const noexcept
    {
        return parent.indexOf (state);
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

    using ScopedLockType = typename CriticalSectionType::ScopedLockType;

protected:
    // call in the sub-class when being created
    void rebuildObjects() noexcept
    {
        deleteAllObjects();
        jassert (objects.size() == 0);
        for (const auto& v : parent)
        {
            jassert (isSuitableType (v));
            if (ObjectType* newObject = createNewObject (v))
                objects.add (newObject);
        }
    }

    virtual bool isSuitableType (const ValueTree&) const = 0;
    virtual ObjectType* createNewObject (const ValueTree&) = 0;
    virtual void deleteObject (ObjectType* object) { delete object; }

private:
    Array<ObjectType*> objects;
    ValueTree parent;
    UndoManager* undoManager;
    CriticalSectionType arrayLock;
    ListenerList<Listener> listeners;

    void valueTreeChildAdded (ValueTree&, ValueTree& tree) override
    {
        if (isChildTree (tree))
        {
            const int index = parent.indexOf (tree);
            (void) index;
            jassert (index >= 0);

            if (ObjectType* newObject = createNewObject (tree))
            {
                {
                    const ScopedLockType sl (arrayLock);
                    objects.insert (index, newObject);
                }

                listeners.call (&Listener::objectAdded, newObject, index);
            }
            else
                jassertfalse;
        }
    }

    void valueTreeChildRemoved (ValueTree& exParent, ValueTree& tree, int indexRemovedFrom) override
    {
        if (parent == exParent)
        {
            jassert (isSuitableType (tree));
            ObjectType* o;

            {
                const ScopedLockType sl (arrayLock);
                o = objects.removeAndReturn (indexRemovedFrom);
            }

            listeners.call (&Listener::objectRemoved, o, indexRemovedFrom);
            deleteObject (o);
        }
    }

    void valueTreeChildOrderChanged (ValueTree& tree, int oldIndex, int newIndex) override
    {
        if (tree == parent)
        {
            {
                const ScopedLockType sl (arrayLock);
                objects.move (oldIndex, newIndex);
            }

            listeners.call (&Listener::objectOrderChanged, objects[newIndex], oldIndex, newIndex);
        }
    }

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override {}
    void valueTreeParentChanged (ValueTree&) override {}
    void valueTreeRedirected (ValueTree&) override { rebuildObjects(); } // may need to add handling if this is hit

    void deleteAllObjects() noexcept
    {
        const ScopedLockType sl (arrayLock);
        while (objects.size() > 0)
            deleteObject (objects.removeAndReturn (objects.size() - 1));
    }

    bool isChildTree (ValueTree& v) const noexcept
    {
        return isSuitableType (v) && v.getParent() == parent;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HomogeneousValueTreeObjectArray)
};
