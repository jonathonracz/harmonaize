/*
  ==============================================================================

    hmnz_NonHomogeneousValueTreeObjectArray.h
    Created: 8 Mar 2018 1:51:34pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObjectArray.h"

/**
    Generic list of ValueTree-backed objects based on ValueTreeObject.
    Originally based on code by David Rowland (drowaudio).
*/
template<class ObjectType, class CriticalSectionType = DummyCriticalSection>
class NonHomogeneousValueTreeObjectArray    : public ValueTreeObjectArray<ObjectType, CriticalSectionType>
{
public:
    struct Listener
    {
        virtual ~Listener() = default;
        virtual void objectAdded (ObjectType*, NonHomogeneousValueTreeObjectArray<ObjectType, CriticalSectionType>*) {}
        virtual void objectRemoved (ObjectType*, NonHomogeneousValueTreeObjectArray<ObjectType, CriticalSectionType>*) {}
        virtual void objectOrderChanged (NonHomogeneousValueTreeObjectArray<ObjectType, CriticalSectionType>*) {}
    };

    using ValueTreeObjectArray<ObjectType, CriticalSectionType>::ValueTreeObjectArray;

    void addListener (Listener* listener)
    {
        listeners.add (listener);
    }

    void removeListener (Listener* listener)
    {
        listeners.remove (listener);
    }

    int compareElements (ObjectType* first, ObjectType* second) const
    {
        int index1 = this->parent.indexOf (first->getState());
        int index2 = this->parent.indexOf (second->getState());
        return index1 - index2;
    }

    using ScopedLockType = typename CriticalSectionType::ScopedLockType;

private:
    ListenerList<Listener> listeners;

    void sortArray()
    {
        this->objects.sort (*this);
    }

    void valueTreeChildAdded (ValueTree& treeChildAddedTo, ValueTree& tree) override
    {
        if (treeChildAddedTo == this->parent)
        {
            ObjectType* addedObject = nullptr;
            int indexOfAlreadyExistingObject = this->indexOfStateInObjects (tree);
            jassert (this->isChildTree (tree));

            if (indexOfAlreadyExistingObject < 0)
            {
                const int index = this->parent.indexOf (tree);
                (void) index;
                jassert (index >= 0);

                addedObject = this->createNewObject (tree, this->undoManager);

                {
                    const ScopedLockType sl (this->arrayLock);

                    if (index == this->parent.getNumChildren() - 1)
                        this->objects.add (addedObject);
                    else
                        this->objects.addSorted (*this, addedObject);
                }
            }
            else
            {
                addedObject = this->objects[indexOfAlreadyExistingObject];
            }

            jassert (addedObject);
            listeners.call (&Listener::objectAdded, addedObject, this);
        }
    }

    void valueTreeChildRemoved (ValueTree& exParent, ValueTree& tree, int) override
    {
        if (this->parent == exParent && this->isSuitableType (tree))
        {
            const int oldIndex = this->indexOfStateInObjects (tree);

            if (oldIndex >= 0)
            {
                ObjectType* o;

                {
                    const ScopedLockType sl (this->arrayLock);
                    o = this->objects.removeAndReturn (oldIndex);
                }

                listeners.call (&Listener::objectRemoved, o, this);
                this->deleteObject (o);
            }
        }
    }

    void valueTreeChildOrderChanged (ValueTree& tree, int, int) override
    {
        if (tree == this->parent)
        {
            {
                const ScopedLockType sl (this->arrayLock);
                sortArray();
            }

            listeners.call (&Listener::objectOrderChanged, this);
        }
    }

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override {}
    void valueTreeParentChanged (ValueTree&) override {}
    void valueTreeRedirected (ValueTree&) override { jassertfalse; } // may need to add handling if this is hit

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NonHomogeneousValueTreeObjectArray)
};

template<class ObjectType, class CriticalSectionType = DummyCriticalSection>
class GenericNonHomogeneousValueTreeObjectArray    : public NonHomogeneousValueTreeObjectArray<ObjectType, CriticalSectionType>
{
public:
    GenericNonHomogeneousValueTreeObjectArray (const ValueTree& parentTree, UndoManager* um)
        : NonHomogeneousValueTreeObjectArray<ObjectType, CriticalSectionType> (parentTree, um)
    {
        this->addObjects();
    }

private:
    ObjectType* createNewObject (const ValueTree& v, UndoManager* um) override
    {
        return new ObjectType (v, um);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenericNonHomogeneousValueTreeObjectArray)
};
