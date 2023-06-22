#ifndef OPENGLRENDERERAPI__H
#define OPENGLRENDERERAPI__H

#include "Flood/Renderer/RendererAPI.h"

namespace Flood
{
	class OpenGLRendererAPI : public RendererAPI 
	{
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
	};
}

#endif // !OPENGLRENDERERAPI__H