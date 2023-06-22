#ifndef Component__H
#define Component__H

#include "Types.h"
#include <array>
#include <cassert>
#include <unordered_map>

#include "Flood/EventSystem/Events/FunctionPointer.h"
#include "Flood/EventSystem/Events/MethodPointer.h"

namespace Flood
{
	namespace ECS
	{
		// visiblement cet héritage pourrait être évité en utilisant des event lors de la destruction
		// de l'entité, tous les components inscris a cet event seraient alors automatiquement détruits
		class IComponentArray
		{
		public:
			virtual ~IComponentArray() = default;
			virtual void EntityDestroyed(Entity entity) = 0;
		};
	}
}

namespace Flood
{
	namespace ECS
	{
		template<typename T>
		class ComponentArray : public IComponentArray
		{
		private:
			template<class C>
			using Method = void(C::*)(Entity, T&);

			using Function = void(*)(Entity, T&);
			using FunctionType = Events::ActionPointer<void, Entity, T&>;
		private:
			// The packed array of components (of generic type T),
			// set to a specified maximum amount, matching the maximum number
			// of entities allowed to exist simultaneously, so that each entity
			// has a unique spot.
			std::array<T, MAX_ENTITIES> m_ComponentArray{};

			// unordored_map cool parce que facile a mettre en place mais pas continu dans la mémoire, le changer pour un array pourrait être bien
			// Map from an entity ID to an array index.
			std::unordered_map<Entity, size_t> m_EntityToIndexMap{};

			// Map from an array index to an entity ID.
			std::unordered_map<size_t, Entity> m_IndexToEntityMap{};

			// Total size of valid entries in the array.
			size_t m_Size{};

			FunctionType* AddComponentCallback = nullptr;

		public:
			void InsertData(Entity entity, T component)
			{
				assert(m_EntityToIndexMap.find(entity) == m_EntityToIndexMap.end() && "Component added to same entity more than once.");

				// Put new entry at end and update the maps
				size_t newIndex = m_Size;
				m_EntityToIndexMap[entity] = newIndex;
				m_IndexToEntityMap[newIndex] = entity;
				m_ComponentArray[newIndex] = component;
				++m_Size;

				if (AddComponentCallback != nullptr)
				{
					(*AddComponentCallback)(entity, m_ComponentArray[newIndex]);
				}
			}

			template<class C>
			void SetAddComponentCallback(C& object, Method<C> method)
			{
				AddComponentCallback = new Events::MethodPointer<C, void, Entity, T&>(object, method);
			}

			void SetAddComponentCallback(Function func)
			{
				AddComponentCallback = new Events::FunctionPointer<void, Entity, T&>(func);
			}

			T RemoveData(Entity entity)
			{
				assert(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end() && "Removing non-existent component.");

				// Copy element at end into deleted element's place to maintain density
				size_t indexOfRemovedEntity = m_EntityToIndexMap[entity];
				size_t indexOfLastElement = m_Size - 1;
				T removedData = m_ComponentArray[indexOfRemovedEntity];
				m_ComponentArray[indexOfRemovedEntity] = m_ComponentArray[indexOfLastElement];

				// Update map to point to moved spot
				Entity entityOfLastElement = m_IndexToEntityMap[indexOfLastElement];
				m_EntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
				m_IndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

				m_EntityToIndexMap.erase(entity);
				m_IndexToEntityMap.erase(indexOfLastElement);

				--m_Size;
				return removedData;
			}

			T& GetData(Entity entity)
			{
				assert(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end() && "Retrieving non-existent component.");

				// Return a reference to the entity's component
				return m_ComponentArray[m_EntityToIndexMap[entity]];
			}

			void EntityDestroyed(Entity entity) override
			{
				if (m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end())
				{
					// Remove the entity's component if it existed
					RemoveData(entity);
				}
			}
		};
	}
}
#endif
