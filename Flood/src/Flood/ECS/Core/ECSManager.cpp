#include "flpch.h"
#include "ECSManager.h"

namespace Flood
{
	namespace ECS
	{
		void ECSManager::Init()
		{
			m_EntityManager = std::make_unique<EntityManager>();
			m_ComponentManager = std::make_unique<ComponentManager>();
			m_SystemManager = std::make_unique<SystemManager>();
		}

		void ECSManager::DestroyEntity(Entity _entity)
		{
			m_EntityManager->DestroyEntity(_entity);
			m_ComponentManager->EntityDestroyed(_entity);
			m_SystemManager->EntityDestroyed(_entity);
		}

		std::set<Flood::ECS::Entity>& ECSManager::GetEntities()
		{
			return m_EntityManager->m_Entities;
		}

		Entity ECSManager::CreateEntity()
		{
			return m_EntityManager->CreateEntity();
		}
	}
}