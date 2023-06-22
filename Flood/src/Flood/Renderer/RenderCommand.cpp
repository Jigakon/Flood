#include "flpch.h"
#include "RenderCommand.h"

namespace Flood
{
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}
