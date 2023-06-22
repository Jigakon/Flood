#include "flpch.h"
#include "SubTexture2D.h"

namespace Flood
{

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& bottomLeft, const glm::vec2& topRight)
		: m_Texture(texture)
	{
		m_TexCoords[0] = { bottomLeft.x, bottomLeft.y };
		m_TexCoords[1] = { topRight.x, bottomLeft.y };
		m_TexCoords[2] = { topRight.x, topRight.y };
		m_TexCoords[3] = { bottomLeft.x, topRight.y };
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize)
	{
		float sheetWidth = texture->GetWidth();
		float sheetHeight = texture->GetHeight();

		glm::vec2 botLeft  = {((			   coords.x) * cellSize.x) / sheetWidth , ((			   coords.y) * cellSize.y) / sheetHeight};
		glm::vec2 topRight = {((spriteSize.x + coords.x) * cellSize.x) / sheetWidth , ((spriteSize.y + coords.y) * cellSize.y) / sheetHeight};

		return CreateRef<SubTexture2D>(texture, botLeft, topRight);
	}

}