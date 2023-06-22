#include "EditorLayer.h"
#include "imgui/imgui.h"
#include "ImGuizmo.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Flood/Scene/SceneSerializer.h"

#include "Flood/Utils/PlatformUtils.h"

#include "Flood/Maths/Maths.h"

//components
#include "Flood/ECS/Systems/CameraController.h"
#include "Flood/ECS/Components/CameraControllerComponent.h"
#include "Flood/ECS/Components/TagComponent.h"

namespace Flood
{
	EditorLayer::EditorLayer() : Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f) {}

	void EditorLayer::OnAttach()
	{
		FLOOD_PROFILE_FUNCTION();

		m_PlayImage = Texture2D::Create("Resources/Play.png");
		m_StopImage = Texture2D::Create("Resources/Stop.png");

		FrameBufferSpecification fbSpecs;
		fbSpecs.Attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INT, FrameBufferTextureFormat::Depth };
		fbSpecs.Width = 1280;
		fbSpecs.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(fbSpecs);

		m_ActiveScene = CreateRef<Scene>();

		m_EditorCamera = EditorCamera(30.f, 1.778f, 0.1f, 1000.f);

		auto& manager = m_ActiveScene->Manager();

		manager.RegisterSystem<CameraController, TransformComponent, CameraControllerComponent>();

		m_HierarchyPanel.SetContext(m_ActiveScene);
		m_InspectorPanel.SetContext(m_ActiveScene, &m_HierarchyPanel);
		m_ViewportPanel.SetContext(m_ActiveScene, &m_HierarchyPanel, this);
	}

	void EditorLayer::OnDetach()
	{
		FLOOD_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		FLOOD_PROFILE_FUNCTION();

		auto viewportSize = m_ViewportPanel.GetSize();
		if (FrameBufferSpecification spec = m_FrameBuffer->GetSpecifications();
			viewportSize.x > 0.0f && viewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != viewportSize.x || spec.Height != viewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			m_CameraController.OnResize(viewportSize.x, viewportSize.y);
			m_EditorCamera.SetViewportSize(viewportSize.x, viewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		}

		// Update all systems
		m_ActiveScene->Manager().OnUpdateEach(ts.GetSeconds());

		Renderer2D::ResetStats();

		m_FrameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.11f, 1.f });
		RenderCommand::Clear();

		m_FrameBuffer->ClearAttachment(1, -1);

		switch (m_SceneState)
		{
			case SceneState::Edit:
			{
				if (m_ViewportPanel.IsFocused())
					m_CameraController.OnUpdate(ts);

				m_EditorCamera.OnUpdate(ts);

				m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
			}
			break;
			case SceneState::Play:
			{
				m_ActiveScene->OnUpdateRuntime(ts);
			}
			break;
		}


		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportPanel.GetBounds(0).x;
		my -= m_ViewportPanel.GetBounds(0).y;
		auto currentViewportSize = m_ViewportPanel.GetBounds(1) - m_ViewportPanel.GetBounds(0);
		my = currentViewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < currentViewportSize.x && mouseY < currentViewportSize.y)
		{
			int pixelData = m_FrameBuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? ECS::Null : pixelData;
		}

		m_FrameBuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		FLOOD_PROFILE_FUNCTION();

		static bool dockSpaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Dockspace Demo", &dockSpaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open Scene...", "Ctrl+O"))
					OpenScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				ImGui::Separator();

				if (ImGui::MenuItem("Exit")) 
					Application::Get().Close();

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		m_HierarchyPanel.OnImGuiRender();
		m_InspectorPanel.OnImGuiRender();

#pragma region stats panel
		ImGui::Begin("Stats");

		std::string name = "None";
		if (m_HoveredEntity != ECS::Null)
			name = m_ActiveScene->Manager().GetComponent<TagComponent>(m_HoveredEntity).Name;
		ImGui::Text("Hovered Entity : %s", name.c_str());

		auto renderer2DStats = Renderer2D::GetStats();
		ImGui::Text("DrawCalls : %d", renderer2DStats.DrawCalls);
		ImGui::Text("Quads : %d", renderer2DStats.QuadCount);
		ImGui::Text("Vertices : %d", renderer2DStats.GetTotalVertexCount());
		ImGui::Text("Indices : %d", renderer2DStats.GetTotalIndexCount());

		ImGui::End(); // Settings End
#pragma endregion

		UI_ToolBar();

		m_ViewportPanel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();

		ImGui::End(); // DockSpace End
	}

	void EditorLayer::OnEvent(Flood::Event& e)
	{
		m_CameraController.OnEvent(e);
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyPressed, this));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed, this));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
			return false;

		bool controlKey = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shiftKey = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (e.GetKeyCode())
		{
		case Key::N:
		{
			if (controlKey)
				NewScene();
			break;
		}
		case Key::O:
		{
			if (controlKey)
				OpenScene();
			break;
		}
		case Key::S:
		{
			if (controlKey && shiftKey)
				SaveSceneAs();
			break;
		}
		// gizmos
		case Key::Q:
			m_GizmoType = -1;
			break;
		case Key::W:
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		case Key::E:
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		case Key::R:
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft)
			if (m_SceneState == SceneState::Edit && m_ViewportPanel.IsHovered() && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				m_HierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		return false;
	}


	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		auto viewportSize = m_ViewportPanel.GetSize();
		m_ActiveScene->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		m_HierarchyPanel.SetContext(m_ActiveScene);
		m_InspectorPanel.SetContext(m_ActiveScene, &m_HierarchyPanel);
		m_ViewportPanel.SetContext(m_ActiveScene, &m_HierarchyPanel, this);
	}

	void EditorLayer::OpenScene()
	{
		auto filePath = FileDialogs::OpenFile("Flood Scene (*.flood)\0*.flood\0");
		if (!filePath.empty())
			OpenScene(filePath);
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		m_ActiveScene = CreateRef<Scene>();
		auto viewportSize = m_ViewportPanel.GetSize();
		m_ActiveScene->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		m_HierarchyPanel.SetContext(m_ActiveScene);
		m_InspectorPanel.SetContext(m_ActiveScene, &m_HierarchyPanel);
		m_ViewportPanel.SetContext(m_ActiveScene, &m_HierarchyPanel, this);

		SceneSerializer sceneSerializer(m_ActiveScene);
		sceneSerializer.Deserialize(path.string());
	}

	void EditorLayer::SaveSceneAs()
	{
		auto filePath = FileDialogs::SaveFile("Flood Scene (*.flood)\0*.flood\0");
		if (!filePath.empty())
		{
			SceneSerializer sceneSerializer(m_ActiveScene);
			sceneSerializer.Serialize(filePath);
		}
	}

	void EditorLayer::UI_ToolBar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		auto& btnHov = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(btnHov.x, btnHov.y, btnHov.z, .5f));
		auto& btnAct = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(btnAct.x, btnAct.y, btnAct.z, .5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		float size = ImGui::GetWindowHeight() - 4.f;
		ImTextureID img = (ImTextureID)(m_SceneState == SceneState::Edit ? m_PlayImage : m_StopImage)->GetRendererID();
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * .5f) - (size * .5f));
		if (ImGui::ImageButton(img, { size, size }, { 0, 0 }, { 1, 1 }, 0))
		{
			switch (m_SceneState)
			{
				case SceneState::Edit:
					OnScenePlay();
					break;
				case SceneState::Play:
					OnSceneStop();
					break;
			}
		}

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void EditorLayer::OnScenePlay()
	{
		m_SceneState = SceneState::Play;
	}

	void EditorLayer::OnSceneStop()
	{
		m_SceneState = SceneState::Edit;
	}

}
