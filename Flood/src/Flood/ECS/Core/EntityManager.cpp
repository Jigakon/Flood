#include "flpch.h"
#include "EntityManager.h"
#include <cassert>

namespace Flood
{
	namespace ECS
	{
		EntityManager::EntityManager()
		{
			// Initialize the queue with all possible entity IDs
			for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
			{
				m_AvailableID.push(entity);
			}
		}

		Entity EntityManager::CreateEntity()
		{
			assert(m_livingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

			// Take an ID from the front of the queue
			Entity id = m_AvailableID.front();
			m_AvailableID.pop();
			++m_livingEntityCount;

			m_Entities.insert(id);

			return id;
		}

		void EntityManager::DestroyEntity(Entity entity)
		{
			if (entity == Null) return;

			assert(entity < MAX_ENTITIES && "Entity out of range.");

			// remplace les bits de la data par des 0b0
			m_Signatures[entity].reset();

			// Put the destroyed ID at the back of the queue
			m_AvailableID.push(entity);
			--m_livingEntityCount;

			m_Entities.erase(entity);
		}

		void EntityManager::SetSignature(Entity entity, Signature signature)
		{
			if (entity == Null) return;

			assert((entity < MAX_ENTITIES) && "Entity out of range.");

			
			// Put this entity's signature into the array
			m_Signatures[entity] = signature;
		}

		Signature EntityManager::GetSignature(Entity entity)
		{
			if (entity == Null) return 0;

			assert(entity < MAX_ENTITIES && "Entity out of range.");

			// Get this entity's signature from the array
			return m_Signatures[entity];
		}

		std::vector<Entity> EntityManager::GetEntitiesFromQuery(Signature _signature)
		{
			std::vector<Entity> queryEntities;
			for (auto entity : m_Entities)
				if ((m_Signatures[entity] & _signature) == _signature)
					queryEntities.push_back(entity);

			return queryEntities;
		}
	}
}
