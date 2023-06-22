#include "flpch.h"
#include "ComponentManager.h"

namespace Flood
{
	namespace ECS
	{
		ComponentManager::~ComponentManager()
		{
			m_ComponentArrays.clear();
		}

		void ComponentManager::EntityDestroyed(Entity entity)
		{
			// Notify each component array that an entity has been destroyed
				// If it has a component for that entity, it will remove it
			for (auto const& pair : m_ComponentArrays)
			{
				auto const& component = pair.second;

				component->EntityDestroyed(entity);
			}
		}
	}
}