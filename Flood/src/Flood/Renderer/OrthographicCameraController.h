#ifndef ORTHOGRAPHICCAMERACONTROLLER__H
#define ORTHOGRAPHICCAMERACONTROLLER__H

#include "Flood/Renderer/OrthographicCamera.h"
#include "Flood/Core/Timestep.h"

#include "Flood/Events/ApplicationEvent.h"
#include "Flood/Events/MouseEvent.h"

namespace Flood
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float heigth);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; }
		float GetZoomLevel() const { return m_ZoomLevel; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;

		OrthographicCamera m_Camera;
		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 1.0f;
		float m_CameraRotationSpeed = 10.0f;
	};
}

#endif // ORTHOGRAPHICCAMERACONTROLLER__H