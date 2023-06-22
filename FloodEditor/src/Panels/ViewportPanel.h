#ifndef VIEWPORTPANEL__H
#define VIEWPORTPANEL__H

#include "Flood/Scene/Scene.h"
#include "Flood/Core/Core.h"
#include "Flood/ECS/Core/Types.h"

#include "SceneHierarchyPanel.h"

#include <glm/gtc/type_ptr.hpp>

namespace Flood
{
	class ViewportPanel
	{
	public:
		ViewportPanel() = default;
		ViewportPanel(const Ref<Scene>& scene, SceneHierarchyPanel* hierarchy, class EditorLayer* editor);
		virtual ~ViewportPanel() = default;

		void SetContext(const Ref<Scene>& scene, SceneHierarchyPanel* hierarchy, class EditorLayer* editor);

		void OnImGuiRender();

		glm::vec2 GetSize() const;
		glm::vec2 GetBounds(uint32_t) const;
		bool IsFocused() const;
		bool IsHovered() const;

	private:
		Ref<Scene> m_Context;
		class EditorLayer* m_Editor;
		SceneHierarchyPanel* m_HierarchyPanel;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
	};
}

#endif //!VIEWPORTPANEL__H
