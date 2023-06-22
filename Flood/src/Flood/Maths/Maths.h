#ifndef MATHS__H
#define MATHS__H

#include <glm/glm.hpp>

namespace Flood::Maths {

	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);

}

#endif // !MATHS__H
