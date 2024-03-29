#ifndef SCENECAMERA__H
#define SCENECAMERA__H

#include "Flood/Renderer/Camera.h"

namespace Flood
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };

	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float clipNear, float farClip);
		void SetPerspective(float fov, float clipNear, float farClip);
		void SetViewportSize(uint32_t width, uint32_t height);
		

		float GetPerspectiveVerticalFOV() const { return m_PerspectiveFOV; };
		void SetPerspectiveVerticalFOV(float verticalFOV) { m_PerspectiveFOV = verticalFOV; RecalculateProjection(); };

		float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }

		float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }


		float GetOrthographicSize() const { return m_OrthographicSize; };
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); };

		float GetOrthographicNearClip() const { return m_OrthographicNear; }
		void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }

		float GetOrthographicFarClip() const { return m_OrthographicFar; }
		void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }


		ProjectionType GetProjectionType() { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }

	private:
		void RecalculateProjection();

	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f;
		float m_OrthographicFar  = 1.0f;

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = .01f;
		float m_PerspectiveFar = 1000.0f;

		float m_AspectRatio = 0.0f;
	};
}

#endif // ! SCENECAMERA__H
