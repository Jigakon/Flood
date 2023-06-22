#include "flpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Flood
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;

		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float fov, float clipNear, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;

		m_PerspectiveFOV = fov;
		m_PerspectiveNear = clipNear;
		m_PerspectiveFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		FLOOD_ENGINE_ASSERT(width > 0 && height > 0, "The Viewport size is too low ! (division by zero)");
		m_AspectRatio = (float)width / (float)height;

		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		if (m_ProjectionType == ProjectionType::Perspective)
		{
			m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
		else
		{
			float orthoLeft = -m_OrthographicSize * m_AspectRatio * .5f;
			float orthoRight = m_OrthographicSize * m_AspectRatio * .5f;
			float orthoBottom = -m_OrthographicSize * .5f;
			float orthoTop = m_OrthographicSize * .5f;

			m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop,
				m_OrthographicNear, m_OrthographicFar);
		}
	}
}
