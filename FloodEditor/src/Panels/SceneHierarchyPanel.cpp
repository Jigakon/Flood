#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "Flood/ECS/Components/Components.h"
#include "Flood/ECS/Components/CameraComponent.h"

namespace Flood
{

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Hierarchy");
		auto& manager = m_Context->Manager();
		auto entities = manager.GetEntities();
		for (auto entity : entities)
		{
			DrawEntityNode(entity);
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = ECS::Null;

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				auto entity = manager.CreateEntity();
				m_Context->m_ECSManager.AddComponent<TagComponent>(entity, {"empty Entity"});
			}

			if (ImGui::MenuItem("Create Empty Scene Entity"))
			{
				auto entity = manager.CreateEntity();
				m_Context->m_ECSManager.AddComponent<TagComponent>(entity, { "Entity" });
				m_Context->m_ECSManager.AddComponent<TransformComponent>(entity, { });
			}

			if (ImGui::MenuItem("Create New Quad"))
			{
				auto entity = manager.CreateEntity();
				m_Context->m_ECSManager.AddComponent<TagComponent>(entity, { "Quad" });
				m_Context->m_ECSManager.AddComponent<TransformComponent>(entity, { });
				m_Context->m_ECSManager.AddComponent<SpriteRendererComponent>(entity, { });
			}
			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(ECS::Entity entity)
	{
		auto& tc = m_Context->Manager().GetComponent<TagComponent>(entity);
		
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, tc.Name.c_str());
		
		if (ImGui::IsItemClicked())
			m_SelectionContext = entity;
	
		bool destroyEntity = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				destroyEntity = true;
			}

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		if (destroyEntity)
		{ 		
			if (m_SelectionContext == entity)
				m_SelectionContext = ECS::Null;
			
			m_Context->m_ECSManager.DestroyEntity(entity);
		}
	}

	void SceneHierarchyPanel::SetSelectedEntity(ECS::Entity entity)
	{
		m_SelectionContext = entity;
	}
}
