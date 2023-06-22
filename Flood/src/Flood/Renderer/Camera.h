#ifndef CAMERA__H
#define CAMERA__H

#include <glm/glm.hpp>

namespace Flood
{
	class Camera
	{
	public:
		Camera() = default;
		virtual ~Camera() = default;
		Camera(const glm::mat4& projection) : m_Projection(projection) {}
	
		const glm::mat4& GetProjection() const { return m_Projection; }

	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};
}

#endif // !CAMERA__H
