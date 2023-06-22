#include <Flood.h>
#include <Flood/Core/EntryPoint.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "ExampleLayer.h"
#include "EditorLayer.h"

namespace Flood
{
	class FloodEditor : public Application
	{
	public:
		FloodEditor(ApplicationCommandLineArgs args) : Application("Flood Editor", args)
		{
			PushLayer(new EditorLayer());
		}

		~FloodEditor()
		{

		}

	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		return new FloodEditor(args);
	}
}
