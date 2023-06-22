#ifndef SUBTEXTURE2D__H
#define SUBTEXTURE2D__H

#include <glm/glm.hpp>
#include "Flood/Renderer/Texture.h"

namespace Flood
{
	class SubTexture2D
	{
		public:
			SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& bottomLeft, const glm::vec2& topRight);

			const Ref<Texture2D> GetTexture() const { return m_Texture; }
			const glm::vec2* GetTexCoords() const { return m_TexCoords; }

			static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = { 1.0f, 1.0f });
		private:
			Ref<Texture2D> m_Texture;
	
			glm::vec2 m_TexCoords[4];
	};
}

#endif // !SUBTEXTURE2D__H