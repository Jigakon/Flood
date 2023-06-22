#ifndef EntityManager__H
#define EntityManager__H

#include "Types.h"
#include <queue>
#include <array>
#include <set>

namespace Flood
{
	namespace ECS
	{
		class EntityManager
		{
		private:
			std::queue<Entity> m_AvailableID;
			std::array<Signature, MAX_ENTITIES> m_Signatures{};
			uint32_t m_livingEntityCount = 0;
			std::set<Entity> m_Entities;

		public:
			// ctr
			EntityManager();

			Entity CreateEntity();

			void DestroyEntity(Entity entity);

			void SetSignature(Entity entity, Signature signature);

			Signature GetSignature(Entity entity);

			auto GetLivingEntityCount() { return m_livingEntityCount; }

			std::vector<Entity> GetEntitiesFromQuery(Signature _signature);

			friend class ECSManager;
		};
	}
}

#endif // !EntityManager__H