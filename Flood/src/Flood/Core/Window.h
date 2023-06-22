#ifndef WINDOW__H
#define WINDOW__H

#include "flpch.h"

#include "Flood/Core/Core.h"
#include "Flood/Events/Event.h"

namespace Flood
{
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Flood Engine", uint32_t width = 1600, unsigned int height = 900) 
		: Title(title), Width(width), Height(height) {}
	};

	class Window
	{
	public:
		// ajouter une liste pour les events possible (clavier, souris, fenêtre...)
		// traiter les différentes fonction dans le dispatcher en début de frame
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}

#endif //!WINDOW__H
