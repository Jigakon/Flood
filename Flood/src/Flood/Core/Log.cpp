#include "flpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Flood
{
	std::shared_ptr<spdlog::logger> Log::s_EngLogger;
	std::shared_ptr<spdlog::logger> Log::s_AppLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		
		s_EngLogger = spdlog::stdout_color_mt("FLOOD");
		s_EngLogger->set_level(spdlog::level::trace);
		
		s_AppLogger = spdlog::stdout_color_mt("APP");
		s_AppLogger->set_level(spdlog::level::trace);
	}

}