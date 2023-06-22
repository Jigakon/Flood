#ifndef APPLICATION__H
#define APPLICATION__H

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Flood/Events/Event.h"
#include "Flood/Events/ApplicationEvent.h"

#include "Timestep.h"

#include "Flood/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Flood
{
	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			FLOOD_ENGINE_ASSERT(index < Count, "index overflowing size of arguments");
			return Args[index];
		}
	};

	class Application
	{
	public:
		Application(const std::string& name = "Flood App", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		
		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; };

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		Scope<Window> m_Window;
		ApplicationCommandLineArgs m_CommandLineArgs;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimize = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};
	
	Application* CreateApplication(ApplicationCommandLineArgs args);

}

#endif //!APPLICATION__H
