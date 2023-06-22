#include "SceneInspectorPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <filesystem>

#include "Flood/ECS/Components/Components.h"
#include "Flood/ECS/Components/CameraComponent.h"
#include "Flood/ECS/Components/PlayerController.h"

namespace Flood
{
	extern const std::filesystem::path g_AssetPath;

	static const std::vector<std::string> g_SupportedImageFormats{ ".png", ".jpg" };

	SceneInspectorPanel::SceneInspectorPanel(const Ref<Scene>& context, SceneHierarchyPanel* hierarchy)
	{
		SetContext(context, hierarchy);
	}

	void SceneInspectorPanel::SetContext(const Ref<Scene>& context, SceneHierarchyPanel* hierarchy)
	{
		m_Context = context;
		m_HierarchyPanel = hierarchy;

		auto& manager = m_Context->Manager();

		manager.SetAddComponentCallback<SceneInspectorPanel, TransformComponent>(*this, &SceneInspectorPanel::OnAddComponent);
		manager.SetAddComponentCallback<SceneInspectorPanel, CameraComponent>(*this, &SceneInspectorPanel::OnAddComponent);
	}

	void SceneInspectorPanel::OnImGuiRender()
	{
		ImGui::Begin("Inspector");

		auto selection = m_HierarchyPanel->GetSelection();
		if (selection != ECS::Null)
		{
			auto& manager = m_Context->Manager();
			DrawComponents(selection);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (!manager.HasComponent<TransformComponent>(selection))
				if (ImGui::MenuItem("Transform"))
				{
					manager.AddComponent<TransformComponent>(selection, {});
					ImGui::CloseCurrentPopup();
				}

				if (!manager.HasComponent<CameraComponent>(selection))
				if (ImGui::MenuItem("Camera"))
				{
					manager.AddComponent<CameraComponent>(selection, {});
					ImGui::CloseCurrentPopup();
				}

				if (!manager.HasComponent<SpriteRendererComponent>(selection))
				if (ImGui::MenuItem("Sprite Renderer"))
				{
					manager.AddComponent<SpriteRendererComponent>(selection, {});
					ImGui::CloseCurrentPopup();
				}

				if (!manager.HasComponent<PlayerControllerComponent>(selection))
					if (ImGui::MenuItem("PlayerController"))
					{
						manager.AddComponent<PlayerControllerComponent>(selection, {});
						ImGui::CloseCurrentPopup();
					}

				ImGui::EndPopup();
			}

		}
		ImGui::End();
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.f, float columnWidth = 100.f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
		ImVec2 buttonSize = { lineHeight + 3.f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ .8f, .1f, .15f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ .9f, .2f, .2f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ .8f, .1f, .15f, 1.f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, .1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ .2f, .7f, .2f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ .3f, .8f, .3f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ .2f, .7f, .2f, 1.f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, .1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ .1f, .25f, .8f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ .2f, .35f, .9f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ .1f, .25f, .8f, 1.f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, .1f);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename Comp, typename UIFunction>
	static void DrawComponent(const char* name, ECS::ECSManager& manager, ECS::Entity entity, UIFunction uiFunction)
	{
		if (manager.HasComponent<Comp>(entity))
		{
			auto contentRegionAvailable = ImGui::GetContentRegionAvail();
			const ImGuiTreeNodeFlags treeNodeFlags = 
			ImGuiTreeNodeFlags_DefaultOpen | 
			ImGuiTreeNodeFlags_Framed | 
			ImGuiTreeNodeFlags_SpanAvailWidth | 
			ImGuiTreeNodeFlags_FramePadding | 
			ImGuiTreeNodeFlags_AllowItemOverlap;
		
			auto& component = manager.GetComponent<Comp>(entity);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
			
			bool open = ImGui::TreeNodeEx((void*)typeid(Comp).hash_code(), treeNodeFlags, name);
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * .5f);
			if (ImGui::Button("...", ImVec2{ lineHeight , lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

				ImGui::EndPopup();
			}
			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}
			ImGui::Separator();

			if (removeComponent)
				manager.RemoveComponent<Comp>(entity);
		}
	}

