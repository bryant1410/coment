#ifndef __COMPONENTTYPEMANAGER_H__
#define __COMPONENTTYPEMANAGER_H__

#include <hash_map>

#include "../utils/ComponentUtils.h"

namespace coment
{
	typedef std::hash_map<size_t, ComponentType> ComponentTypeMap;
	
	class ComponentTypeManager
		: public Manager
	{
	public:
		template <typename T>
		ComponentType getComponentType();

	private:
		ComponentTypeMap _componentTypes;
	};

	template <typename T>
	ComponentType ComponentTypeManager::getComponentType()
	{
		ComponentTypeMap::iterator it = _componentTypes.find(typeid(T).hash_code());

		// If hash map does not contain this type
		if (it == _componentTypes.end())
		{
			// Assign it a ComponentType
			ComponentType componentType = ComponentUtils::getNextType();

			// Add it to hash map
			_componentTypes[typeid(T).hash_code()] = componentType;
			
			// Return it
			return componentType;
		}
		else
		{
			// Return component type
			return (*it).second;
		}
	}
}

#endif /* __COMPONENTTYPEMANAGER_H__ */
