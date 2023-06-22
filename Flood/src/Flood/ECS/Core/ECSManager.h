#ifndef ECSMANAGER__H
#define ECSMANAGER__H

#include <memory>
#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"

namespace Flood
{
	namespace ECS
	{
		class ECSManager
		{
		private:
			template<class C, typename Comp>
			using Method = void(C::*)(Entity, Comp&);

			template<typename Comp>
			using Function = void(*)(Entity, Comp&);

		private:
			std::unique_ptr<EntityManager> m_EntityManager;
			std::unique_ptr<ComponentManager> m_ComponentManager;
			std::unique_ptr<SystemManager> m_SystemManager;

		public:
			void Init();

#pragma region Entity

			Entity CreateEntity();
			void DestroyEntity(Entity _entity);
			std::set<Entity>& GetEntities();

#pragma endregion

#pragma region Component

			template<typename Comp>
			void RegisterComponent()
			{
				m_ComponentManager->RegisterComponent<Comp>();
			}

			template<typename Comp>
			Comp& AddComponent(Entity _entity, Comp _component)
			{
				Comp& comp = m_ComponentManager->AddComponent<Comp>(_entity, _component);
				Signature signature = m_EntityManager->GetSignature(_entity);
				signature.set(m_ComponentManager->GetComponentType<Comp>(), true);
				m_EntityManager->SetSignature(_entity, signature);

				m_SystemManager->EntitySignatureChanged(_entity, signature);
				return comp;
			}

			template<typename Comp>
			void SetAddComponentCallback(Function<Comp> func)
			{
				m_ComponentManager->SetAddComponentCallback<Comp>(func);
			}

			template<class C, typename Comp>
			void SetAddComponentCallback(C& object, Method<C, Comp> method)
			{
				m_ComponentManager->SetAddComponentCallback<C, Comp>(object, method);
			}

			template<typename Comp>
			Comp RemoveComponent(Entity _entity)
			{
				Comp removedComponent = m_ComponentManager->RemoveComponent<Comp>(_entity);

				Signature signature = m_EntityManager->GetSignature(_entity);
				signature.set(m_ComponentManager->GetComponentType<Comp>(), false);

				m_EntityManager->SetSignature(_entity, signature);

				m_SystemManager->EntitySignatureChanged(_entity, signature);

				return removedComponent;
			}

			template<typename Comp>
			Comp& GetComponent(Entity _entity)
			{
				return m_ComponentManager->GetComponent<Comp>(_entity);
			}

			template<typename Comp>
			ComponentType GetComponentType()
			{
				return m_ComponentManager->GetComponentType<Comp>();
			}

			template<typename T>
			bool HasComponent(Entity _entity) const
			{
				return (m_EntityManager->GetSignature(_entity).to_ulong() & (1 << m_ComponentManager->GetComponentType<T>()));
			}

#pragma endregion

#pragma region Signature creation

		private:
			template<typename Comp>
			void CreateSignatureInternOneType(Signature& _signature)
			{
				_signature.set(m_ComponentManager->GetComponentType<Comp>());
			}

		public:
			// solution trouvee sur : https://stackoverflow.com/questions/21180346/variadic-template-unpacking-arguments-to-typename
			template<typename... Comps>
			Signature CreateSignature()
			{
				Signature sig;
				int _[] = { 0, (CreateSignatureInternOneType<Comps>(sig), 0)... };
				// dit au compilateur que ce n'est pas grave si _ n'est pas utilise
				(void)_;
				return sig;
			}

#pragma endregion

#pragma region Query

		public:
			template<typename... Comps>
			std::vector<Entity> Query()
			{
				Signature sig = CreateSignature<Comps...>();
				return m_EntityManager->GetEntitiesFromQuery(sig);
			}

#pragma endregion

#pragma region System

#pragma region RegisterSystem

			// enregistre un system sans signature
			template <typename Sys>
			std::shared_ptr<Sys> RegisterSystem()
			{
				std::shared_ptr<Sys> system = m_SystemManager->RegisterSystem<Sys>();
				system->m_Manager = this;
				return system;
			}

			// enregister un systeme en donnant une signature
			template <typename Sys>
			std::shared_ptr<Sys> RegisterSystem(Signature _signature)
			{
				std::shared_ptr<Sys> system = m_SystemManager->RegisterSystem<Sys>();
				m_SystemManager->SetSignature<Sys>(_signature);
				system->m_Manager = this;
				return system;
			}

			// enregistre un systeme en donnant les composants qui creeront la signature
			template <typename Sys, typename... Comps>
			std::shared_ptr<Sys> RegisterSystem()
			{
				std::shared_ptr<Sys> system = m_SystemManager->RegisterSystem<Sys>();

				Signature sig = CreateSignature<Comps...>();
				m_SystemManager->SetSignature<Sys>(sig);
				system->m_Manager = this;

				for (auto entity : m_EntityManager->m_Entities)
				{
					if ((GetEntitySignature(entity) & sig) == sig)
					{
						system->m_Entities.insert(entity);
						system->InitEntity(entity);
					}
				}

				return system;
			}

#pragma endregion

#pragma region SystemSignature

			// donne une signature a un systeme donne en template avec une signature en parametre
			template <typename Sys>
			void SetSystemSignature(Signature _signature)
			{
				m_SystemManager->SetSignature<Sys>(_signature);
			}

			// donne une signature a un systeme donne en template avec les composants en template qui creerons la signature
			template <typename Sys, typename... Comps>
			void SetSystemSignature()
			{
				Signature sig = CreateSignature<Comps...>();
				m_SystemManager->SetSignature<Sys>(sig);
			}

#pragma endregion

			void OnUpdateEach(float dt)
			{
				m_SystemManager->EachOnUpdate(dt);
			}

#pragma endregion

#pragma region Bricolage

			auto GetLinvingEntityCount() { return m_EntityManager->GetLivingEntityCount(); }
			auto GetEntitySignature(Entity _entity) { return m_EntityManager->GetSignature(_entity); }
			
#pragma endregion

		};
	}
}

#endif
