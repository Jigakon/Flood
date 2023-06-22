#ifndef SCENEINSPECTORPANEL__H
#define SCENEINSPECTORPANEL__H

#include "Flood/Scene/Scene.h"
#include "Flood/Core/Core.h"
#include "Flood/ECS/Core/Types.h"

#include "SceneHierarchyPanel.h"

#include "Flood/Core/Log.h"

#include "Flood/ECS/Components/Transform.h"
#include "Flood/ECS/Components/CameraComponent.h"

namespace Flood
{
	class SceneInspectorPanel
	{
	public:
		SceneInspectorPanel() = default;
		SceneInspectorPanel(const Ref<Scene>& scene, SceneHierarchyPanel* hierarchy);
		virtual ~SceneInspectorPanel() = default;

		void SetContext(const Ref<Scene>& scene, SceneHierarchyPanel* hierarchy);

		void OnImGuiRender();

	private:
		void DrawComponents(ECS::Entity entity);

	public:
		template<typename Comp>
		void OnAddComponent(ECS::Entity entity, Comp& component)
		{
		}

		template<>
		void OnAddComponent<TransformComponent>(ECS::Entity entity, TransformComponent& component)
		{
		}

		template<>
		void OnAddComponent<CameraComponent>(ECS::Entity entity, CameraComponent& component)
		{
			auto [width, height] = m_Context->GetViewportSize();
			if (width > 0 && height > 0)
				component.Camera.SetViewportSize(width, height);
		}

	private:
		Ref<Scene> m_Context;
		SceneHierarchyPanel* m_HierarchyPanel;
	};
}

#endif // !SCENEINSPECTORPANEL__H