	void SceneInspectorPanel::DrawComponents(ECS::Entity entity)
	{
		auto& manager = m_Context->Manager();
		if (manager.HasComponent<TagComponent>(entity))
		{
			auto& tc = manager.GetComponent<TagComponent>(entity);
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			// permet d'afficher l'id 
			std::string tag = tc.Name;
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("##", buffer, sizeof(buffer)))
				tc.Name = buffer;

			ImGui::SameLine();
			std::string id = "(ID : " + std::to_string((uint32_t)entity) + ")";
			ImGui::Text(id.c_str());
			ImGui::Separator();
		}

		DrawComponent<TransformComponent>("Transform", manager, entity, [](auto& component)
			{
				DrawVec3Control("Translation", component.Translation);
				glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawVec3Control("Rotation", rotation);
				component.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", component.Scale, 1.f);
			});

		DrawComponent<CameraComponent>("Camera Component", manager, entity, [](auto& component)
			{
				auto& camera = component.Camera;

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (ImGui::Checkbox("Primary Camera", &component.Primary))
				{

				}
				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.SetOrthographicNearClip(orthoNear);

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.SetOrthographicFarClip(orthoFar);
				}
				else
				{
					float persFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &persFOV))
						camera.SetPerspectiveVerticalFOV(glm::radians(persFOV));

					float persNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &persNear))
						camera.SetPerspectiveNearClip(persNear);

					float persFar = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &persFar))
						camera.SetPerspectiveFarClip(persFar);
				}
				if (ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio))
				{
					FLOOD_ENGINE_TRACE("fixed aspect ratio");
				}
			});

		if (manager.HasComponent<CameraComponent>(entity))
		{
			if (m_Context->GetPrimaryCamera() == ECS::Null)
			{
				if (manager.GetComponent<CameraComponent>(entity).Primary)
					m_Context->SetPrimaryCamera(entity);
			}
			else if (m_Context->GetPrimaryCamera() != entity)
			{
				if (manager.GetComponent<CameraComponent>(entity).Primary)
				{
					manager.GetComponent<CameraComponent>(m_Context->GetPrimaryCamera()).Primary = false;
					m_Context->SetPrimaryCamera(entity);
				}
			}
			else
			{
				if (!manager.GetComponent<CameraComponent>(entity).Primary)
				{
					manager.GetComponent<CameraComponent>(entity).Primary = false;
					m_Context->SetPrimaryCamera(ECS::Null);
				}
			}
		}

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", manager, entity, [](auto& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

				ImGui::Text("Image");
				if (component.Texture != nullptr)
				{
					if (ImGui::ImageButton((ImTextureID)component.Texture->GetRendererID(), { 100.f, 100.f }, { 0 , 1 }, { 1, 0 }))
						component.Texture = nullptr;
				}
				else
					ImGui::Button("", { 100.f, 100.f });
					
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
						for (auto extension : g_SupportedImageFormats)
						{
							auto fileExtension = texturePath.extension().string();
							if (extension.compare(fileExtension) == 0)
							{
								component.Texture = Texture2D::Create(texturePath.string());
								break;
							}
						}
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.f);
			});
	
		DrawComponent<PlayerControllerComponent>("Player Controller", manager, entity, [](auto& component)
			{
				ImGui::DragFloat("Speed", &component.speed, 0.1f, 0.0f, 20.f);

				ImGui::Checkbox("Has a following camera", &component.isCameraFollowing);
				if (component.isCameraFollowing)
				{
					static int camId = 0;
					ImGui::SliderInt("Follow Camera", &camId, 0, 100);
					component.FollowingCamera = (ECS::Entity)(uint32_t)camId;
				}

			});
	}
}
