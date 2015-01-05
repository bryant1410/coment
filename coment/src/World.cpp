#include "coment/World.h"

#include "coment/managers/EntityManager.h"
#include "coment/managers/ComponentManager.h"

namespace coment
{
    /** Create default managers and initialise the world */
    COMENT_API World::World()
    {
        // Create default managers
        mEntityManager = addManager<EntityManager>(this);
        mComponentManager = addManager<ComponentManager>();
    }

    /** Update all systems and managers */
    void COMENT_API World::update()
    {
        // Manager Callback: preUpdate
        // Notify managers that an update is about to occur
        for (auto& mgr : *mManagerMap.getVector())
        {
            mgr->preUpdate();
        }

        // System Callback: preUpdate
        // Notify systems that an update is about to occur
        for (auto& system : *mSystemMap.getVector())
        {
            system->preUpdate();
        }

        // System Callback: onUpdate
        // Notify systems that an update is occurring
        for (auto& system : *mSystemMap.getVector())
        {
            system->onUpdate();
        }

        // Manager Callback: postUpdate
        // Notify managers that an update has just occurred
        for (auto& mgr : *mManagerMap.getVector())
        {
            mgr->postUpdate();
        }

        // System Callback: postUpdate
        // Notify systems that an update has just occurred
        for (auto& system : *mSystemMap.getVector())
        {
            system->postUpdate();
        }
    }

    /* Proxy API for EntityManager */

    /** Creates or recycles an entity */
    Entity COMENT_API World::createEntity()
    {
        return mEntityManager->createEntity();
    }

    /** Destroys an entity and recycles its ID */
    void COMENT_API World::destroyEntity(Entity& e)
    {
        mEntityManager->destroyEntity(e);
    }

    /** Get whether an entity is living */
    bool World::isLiving(Entity e) const
    {
        return mEntityManager->isLiving(e);
    }
}