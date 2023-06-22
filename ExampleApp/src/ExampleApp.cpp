#include <Flood.h>
#include <Flood/Core/EntryPoint.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "ExampleLayer.h"
#include "Example2D.h"

class ExempleApp : public Flood::Application
{
public:
	ExempleApp(Flood::ApplicationCommandLineArgs args) : Application("ExampleApp", args)
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Example2D());
	}

	~ExempleApp()
	{

	}

};

Flood::Application* Flood::CreateApplication(ApplicationCommandLineArgs args)
{
	return new ExempleApp(args);
}
