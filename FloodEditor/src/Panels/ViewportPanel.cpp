#include "ViewportPanel.h"

#include "Flood/Core/Application.h"
#include "Flood/ECS/Components/Transform.h"
#include "Flood/ECS/Components/CameraComponent.h"
#include "Flood/Core/Input.h"
#include "Flood/Maths/Maths.h"

#include "imgui/imgui.h"
#include "ImGuizmo.h"

#include "../EditorLayer.h"
#include "Flood/Renderer/EditorCamera.h"

namespace Flood
{
	extern const std::filesystem::path g_AssetPath = "assets";

	ViewportPanel::ViewportPanel(const Ref<Scene>& scene, SceneHierarchyPanel* hierarchy, EditorLayer* editor)
	{
		SetContext(scene, hierarchy, editor);
	}

	void ViewportPanel::SetContext(const Ref<Scene>& scene, SceneHierarchyPanel* hierarchy, EditorLayer* editor)
	{
		m_Context = scene;
		m_HierarchyPanel = hierarchy;
		m_Editor = editor;
	}

	glm::vec2 ViewportPanel::GetBounds(uint32_t index) const
	{
		return m_ViewportBounds[index];
	}

	void ViewportPanel::OnImGuiRender()
	{
		uint32_t textureID = m_Editor->GetFrameBuffer()->GetColorAttachmentRendererID(0);
		int GizmoType = m_Editor->GetGizmoType();
		auto camera = m_Editor->GetEditorCamera();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
		ImGui::Begin("ViewPort");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				auto filePath = std::filesystem::path(g_AssetPath) / path;
				if (filePath.extension().compare(".flood") == 0)
				{
					m_Editor->OpenScene(filePath);
					m_HierarchyPanel->SetSelectedEntity(ECS::Null);
				}
			}

			ImGui::EndDragDropTarget();
		}

		auto minRegion = ImGui::GetWindowContentRegionMin();
		auto maxRegion = ImGui::GetWindowContentRegionMax();
		auto offset = ImGui::GetWindowPos(); // include tab bar
		m_ViewportBounds[0] = { minRegion.x + offset.x, minRegion.y + offset.y };
		m_ViewportBounds[1] = { maxRegion.x + offset.x, maxRegion.y + offset.y };

		if (m_Editor->GetSceneState() != EditorLayer::SceneState::Play)
		{
			// Gizmos
			auto& manager = m_Context->Manager();
			ECS::Entity selectedEntity = m_HierarchyPanel->GetSelection();
			if (selectedEntity != ECS::Null && manager.HasComponent<TransformComponent>(selectedEntity) && GizmoType != -1)
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();

				float windowWidth = (float)ImGui::GetWindowWidth();
				float windowHeight = (float)ImGui::GetWindowHeight();
				ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

				const glm::mat4& cameraProjection = camera.GetProjection();
				glm::mat4 cameraView = camera.GetViewMatrix();

				auto& tc = manager.GetComponent<TransformComponent>(selectedEntity);
				glm::mat4 transform = tc.GetTransform();

				bool snap = Input::IsKeyPressed(Key::LeftControl);
				float snapValue = 0.5f; // Snap to 0.5m for translation/scale
				// Snap to 45 degrees for rotation
				if (GizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 45.0f;

				float snapValues[3] = { snapValue, snapValue, snapValue };

				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)m_Editor->GetGizmoType(), ImGuizmo::LOCAL, glm::value_ptr(transform),
					nullptr, snap ? snapValues : nullptr);

				if (ImGuizmo::IsUsing())
				{
					glm::vec3 translation, rotation, scale;
					Maths::DecomposeTransform(transform, translation, rotation, scale);

					glm::vec3 deltaRotation = rotation - tc.Rotation;
					tc.Translation = translation;
					tc.Rotation += deltaRotation;
					tc.Scale = scale;
				}
			}
		}

		ImGui::End(); // Game End
		ImGui::PopStyleVar();
	}

	glm::vec2 ViewportPanel::GetSize() const
	{
		return m_ViewportSize;
	}

	bool ViewportPanel::IsFocused() const
	{
		return m_ViewportFocused;
	}

	bool ViewportPanel::IsHovered() const
	{
		return m_ViewportHovered;
	}

}
