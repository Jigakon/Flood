#include "flpch.h"

#include "Flood/Renderer/GraphicsContext.h"

#include "Flood/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Flood
{

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    FLOOD_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		FLOOD_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}