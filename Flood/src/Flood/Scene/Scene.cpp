#include "flpch.h"
#include "Scene.h"

#include "Flood/ECS/Core/ECSManager.h"

#include "Flood/ECS/Components/Components.h"
#include "Flood/ECS/Components/CameraComponent.h"
#include "Flood/Renderer/Renderer2D.h"
#include "Flood/ECS/Components/PlayerController.h"

namespace Flood
{
	Scene::Scene()
	{
		m_ECSManager.Init();
	}

	Scene::~Scene()
	{

	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);
		auto group = m_ECSManager.Query<TransformComponent, SpriteRendererComponent>();
		for (auto entity : group)
		{
			auto& transform = m_ECSManager.GetComponent<TransformComponent>(entity);
			auto& sprite = m_ECSManager.GetComponent<SpriteRendererComponent>(entity);

			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}
		Renderer2D::EndScene();
	}

	glm::vec2 normalize(const glm::vec2& vec)
	{
		float length = sqrtf((vec.x * vec.x) + (vec.y * vec.y));
		if (length == 0) return { 0.f, 0.f };
		return glm::vec2(vec.x / length, vec.y / length);
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// find and set mainCamera => there is a better way to do this
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto group = m_ECSManager.Query<TransformComponent, CameraComponent>();
			for (auto entity : group)
			{
				auto& camera = m_ECSManager.GetComponent<CameraComponent>(entity);
				if (camera.Primary)
				{
					auto& transform = m_ECSManager.GetComponent<TransformComponent>(entity);
					cameraTransform = transform.GetTransform();
					mainCamera = &camera.Camera;
					break;
				}
			}
		}

		// system controller => can be change to a system itself, run out of time
		{
			auto group = m_ECSManager.Query<TransformComponent, PlayerControllerComponent>();
			for (auto entity : group)
			{
				auto& transform = m_ECSManager.GetComponent<TransformComponent>(entity);
				auto& controller = m_ECSManager.GetComponent<PlayerControllerComponent>(entity);
				
				glm::vec2 dir{ 0.f, 0.f };

				if (Input::IsKeyPressed(controller.up))
					dir.y += 1.f;
				if (Input::IsKeyPressed(controller.down))
					dir.y -= 1.f;
				if (Input::IsKeyPressed(controller.right))
					dir.x += 1.f;
				if (Input::IsKeyPressed(controller.left))
					dir.x -= 1.f;

				dir = normalize(dir) * controller.speed * ts.GetSeconds();
				transform.Translation.x += dir.x;
				transform.Translation.y += dir.y;
				if (controller.isCameraFollowing && controller.FollowingCamera != ECS::Null)
				{
					if (!m_ECSManager.HasComponent<TransformComponent>(controller.FollowingCamera)) continue;
					auto& cameraTransform = m_ECSManager.GetComponent<TransformComponent>(controller.FollowingCamera);
					cameraTransform.Translation.x += dir.x;
					cameraTransform.Translation.y += dir.y;
				}
			}
		
		}

		// system render => can be change to a system itself, run out of time
		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);
			auto group = m_ECSManager.Query<TransformComponent, SpriteRendererComponent>();
			for (auto entity : group)
			{
				auto& transform = m_ECSManager.GetComponent<TransformComponent>(entity);
				auto& sprite = m_ECSManager.GetComponent<SpriteRendererComponent>(entity);

				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
			}
			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto query = m_ECSManager.Query<CameraComponent>();
		for (auto entity : query)
		{
			auto& cameraComponent = m_ECSManager.GetComponent<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

	const ECS::Entity Scene::GetPrimaryCamera()
	{
		return m_PrimaryCamera;

		auto group = m_ECSManager.Query<CameraComponent>();
		for (auto entity : group)
		{
			if (m_ECSManager.GetComponent<CameraComponent>(entity).Primary)
				return entity;
		}
		return ECS::Null;
	}

	void Scene::SetPrimaryCamera(ECS::Entity entity)
	{
		m_PrimaryCamera = entity;
	}

}
