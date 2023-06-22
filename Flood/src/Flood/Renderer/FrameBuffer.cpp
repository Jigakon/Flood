#include "flpch.h"
#include "FrameBuffer.h"

#include "Flood/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Flood
{

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& specs)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: FLOOD_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLFrameBuffer>(specs);
		}

		FLOOD_ENGINE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}