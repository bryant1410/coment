#include "managers/EntityManager.h"

namespace coment
{
	EntityManager::EntityManager()
		: _count(0), _nextEntityId(0), _totalCreated(0), _totalRemoved(0)
	{

	}

	EntityManager::~EntityManager()
	{

	}

	Entity EntityManager::createEntity()
	{
		Entity nextEntity;

		// If an old entity is available for reuse
		if (_dead.getSize() != 0)
		{
			// Get last dead entity and bring it back to life
			nextEntity = _dead.getLast();

			_alive.add(nextEntity);
			_dead.removeLast();
		}
		else
		{
			EntityId newId = _nextEntityId++;

			// Add entity to master list of entities if this is a completely new entity
			_entities.set(newId, EntityInfo(newId));

			// Set entity ID to return
			nextEntity._id = newId;
		}
		
		// Create new entity
		_alive.add(nextEntity);

		// Update counters
		_totalCreated++;
		_count++;

		// Return entity id
		return nextEntity;
	}

	void EntityManager::removeEntity(EntityInfo& e)
	{
		unsigned int id = _alive.getIndex(e);

		// If entity is alive
		if (id > 0)
		{
			// Murder entity
			_alive.remove(id);
			_dead.add(e);
			
			// Reset the component mask
			e._componentMask.clear();

			// Update counters
			_count--;
			_totalRemoved++;
		}
	}

	// Get the entity info for an entity
	EntityInfo& EntityManager::getEntityInfo(Entity e) 
	{
		return _entities.get(e.getId());
	}
}
