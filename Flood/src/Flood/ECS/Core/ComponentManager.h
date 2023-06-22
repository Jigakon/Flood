#ifndef ComponentManager__H
#define ComponentManager__H

#include "ComponentArray.h"
#include "Types.h"
#include <unordered_map>
#include <memory>
#include <any>

namespace Flood
{
	namespace ECS
	{
		class ComponentManager
		{
		private:
			template<class C, typename T>
			using Method = void(C::*)(Entity, T&);

			template<typename T>
			using Function = void(*)(Entity, T&);

		private:
			// dictionnaire avec les ID de type de component reliés a leur "nom"
			std::unordered_map<const char*, ComponentType> m_ComponentTypes{};

			// dictionnaire avec les arrays de components reliés au "nom" du type de component
			std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_ComponentArrays{};

			// le prochain ID de component a assigner, il commence a 0
			ComponentType m_NextComponentType{};

		public:
			~ComponentManager();

			// enregistrera les différents components dans les dictionnaires et incrémentera le compteur de component
			template<typename T>
			void RegisterComponent()
			{
				const char* typeName = typeid(T).name();

				assert(m_ComponentTypes.find(typeName) == m_ComponentTypes.end() && "Registering component type more than once.");

				// Add this component type to the component type map
				m_ComponentTypes.insert({ typeName, m_NextComponentType });

				// Create a ComponentArray pointer and add it to the component arrays map
				m_ComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

				// Increment the value so that the next component registered will be different
				++m_NextComponentType;
			}

			// recupèrera l'ID du component par rapport a son type s'il est enregistré dans les dictionnaires
			// permettra de créer les signatures
			template<typename T>
			ComponentType GetComponentType()
			{
				const char* typeName = typeid(T).name();

				// Ajouté pour ne pas avoir a enregistrer les composants a la main
				if (m_ComponentTypes.find(typeName) == m_ComponentTypes.end())
					RegisterComponent<T>();
		
				assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "Component not registered before use.");

				return m_ComponentTypes[typeName];
			}

			// ajoute un component a une entité
			template<typename T>
			T& AddComponent(Entity entity, T component)
			{
				// Ajouté pour ne pas avoir a enregistrer les composants a la main
				const char* typeName = typeid(T).name();
				if (m_ComponentTypes.find(typeName) == m_ComponentTypes.end())
					RegisterComponent<T>();

				GetComponentArray<T>()->InsertData(entity, component);
				return GetComponent<T>(entity);
			}

			template<typename T>
			void SetAddComponentCallback(Function<T> func)
			{
				const char* typeName = typeid(T).name();
				if (m_ComponentTypes.find(typeName) == m_ComponentTypes.end())
					RegisterComponent<T>();

				GetComponentArray<T>()->SetAddComponentCallback(func);
			}

			template<class C, typename T>
			void SetAddComponentCallback(C& object, Method<C, T> method)
			{
				const char* typeName = typeid(T).name();
				if (m_ComponentTypes.find(typeName) == m_ComponentTypes.end())
					RegisterComponent<T>();

				GetComponentArray<T>()->SetAddComponentCallback(object, method);
			}

			template<typename T>
			T RemoveComponent(Entity entity)
			{
				// Remove a component from the array for an entity
				return GetComponentArray<T>()->RemoveData(entity);
			}

			template<typename T>
			T& GetComponent(Entity entity)
			{
				// Get a reference to a component from the array for an entity
				return GetComponentArray<T>()->GetData(entity);
			}

			void EntityDestroyed(Entity entity);

		private:
			// Convenience function to get the statically casted pointer to the ComponentArray of type T.
			template<typename T>
			std::shared_ptr<ComponentArray<T>> GetComponentArray()
			{
				const char* typeName = typeid(T).name();

				// peut être enregistrer ici les components, ça pourrait fluidifier la manipulation des components pour les entités
				assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "Component not registered before use.");

				return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
			}
		};
	}
}

#endif
