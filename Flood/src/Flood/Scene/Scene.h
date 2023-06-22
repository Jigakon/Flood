#ifndef SCENE__H
#define SCENE__H

#include "Flood/ECS/Core/ECSManager.h"
#include "Flood/Core/Timestep.h"
#include "Flood/Renderer/EditorCamera.h"

namespace Flood
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		ECS::Entity CreateEntity() { return m_ECSManager.CreateEntity(); }
		ECS::ECSManager& Manager() { return m_ECSManager; }

		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnUpdateRuntime(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

		std::pair<uint32_t, uint32_t> GetViewportSize() { return { m_ViewportWidth, m_ViewportHeight }; }

		const ECS::Entity GetPrimaryCamera();
		void SetPrimaryCamera(ECS::Entity);
	private:
		ECS::Entity m_PrimaryCamera = ECS::Null;
		ECS::ECSManager m_ECSManager;
		uint32_t m_ViewportWidth, m_ViewportHeight;

		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}

#endif // !SCENE__H
