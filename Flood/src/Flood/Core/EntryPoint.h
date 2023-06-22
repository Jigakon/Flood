#ifndef ENTRYPOINT__H
#define ENTRYPOINT__H

#include "Flood/Core/Core.h"

#ifdef FLOOD_PLATFORM_WINDOWS

extern Flood::Application* Flood::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	Flood::Log::Init();
	FLOOD_ENGINE_INFO("Initialized Engine Logger");
	FLOOD_APP_INFO("Initialized Application Logger");

	FLOOD_PROFILE_BEGIN_SESSION("Startup", "FloodProfile-Startup.json");
	auto app = Flood::CreateApplication({argc, argv});
	FLOOD_PROFILE_END_SESSION();

	FLOOD_PROFILE_BEGIN_SESSION("Runtime", "FloodProfile-Runtime.json");
	app->Run();
	FLOOD_PROFILE_END_SESSION();

	FLOOD_PROFILE_BEGIN_SESSION("Startup", "FloodProfile-Shutdown.json");
	delete app;
	FLOOD_PROFILE_END_SESSION();
}

#endif

#endif //!ENTRYPOINT__H
