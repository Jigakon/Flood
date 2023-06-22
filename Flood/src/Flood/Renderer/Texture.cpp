#include "flpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Flood
{

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: FLOOD_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported");
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);
		}

		FLOOD_ENGINE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: FLOOD_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported");
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(width, height);
		}

		FLOOD_ENGINE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}