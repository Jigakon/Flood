#include "flpch.h"
#include "SystemManager.h"

namespace Flood
{
	namespace ECS
	{
		SystemManager::~SystemManager()
		{
			m_Systems.clear();
			m_Signatures.clear();
		}

		void SystemManager::EntityDestroyed(Entity _entity)
		{
			for (auto& pair : m_Systems)
			{
				auto const& system = pair.second;

				system->m_Entities.erase(_entity);
			}
		}

		void SystemManager::EntitySignatureChanged(Entity _entity, Signature _entitySignature)
		{
			for (auto& pair : m_Systems)
			{
				auto const& type = pair.first;
				auto const& system = pair.second;
				auto const& systemSignature = m_Signatures[type];

				if ((_entitySignature & systemSignature) == systemSignature)
				{
					system->m_Entities.insert(_entity);
					system->InitEntity(_entity);
				}
				else
					system->m_Entities.erase(_entity);
			}
		}
	}
}
