#include "flpch.h"
#include "Flood/Core/Window.h"

#ifdef FLOOD_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Flood
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef FLOOD_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		FLOOD_ASSERT(false, "Unknown platform");
		return nullptr;
#endif
	}
}
