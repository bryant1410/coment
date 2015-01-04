#include "coment/managers/ComponentManager.h"

namespace coment
{
    /** Add a component to an entity */
    template <typename T, typename... Args>
    T* ComponentManager::addComponent(Entity e, Args... args)
    {
        // Check if entity is valid and alive
        checkEntityAlive(e);

        // Get type ID for type
        unsigned int typeId = getComponentTypeId<T>();

        // Get component array for type
        std::vector<T>& componentArray = *getComponentArray<T>();

        // Initialise component
        componentArray[e.getId()] = T(args...);

        // Update component mask for entity
        // Get component bitmask
        dynamic_bitset<>& componentBitmask = mEntityInfo[e.getId()].componentBitmask;

        // Update component bitmask
        dynamic_bitset<> oldBitmask = componentBitmask;

        if (componentBitmask.size() <= typeId)
            componentBitmask.resize(typeId + 1);

        componentBitmask.set(typeId, true);

        // Update mEntitiesByComponentBitmask arrays
        updateEntityMaps(e, oldBitmask, componentBitmask);

        // Return a pointer to the new component
        return &componentArray[e.getId()];
    }

    /** Get a component from an entity */
    template <typename T>
    T* ComponentManager::getComponent(Entity e)
    {
        // Check if entity is valid and alive
        checkEntityAlive(e);

        // Get component array for type
        std::vector<T>& componentArray = *getComponentArray<T>();

        // Get component
        return &componentArray[e.getId()];
    }

    /** Remove a component from an entity */
    template <typename T>
    void ComponentManager::removeComponent(Entity e)
    {
        // Check if entity is valid and alive
        checkEntityAlive(e);

        // Get type id for type
        unsigned int typeId = getComponentTypeId<T>();

        // Update component mask for entity
        // Get component bitmask
        dynamic_bitset<>& componentBitmask = mEntityInfo[e.getId()].componentBitmask;

        // Update component bitmask
        dynamic_bitset<> oldBitmask = componentBitmask;
        componentBitmask.set(typeId, false);

        // Update mEntitiesByComponentBitmask arrays
        updateEntityMaps(e, oldBitmask, componentBitmask);
    }

    /* Get an entity -> component map for a specific set of components */
    template <typename... ComponentTypes>
    EntityMap<ComponentTypes...> ComponentManager::getEntityMap()
    {
        dynamic_bitset<> componentTypes = getComponentTypes<ComponentTypes...>();

        // Resize componentTypes to correct length
        componentTypes.resize(mTypeEnumerator.getCurrentMax() + 1);

        // Check for component types in map
        std::shared_ptr<std::vector<Entity>> entityArray;

        auto it = mEntitiesByComponentBitmask.find(componentTypes);
        if (it == mEntitiesByComponentBitmask.end())
        {
            // Create entity array
            entityArray = std::make_shared<std::vector<Entity>>();
            mEntitiesByComponentBitmask[componentTypes] = entityArray;

            // Populate it for the first time
            for (unsigned int i = 0; i < mEntityInfo.size(); ++i)
            {
                if (mEntityInfo[i].alive)
                {
                    dynamic_bitset<>& bitmask = mEntityInfo[i].componentBitmask;

                    // Check if this bitmask has all required components
                    if (componentTypes.is_subset_of(bitmask))
                    {
                        entityArray->push_back(Entity(i, mEntityInfo[i].uniqueId));
                    }
                }
            }
        }
        else
        {
            entityArray = it->second;
        }

        return EntityMap<ComponentTypes...>(componentTypes, entityArray);
    }

    /** Get a type ID for a specific component type */
    template <typename T>
    unsigned int ComponentManager::getComponentTypeId()
    {
        // Get type id
        unsigned int id = mTypeEnumerator.getTypeId<T>();

        return id;
    }

    /** Get an array for a specific component type */
    template <typename T>
    std::vector<T>* ComponentManager::getComponentArray()
    {
        // Get type id
        unsigned int id = getComponentTypeId<T>();

        // Create array if not existent
        // Check if arrays array long enough
        if (mComponentArrays.size() <= id)
        {
            // Resize arrays array to accomodate this type
            mComponentArrays.resize(id + 1);
        }

        // Check if this type has an array
        if (mComponentArrays[id] == nullptr)
        {
            // Create this type's array
            mComponentArrays[id] = std::make_shared<std::vector<T>>();
        }

        // Return component array
        std::vector<T>* arr = static_cast<std::vector<T>*>(mComponentArrays[id].get());

        // Resize to number of entities if smaller
        if (arr->size() < mEntityInfo.size())
            arr->resize(mEntityInfo.size());

        return arr;
    }

    /** Decompose a variadic type pack into a list of integer ids */
    template <typename T1>
    dynamic_bitset<> ComponentManager::getComponentTypes()
    {
        // Create new (0) bitset
        dynamic_bitset<> componentTypes;

        // Get this type's id
        unsigned int id = mTypeEnumerator.getTypeId<T1>();

        // Ensure bitset is long enough for type
        if (componentTypes.size() <= id)
            componentTypes.resize(id + 1);

        // Set type's bit to true
        componentTypes.set(id, true);

        return componentTypes;
    }

    /** Decompose a variadic type pack into a list of integer ids */
    template <typename T1, typename T2, typename... ComponentTypes>
    dynamic_bitset<> ComponentManager::getComponentTypes()
    {
        // Create new (0) bitset
        dynamic_bitset<> componentTypes;

        // resize a to be at least as long as b, and then a |= b
        auto combine = [](dynamic_bitset<>& a, dynamic_bitset<>& b)
        {
            if (a.size() < b.size())
                a.resize(b.size());

            a |= b;
        };

        // Get bit for T1 and add it to componentTypes bitmask
        combine(componentTypes, getComponentTypes<T1>());

        // Get bits for the rest and add them to componentTypes bitmask
        combine(componentTypes, getComponentTypes<T2, ComponentTypes...>());

        return componentTypes;
    }

    /** Check if an entity is valid */
    inline bool ComponentManager::isEntityAlive(Entity e) const
    {
        // Check the entity is initialised
        if (!e.isInitialised())
            return false;

        // Check the entity hasn't been recycled
        if (e.getUniqueId() != mEntityInfo[e.getId()].uniqueId)
            return false;

        // This entity definitely refers to mEntityInfo[e.mId],
        // so check that the entity is alive
        return mEntityInfo[e.getId()].alive;
    }

    /** Check if an entity is valid and throw an exception otherwise */
    inline void ComponentManager::checkEntityAlive(Entity e) const
    {
        if (!isEntityAlive(e))
        {
            throw std::invalid_argument("Passed entity is uninitialised or has been destroyed");
        }
    }
}