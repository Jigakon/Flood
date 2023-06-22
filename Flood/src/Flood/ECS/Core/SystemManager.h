#ifndef SYSTEMMANAGER__H
#define SYSTEMMANAGER__H

#include "Types.h"
#include "System.h"

#include <cassert>
#include <unordered_map>
#include <memory>

namespace Flood
{
	namespace ECS
	{
		class SystemManager
		{
		private:
			// Map from system type string pointer to a signature
			std::unordered_map<const char*, Signature> m_Signatures{};

			// Map from system type string pointer to a system pointer
			std::unordered_map<const char*, std::shared_ptr<System>> m_Systems{};


		public:
			~SystemManager();

			template<typename T>
			std::shared_ptr<T> RegisterSystem()
			{
				const char* typeName = typeid(T).name();

				assert(m_Systems.find(typeName) == m_Systems.end() && "Registering System more than once.");

				std::shared_ptr<T> system = std::make_shared<T>();
				m_Systems.insert({ typeName, system });

				return system;
			}

			template<typename T>
			void SetSignature(Signature _signature)
			{
				const char* typeName = typeid(T).name();

				assert(m_Systems.find(typeName) != m_Systems.end() && "System used before registering.");

				m_Signatures.insert({ typeName, _signature });
			}

			template<typename T>
			std::shared_ptr<T> GetSystem()
			{
				const char* typeName = typeid(T).name();

				assert(m_Systems.find(typeName) != m_Systems.end() && "System used before registering.");

				return m_Systems[typeName].second;
			}

			void EachOnUpdate(float dt)
			{
				for (auto system : m_Systems)
					system.second->OnUpdate(dt);
			}

			template<typename T>
			Signature GetSystemSignature()
			{
				const char* typeName = typeid(T).name();

				assert(m_Systems.find(typeName) != m_Systems.end() && "System used before registering.");

				return m_Systems[typeName].first;
			}

			void EntityDestroyed(Entity _entity);

			void EntitySignatureChanged(Entity _entity, Signature _entitySignature);
		};
	}
}
#endif
