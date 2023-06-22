#ifndef SYSTEM__H
#define SYSTEM__H

#include "Types.h"
#include <set>

namespace Flood
{
	namespace ECS
	{
		class ECSManager;
		class System
		{
		public:
			std::set<Entity> m_Entities;
			ECSManager* m_Manager;

			virtual void OnCreate() {}

			virtual void InitEntity(Entity _entity) {}

			virtual void OnUpdate(float dt) {}
			
			virtual void OnEditorUpdate(float dt) {}

			virtual void OnDestroy() {}
		};
	}
}
#endif
