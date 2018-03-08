/*
  ==============================================================================

    hmnz_HomogeneousValueTreeObjectArray.h
    Created: 8 March 2018 12:07:48pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObjectArray.h"

/**
    Generic homogeneous list of ValueTreeObject-backed objects. Originally based
    on code by David Rowland (drowaudio).
*/
template<typename ObjectType, typename CriticalSectionType = DummyCriticalSection>
class HomogeneousValueTreeObjectArray   : public ValueTreeObjectArray<ObjectType, CriticalSectionType>
{
public:
    struct Listener
    {
        virtual ~Listener() = default;
        virtual void objectAdded (ObjectType*, int, HomogeneousValueTreeObjectArray<ObjectType, CriticalSectionType>*) {}
        virtual void objectRemoved (ObjectType*, int, HomogeneousValueTreeObjectArray<ObjectType, CriticalSectionType>*) {}
        virtual void objectOrderChanged (ObjectType*, int, int, HomogeneousValueTreeObjectArray<ObjectType, CriticalSectionType>*) {}
    };

    using ValueTreeObjectArray<ObjectType, CriticalSectionType>::ValueTreeObjectArray;

    void addListener (Listener* listener) noexcept
    {
        listeners.add (listener);
    }

    void removeListener (Listener* listener) noexcept
    {
        listeners.remove (listener);
    }

    using ScopedLockType = typename CriticalSectionType::ScopedLockType;

private:
    ListenerList<Listener> listeners;

    void valueTreeChildAdded (ValueTree&, ValueTree& tree) override
    {
        if (this->isChildTree (tree))
        {
            const int index = this->parent.indexOf (tree);
            (void) index;
            jassert (index >= 0);

            if (ObjectType* newObject = this->createNewObject (tree, this->undoManager))
            {
                {
                    const ScopedLockType sl (this->arrayLock);
                    this->objects.insert (index, newObject);
                }

                listeners.call (&Listener::objectAdded, newObject, index, this);
            }
            else
                jassertfalse;
        }
    }

    void valueTreeChildRemoved (ValueTree& exParent, ValueTree& tree, int indexRemovedFrom) override
    {
        if (this->parent == exParent)
        {
            jassert (this->isSuitableType (tree));
            ObjectType* o;

            {
                const ScopedLockType sl (this->arrayLock);
                o = this->objects.removeAndReturn (indexRemovedFrom);
            }

            listeners.call (&Listener::objectRemoved, o, indexRemovedFrom, this);
            this->deleteObject (o);
        }
    }

    void valueTreeChildOrderChanged (ValueTree& tree, int oldIndex, int newIndex) override
    {
        if (tree == this->parent)
        {
            {
                const ScopedLockType sl (this->arrayLock);
                this->objects.move (oldIndex, newIndex);
            }

            listeners.call (&Listener::objectOrderChanged, this->objects[newIndex], oldIndex, newIndex, this);
        }
    }

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override {}
    void valueTreeParentChanged (ValueTree&) override {}
    void valueTreeRedirected (ValueTree&) override { jassertfalse; } // may need to add handling if this is hit

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HomogeneousValueTreeObjectArray)
};

template<class ObjectType, class CriticalSectionType = DummyCriticalSection>
class GenericHomogeneousValueTreeObjectArray    : public HomogeneousValueTreeObjectArray<ObjectType, CriticalSectionType>
{
public:
    GenericHomogeneousValueTreeObjectArray (const ValueTree& parentTree, UndoManager* um)
        : HomogeneousValueTreeObjectArray<ObjectType, CriticalSectionType> (parentTree, um)
    {
        this->addObjects();
    }

private:
    ObjectType* createNewObject (const ValueTree& v, UndoManager* um) override
    {
        return new ObjectType (v, um);
    }
};
