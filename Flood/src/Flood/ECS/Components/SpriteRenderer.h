#ifndef SPRITERENDERER__H
#define SPRITERENDERER__H

#include <glm/glm.hpp>

#include "Flood/Renderer/Texture.h"

namespace Flood
{
	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		Ref<Texture2D> Texture;
		float TilingFactor = 1.f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {};
	};
}
#endif // !SPRITERENDERER__H
