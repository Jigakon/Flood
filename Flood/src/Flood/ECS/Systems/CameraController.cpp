#include "flpch.h"
#include "CameraController.h"

#include "Flood/Core/Input.h"
#include "Flood/ECS/Core/ECSManager.h"
#include "Flood/ECS/Components/Transform.h"
#include "Flood/ECS/Components/CameraControllerComponent.h"

namespace Flood
{
	void CameraController::OnUpdate(float dt)
	{
		for (auto& entity : m_Entities)
		{
			auto& translation = m_Manager->GetComponent<TransformComponent>(entity).Translation;
			auto& CameraController = m_Manager->GetComponent<CameraControllerComponent>(entity);

			if (Input::IsKeyPressed(Key::A))
				translation.x -= CameraController.speed * dt;
			if (Input::IsKeyPressed(Key::D))
				translation.x += CameraController.speed * dt;
			if (Input::IsKeyPressed(Key::S))
				translation.y -= CameraController.speed * dt;
			if (Input::IsKeyPressed(Key::W))
				translation.y += CameraController.speed * dt;
		}

	}
}
