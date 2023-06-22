#ifndef SCENEHIRARCHYPANEL__H
#define SCENEHIRARCHYPANEL__H

#include "Flood/Scene/Scene.h"
#include "Flood/Core/Core.h"
#include "Flood/ECS/Core/Types.h"

namespace Flood
{
	class SceneHierarchyPanel
	{
	public: 
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

		const ECS::Entity GetSelection() const { return m_SelectionContext; };

		void SetSelectedEntity(ECS::Entity entity);

	private:
		void DrawEntityNode(ECS::Entity entity);

	private:
		Ref<Scene> m_Context;
		ECS::Entity m_SelectionContext = ECS::Null;
	};
}

#endif // !SCENEHIRARCHYPANEL__H
