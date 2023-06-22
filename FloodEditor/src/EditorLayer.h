#ifndef EDITOR_LAYER__H
#define EDITOR_LAYER__H

#include <Flood.h>
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/SceneInspectorPanel.h"
#include "Panels/ViewportPanel.h"
#include "Panels/ContentBrowserPanel.h"

#include "Flood/Renderer/EditorCamera.h"

namespace Flood
{

	class EditorLayer : public Layer
	{
	public:
		enum class SceneState
		{
			Edit = 0, Play = 1
		};

		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& event) override;

		const EditorCamera& GetEditorCamera() const { return m_EditorCamera; }
		int GetGizmoType() const { return m_GizmoType; }
		Ref<FrameBuffer> GetFrameBuffer() const { return m_FrameBuffer; }

		SceneState GetSceneState() const { return m_SceneState; }

	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();

		void UI_ToolBar();

		void OnScenePlay();

		void OnSceneStop();
	// trouver autre chose pour le faire dans le viewport
	public:
		void OpenScene(const std::filesystem::path& path);
	
	private:
		OrthographicCameraController m_CameraController;

		Ref<FrameBuffer> m_FrameBuffer;

		Ref<Scene> m_ActiveScene;

		ECS::Entity m_SquareEntity;
		ECS::Entity m_Camera;
		ECS::Entity m_HoveredEntity = ECS::Null;

		EditorCamera m_EditorCamera;

		int m_GizmoType = -1;

		//Panels
		SceneHierarchyPanel m_HierarchyPanel;
		SceneInspectorPanel m_InspectorPanel;
		ViewportPanel m_ViewportPanel;
		ContentBrowserPanel m_ContentBrowserPanel;

		Ref<Texture2D> m_PlayImage;
		Ref<Texture2D> m_StopImage;

		
		SceneState m_SceneState = SceneState::Edit;
	};
}

#endif // !EDITOR_LAYER__H
